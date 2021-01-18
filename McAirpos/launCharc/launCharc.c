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
    system("clear");
    char* game = "";
    char* options = "";
    if (argc == 2) {
        game = argv[1];
    } else if (argc == 3) {
	game = argv[2];
	options = argv[1];
    } else if ((argc > 3) || (argc < 2)) {
        printf("usage: launchArcade [nomap] [/path/to/arcadegame.elf]\n");
        return 1;
    }


   // Check for nomap option
   if (!strcmp(options, "nomap")) {
	printf("%s argument detected,\nStarting %s with no automatic gamepad mappings...\n", options, game);
	sleep(1);
   } else {
	// Determine the number of connected gamepads
	printf("Starting %s with automatic gamepad mappings...\n\n", game);
	char eventPaths[100];
	memset (eventPaths, 0, sizeof(eventPaths));
	int numberOfPads = 0;
	int numberOfEvents = 1 + atoi(getSystemOutput("ls /dev/input | sed 's/event//' | sort -n | tail -1"));
       	printf("\nHighest found input event number: %d\n\n", numberOfEvents);
	char padEvent[2][20];
	for (int i = 0; i < numberOfEvents; i++) {
      	    if (numberOfPads < 2) {
      	    	char padCommand[150];
      	    	snprintf(padCommand, 150, "/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -vp /dev/input/event%d 2>&1 | grep -e BTN_SOUTH -e BTN_PINKIE", i);
      	    	char* event = getSystemOutput(padCommand);
      	    	if (strcmp(event, "")) {
		    if (numberOfPads == 0) {
	    		printf("\nGamepad search hits:\n");
		    }
         	    printf("%s, Output:%s", padCommand, getSystemOutput(padCommand));
         	    char iString[20];
         	    sprintf(iString, "%d", i);
         	    strcat(strcat(strcat(eventPaths, "/dev/input/event"), iString), " ");
         	    strcat(strcat(strcat(eventPaths, "/dev/input/event"), iString), " ");
		    memset (padEvent[numberOfPads], 0, sizeof(padEvent[numberOfPads]));
		    strcat(strcat(padEvent[numberOfPads], "/dev/input/event"), iString);
	       	    numberOfPads++;
   	    	}
      	    }
    	}

	// Determine if keyboard is connected
    	char keybCommand[] = "cat /proc/bus/input/devices | grep -B 7 EV=12001 | tr ' ' '\\n' | grep event";
    	char* keybEvent = getSystemOutput(keybCommand);
	if (strcmp(keybEvent, "")) {
	    printf("\nKeyboard search hit:\n");
       	    printf("%s, Output:%s", keybCommand, getSystemOutput(keybCommand));
	}


    	// Automatically set up uinput-mapper with keyboard, 1 or 2 gamepads
    	// printf("\n%s\n", eventPaths);
    	char stringNumberOfPads[20];
      	char uiMapCommand[400];
      	memset (uiMapCommand, 0, sizeof(uiMapCommand));
	char uinputMapperOrKeyboard[20];
    	memset (uinputMapperOrKeyboard, 0, sizeof(uinputMapperOrKeyboard));
    	char defaultEvent[67];
    	memset (defaultEvent, 0, sizeof(defaultEvent));
    	strcat(defaultEvent, "SCAN_CODES=/dev/input/");
	printf("\n");
	if ((numberOfPads == 0) && !strcmp(keybEvent, "")) {
	    printf("\nFound no gamepads or keyboards to configure...\n");
	    printf("\nPlease try the \"nomap\" option and configure /sd/arcade.cfg manually. If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	    sleep(1);
	    exit(1);
	}
	else if (numberOfPads == 2) {
	    printf("\nFound %d gamepads to configure on:\n%s, and\n%s\n\n", numberOfPads, padEvent[0], padEvent[1]);
	    sprintf(stringNumberOfPads, "%d.py)&",numberOfPads);
	}
	else if ((numberOfPads == 1) && strcmp(keybEvent, "")) {
	    printf("\nFound %d gamepad to configure on:\n%s\n", numberOfPads, padEvent[0]);
	    printf("\nFound 1 keyboard to configure on:\n/dev/input/%s\n\n", keybEvent);
            strcat(strcat(eventPaths, "/dev/input/"), keybEvent);
            eventPaths[strlen(eventPaths)-1] = 0;
            strcat(eventPaths, " ");
	    sprintf(stringNumberOfPads, "%d.py)&",numberOfPads);
	}
	else if (strcmp(keybEvent, "")) {
	    printf("\nFound 1 keyboard to configure on:\n/dev/input/%s\n\n", keybEvent);
      	    strcat(defaultEvent, keybEvent);
      	    defaultEvent[strlen(defaultEvent)-1] = 0;
	    strcat(uinputMapperOrKeyboard, "keyboard");
	}
	else if (numberOfPads == 1) {
	    printf("\nFound %d gamepad to configure on:\n%s\n\n", numberOfPads, padEvent[0]);
	    sprintf(stringNumberOfPads, "%d.py)&",numberOfPads);
	}
	else {
	    printf("\nSomething went wrong, exiting...\n");
	    printf("If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	    sleep(1);
	    exit(1);
	}


	// Launching uinput-mapper
	if (numberOfPads > 0) {
            strcat(strcat(strcat(strcat(uiMapCommand, "(/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -C -D "), eventPaths), "| sudo /home/pi/McAirpos/McAirpos/uinput-mapper/input-create -C -S /home/pi/McAirpos/McAirpos/uinput-mapper/configs/arcade"), stringNumberOfPads);
      	    if (system(uiMapCommand) == 0) {
	    	printf("\nStarting UInput-Mapper with command:\n%s\n", uiMapCommand);
	    	int whileCount = 0;
	    	while (!strcmp("", getSystemOutput("cat /proc/bus/input/devices | grep -A 8 \"UInputMapper: MakeCode_Arcade\" | tr ' ' '\n' | grep event"))) {
	    	    if (whileCount > 500) {
	       	    	printf("\nTimed out trying to set up UInput-Mapper...\n");
		        printf("If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	       	    	exit(1);
	    	    }
	    	whileCount++;
	    	}
	    	char* uinputEvent = getSystemOutput("cat /proc/bus/input/devices | grep -A 8 \"UInputMapper: MakeCode_Arcade\" | tr ' ' '\n' | grep event");
      	    	strcat(defaultEvent, uinputEvent);
            	defaultEvent[strlen(defaultEvent)-1] = 0;
		strcat(uinputMapperOrKeyboard, "UInputMapper");
	    } else {
	    	printf("\nUInput-Mapper failed to start...\n");
	        printf("If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	    	exit(1);
      	    }
	    // Alternative way to launch uinput-mapper
      	    /*if (!fork()) {
           	setpgid(0, 0);
           	system(uiMapCommand);
	    }else {}*/
	}


	// Set default default event for MakeCode Arcade elf game file
    	char sedCommand[100];
    	snprintf(sedCommand, 100, "sed -i \"1s&.*&\"%s\"&\" /sd/arcade.cfg", defaultEvent);
    	if (system(sedCommand) == 0) {
    	    printf("\n\nSetting up %s in MakeCode Arcade game's /sd/arcade.cfg with:\n%s\n\n", uinputMapperOrKeyboard, defaultEvent);
	} else {
	    printf("\n\nPlease check path or write permissions for /sd/arcade.cfg and try again.\n\n");
	    goto cleanup;
	}
    }
    system("stty -ixon");  //Disable pause in terminal with key combo CTRL+S


    // Fork game execution on launch, so that it is executed
    // the same way it's done in-game on reset and finish
    char* path = "/dev/tty";
    int fd = open(path, O_RDWR, 0);
    if  (!fork()) {
	//Switch console to graphics mode to avoid disturbing text output in borders
        if (fd < 0) {
            perror("unable to open tty");
            return 1;
        }
        if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0) {
            perror("warn: ioctl KDSETMODE failed");
        }
	close(fd);
	system("sudo killall pulseaudio");
        if (system(game) == 36608) {
	    printf("\n\n%s was exited by the user\n\n\n", game);
	} else {
	    printf("\n\nPlease check path to and executable permissions for game_file.elf and try again.\n\n\n");
	}
	//Alternative way to launch game
        //execl(game, game, NULL);
    }else {
        sleep(1);  // Wait for fork/game to launch and get pid

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
        snprintf(killAllCmd, 100, "killall %s 2>&1", processName);
        system(killAllCmd);
cleanup:
        system("sudo killall input-create 2>&1 & sudo killall input-read 2>&1");


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
            //perror("warn: ioctl VT_UNLOCKSWITCH failed");
        }

        if (ioctl(fd, KDSETMODE, KD_TEXT) < 0) {
            perror("warn: ioctl KDSETMODE failed");
        }

        if (ioctl(fd, KDSKBMODE, K_XLATE) < 0) {
            perror("warn: ioctl KBSKBMODE failed");
        }

        system("stty ixon");
        //system("clear");
    }

    return 0;
}
