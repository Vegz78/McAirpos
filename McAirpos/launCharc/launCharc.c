/* MakeCode Arcade native game file launcher for RetroPie/Raspberry Pi OS.
 *
 * Based on Terminal fixer * https://github.com/hobbitalistair/termfix,
 *
 * which in turn was based on
 * https://gist.github.com/joerick/9e2d244f456c2431619e7063eda62e1d
 *
 * launCharc wraps the MakeCode Arcade game ELF executables' process, so
 * that RetroPie does not reactivate EmulationStation with the game still
 * running in the background every time the game finishes or resets.
 * Restores the terminal or EmulationStation GUI session, which is messed
 * up by the MakeCode Arcade ELF executable game file, that does not
 * release the framebuffer or raw input modes on exit.
 *
 * This file is a part of the https://github.com/vegz78/McAirpos repository.
 *
 * MIT License * * Copyright (c) 2020 Vegz78
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/keyboard.h>


// Function which returns results from shell commands run through system()
char* getSystemOutput(char* inputCommand) {

    FILE *fp;
    static char systemOutput[100];
    memset (systemOutput, 0, sizeof(systemOutput));
    fp = popen(inputCommand, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    while (fgets(systemOutput, sizeof(systemOutput), fp) != NULL) {
    }
    pclose(fp);

    return systemOutput; 
}


// Main function
int main(int argc, char** argv) {

    // Read game file argument to execute
    char* game = "";
    if (argc == 2) {
        game = argv[1];
    } else if (argc > 2) {
        printf("usage: launchArcade [/path/to/arcadegame.elf]\n");
        return 1;
    }


   // Determine the number of connected gamepads
   printf("%s\n", game);
   char eventPaths[100];
   memset (eventPaths, 0, sizeof(eventPaths));
   int numberOfPads = 0;
   int numberOfEvents = 1 + atoi(getSystemOutput("ls /dev/input | sed 's/event//' | sort -n | tail -1"));
   for (int i = 0; i < numberOfEvents; i++) {
      if (numberOfPads < 2) {
      char processCommand[100];
      snprintf(processCommand, 100, "/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -vp /dev/input/event%d | grep DPAD", i);
      char* event = getSystemOutput(processCommand);
      if (strcmp(event, "")) {
         printf("%s, Output:%s\n", processCommand, getSystemOutput(processCommand));
         printf("%d Possible gamepads\n", numberOfEvents);
         char iString[20];
         sprintf(iString, "%d", i);
         strcat(strcat(strcat(eventPaths, "/dev/input/event"), iString), " ");
         numberOfPads++;
      }
      }
   }


   // Set up MakeCode Arcade and uinput-mapper with keyboard, 1 or 2 gamepads
   printf("\n%s\n", eventPaths);
   char defaultEvent[67];
   memset (defaultEvent, 0, sizeof(defaultEvent));
   strcat(defaultEvent, "SCAN_CODES=/dev/input/");
   char sedCommand[100]; 
   char* tempString = getSystemOutput("cat /proc/bus/input/devices | grep -B 7 EV=12001 | tr ' ' '\n' | grep event");
   if ((numberOfPads == 1) || (numberOfPads == 2)) {
      if (numberOfPads == 1) {
         strcat(strcat(eventPaths, "/dev/input/"), tempString);
         eventPaths[strlen(eventPaths)-1] = 0;
         strcat(eventPaths, " ");
      }
      char newEventNo[20];
      sprintf(newEventNo, "%d", numberOfEvents);
      strcat(strcat(defaultEvent, "event"), newEventNo);
      char uiMapCommand[400];
      memset (uiMapCommand, 0, sizeof(uiMapCommand));
      sprintf(newEventNo, "%d.py)&",numberOfPads); 
      strcat(strcat(strcat(strcat(uiMapCommand, "(/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -C -D "), eventPaths), "| sudo /home/pi/McAirpos/McAirpos/uinput-mapper/input-create -C -S /home/pi/McAirpos/McAirpos/uinput-mapper/configs/arcade"), newEventNo);
      printf("%s\n", uiMapCommand);
      /*if (!fork()) {
         setpgid(0, 0);
         system(uiMapCommand);
      }else {}*/
      system(uiMapCommand);
   }
   if (numberOfPads == 0) {
      strcat(defaultEvent, tempString);
      defaultEvent[strlen(defaultEvent)-1] = 0;
   }
   snprintf(sedCommand, 100, "sed -i \"1s&.*&\"%s\"&\" /sd/arcade.cfg", defaultEvent);
   system(sedCommand);
   system("stty -ixon");

    // Fork game execution on launch, so that it is executed
    // the same way it's done in-game on reset and finish
    if  (!fork()) {
        sleep(1);
        system(game);
        //execl(game, game, NULL);
    }else {
        sleep(2);  // Wait for fork/game to launch and get pid

	//Switch console to graphics mode to avoid disturbing text output in borders
        char* path = "/dev/tty";
        int fd = open(path, O_RDWR, 0);
        if (fd < 0) {
            perror("unable to open tty");
            return 1;
        }
	system("clear");
        if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0) {
            perror("warn: ioctl KDSETMODE failed");
        }
	close(fd);
        system("clear");

        // Get running game's process name
        char* processID = getSystemOutput("head -1 /tmp/pxt-pid");
        char processCommand[100];
        snprintf(processCommand, 100, "head -1 /proc/%s/comm", processID);
        char* processName = getSystemOutput(processCommand);
        char processCheckCmd[100];
        snprintf(processCheckCmd, 100, "pgrep %s", processName);

        // Check every 2 secounds if a game process is still active
        // Busy waiting to continue and cleanup when game is exited
        // Why does this not work without the printf?!?
besure:
        while (0 == system(processCheckCmd)) {
            printf("%s\n", getSystemOutput(processCheckCmd));
            sleep(2);
        }

        // Double check that game really has exited
        sleep(1);
        if (0 == system(processCheckCmd)) {goto besure;}

        // Kill any remaining/orphaned game processes before exit
        char killAllCmd[100];
        snprintf(killAllCmd, 100, "killall %s", processName);
        system(killAllCmd);
        system("sudo killall input-create&&sudo killall input-read");


        // The following code is borrowed from https://github.com/hobbitalistair/termfix:
        // Terminal Fixer's cleanup part, which returns control of the
        // framebuffer and input mode to calling process after game's exit
        fd = open(path, O_RDWR, 0);
        if (fd < 0) {
            perror("unable to open tty");
            return 1;
        }

        // This one fails without sudo, but doesn't seem needed for 
        // MakeCode Arcade games(comment out or leave as and option?).
        if (ioctl(fd, VT_UNLOCKSWITCH, 1) < 0) {
            perror("warn: ioctl VT_UNLOCKSWITCH failed");
        }

        if (ioctl(fd, KDSETMODE, KD_TEXT) < 0) {
            perror("warn: ioctl KDSETMODE failed");
        }

        if (ioctl(fd, KDSKBMODE, K_XLATE) < 0) {
            perror("warn: ioctl KBSKBMODE failed");
        }

        system("stty -ixon");
        system("clear");
    }

    return 0;
}
