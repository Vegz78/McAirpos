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
    char* options = "";
    if (argc == 2) {
        game = argv[1];
    } else if (argc == 3) {
	game = argv[2];
	options = argv[1];
    } else if ((argc > 3) || (argc < 2)) {
        printf("usage: launCharc [nomap / keybswap] [/path/to/arcadegame.elf]\n");
        return 1;
    }


   // Check for nomap option
   system("clear");
   if (!strcmp(options, "nomap")) {
	printf("%s argument detected,\nlaunCharc starting %s with no automatic gamepad mappings...\n", options, game);
	sleep(1);
   } else {
	// Determine the number of connected gamepads
	printf("launCharc starting %s with automatic gamepad mappings...\n", game);
	char eventPaths[100];
	memset (eventPaths, 0, sizeof(eventPaths));
	int numberOfPads = 0;
	int numberOfEvents = 1 + atoi(getSystemOutput("ls /dev/input | sed 's/event//' | sort -n | tail -1 | tr -d [:cntrl:]"));
       	printf("\nHighest found input event number: %d\n", numberOfEvents);
	char padEvent[2][20];
	memset (padEvent, 0, sizeof(padEvent));
	for (int i = 0; i < numberOfEvents; i++) {
      	    if (numberOfPads < 2) {
      	    	char padCommand[150];
		memset (padCommand, 0, sizeof(padCommand));
      	    	snprintf(padCommand, 150, "/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -vp /dev/input/event%d 2>&1 | grep -e BTN_START -e BTN_SOUTH -e BTN_PINKIE", i);
      	    	char* event = getSystemOutput(padCommand);
      	    	if (strcmp(event, "")) {
		    if (numberOfPads == 0) {
	    		printf("Gamepad search hits:\n");
		    }
         	    printf("%s, Output:%s", padCommand, getSystemOutput(padCommand));
         	    char iString[20];
		    memset (iString, 0, sizeof(iString));
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
	char keybCommand[300];
      	memset (keybCommand, 0, sizeof(keybCommand));
	if (!strcmp(options, "keybswap")) {
    	    strcat(keybCommand, "cat /proc/bus/input/devices | grep -B 5 -A 5 Handlers=sysrq | grep -B 7 -A 3 -e EV=12001 -e EV=10001 | grep -B 2 -A 8 -E 'Phys=(usb\\S+\\/input0|[a-zA-Z0-9]{2}(:[a-zA-Z0-9]{2}){5}.*)' | tr ' ' '\\n' | grep event | tail -1 | tr -d [:cntrl:]");
	} else {
    	    strcat(keybCommand, "cat /proc/bus/input/devices | grep -B 5 -A 5 Handlers=sysrq | grep -B 7 -A 3 -e EV=12001 -e EV=10001 | grep -B 2 -A 8 -E 'Phys=(usb\\S+\\/input0|[a-zA-Z0-9]{2}(:[a-zA-Z0-9]{2}){5}.*)' | tr ' ' '\\n' | grep event | head -1 | tr -d [:cntrl:]");
	}
    	char* keybEvent = getSystemOutput(keybCommand);
	if (strcmp(keybEvent, "")) {
	    printf("Keyboard search hit:\n");
       	    printf("%s, Output:%s", keybCommand, getSystemOutput(keybCommand));
	}


    	// Automatically set up uinput-mapper with keyboard, 1 or 2 gamepads
    	char stringNumberOfPads[20];
      	memset (stringNumberOfPads, 0, sizeof(stringNumberOfPads));
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
            strcat(eventPaths, " ");
	    sprintf(stringNumberOfPads, "%d.py)&",numberOfPads);
	}
	else if (strcmp(keybEvent, "")) {
	    printf("\nFound 1 keyboard to configure on:\n/dev/input/%s\n\n", keybEvent);
      	    strcat(defaultEvent, keybEvent);
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
	    	printf("Starting UInput-Mapper with command:\n%s\n", uiMapCommand);
	    	int whileCount = 0;
	    	while (!strcmp("", getSystemOutput("cat /proc/bus/input/devices | grep -A 8 \"UInputMapper: MakeCode_Arcade\" | tr ' ' '\n' | grep event"))) {
	    	    if (whileCount > 500) {
	       	    	printf("\nTimed out trying to set up UInput-Mapper...\n");
		        printf("If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	       	    	exit(1);
	    	    }
	    	whileCount++;
	    	}
	    	char* uinputEvent = getSystemOutput("cat /proc/bus/input/devices | grep -A 8 \"UInputMapper: MakeCode_Arcade\" | tr ' ' '\n' | grep event | tr -d [:cntrl:]");
      	    	strcat(defaultEvent, uinputEvent);
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


	// Set default input event for MakeCode Arcade elf game file
    	char sedCommand[100];
      	memset (sedCommand, 0, sizeof(sedCommand));
    	snprintf(sedCommand, 100, "sed -i \"1s&.*&\"%s\"&\" /sd/arcade.cfg", defaultEvent);
    	if (system(sedCommand) == 0) {
    	    printf("Setting up %s in MakeCode Arcade game's /sd/arcade.cfg with:\n%s\n\n", uinputMapperOrKeyboard, defaultEvent);
	} else {
	    printf("Please check path or write permissions for /sd/arcade.cfg and try again.\n\n");
	    goto cleanup;
	}
    }


    // Ready terminal and game files
    int foundPxtFile = 0;
    system("stty -ixon intr undef susp undef");  //Disable pause(CTRL+S), suspend(CTRL+Z) and interrupt(CTRL+C) in terminal
    if (0 == system("head -1 /tmp/pxt-pid >>/dev/null 2>&1")) {
	system("sed -i \"1s&.*&\"\"&\" /tmp/pxt-pid");  //Clear old pid from /tmp/pxt if present
	foundPxtFile = 1;
    }
    if (strcmp("", getSystemOutput("ps -A | grep pulse"))) {
	system("sudo killall pulseaudio");  //Kill PulseAudio if running, can sometimes halt game looking for ALSA 
    }
    char* path = "/dev/tty";
    int fd;
    fflush(stdout);

    // Fork game execution on launch, so that it is executed
    // the same way it's done in-game on reset
    if  (!fork()) {

	//Switch console to graphics mode to avoid disturbing text output in borders
	fd = open(path, O_RDWR, 0);
        if (fd < 0) {
            perror("unable to open tty");
            return 1;
        }
        if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0) {
            perror("warn: ioctl KDSETMODE failed");
        }
	close(fd);

	// Launch the game
        if (system(game) == 36608) {
	    printf("%s was exited by the user\n\n", game);
	} else {
	    printf("Please check path to and executable permissions for game_file.elf and try again.\n\n");
	}
	//Alternative way to launch game, but I need to spawn new process in fork, not replace fork process
        //execl(game, game, NULL);

    // Main thread continues
    }else {

        // Wait for fork/game to launch and get running game's process name
	struct stat pxtFileBuffer;
        char processID[20]; 
      	memset (processID, 0, sizeof(processID));
        char processCommand[100];
      	memset (processCommand, 0, sizeof(processCommand));
    	int whileCount = 0;
	int maxCount = 500;
	do {
	    sleep(0.1);
	    whileCount++;
            snprintf(processCommand, 100, "head -1 /proc/%s/comm >>/dev/null 2>&1", processID);
    	    if (whileCount > maxCount) {
       	    	printf("\nTimed out trying to find game's process ID...\n");
	        printf("If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
       	    	goto cleanup;
    	    }
	    // Check until found if /tmp/pxt-pid file exists without opening it, time out otherwise
            if (0 == foundPxtFile) {
        	if (0 != stat("/tmp/pxt-pid", &pxtFileBuffer)) {
		    if (whileCount > maxCount -1) {
			printf("\nDid not find the file /tmp/pxt-pid...\n");
		    }
		    continue;
		} else {
		    foundPxtFile = 1;
		    whileCount = 0;
		}
	    }
	    // Set new game's processID from file /tmp/pxt-pid
	    snprintf(processID, 20, "%s", getSystemOutput("head -1 /tmp/pxt-pid | tr -d [:cntrl:]"));
	}
	// Check whether processID from /tmp/pxt-pid is running/registered on system, time out otherwise
        while (0 != system(processCommand));

	// Get name of game process
        snprintf(processCommand, 100, "head -1 /proc/%s/comm 2>&1 | tr -d [:cntrl:]", processID);
        char processName[50];
	memset (processName, 0, sizeof(processName));
	snprintf(processName, 50, "%s", getSystemOutput(processCommand));
	printf("\nFound running game process \"%s\" on PID=%s\n\n", processName, processID);


        // Check every 2 secounds if a game process is still active
        // Busy waiting to continue and clean up when game is exited
        // Why does this not work without the printf?!?
	char processCheckCmd[100];
	memset (processCheckCmd, 0, sizeof(processCheckCmd));
	snprintf(processCheckCmd, 100, "pgrep -n %s 2>&1 | tr -d [:cntrl:]", processName);
	char newProcessID[20];
	memset (newProcessID, 0, sizeof(newProcessID));
besure:
	while (strcmp(processID, "")) {
            //printf("%s@PID=%s is keeping launCharc alive...\n", processName, processID);
            sleep(2);
	    snprintf(newProcessID, 20, getSystemOutput(processCheckCmd));
	    if (strcmp(processID, newProcessID)) {
		if (strcmp(newProcessID, "")) {
		    printf("\"%s\" restarted on PID=%s\n", processName, newProcessID);
		} else {
		    printf("PID not found for \"%s\", trying again...\n", processName);
		}
		snprintf(processID, 20, "%s", newProcessID);
	    }
        }

        // Doublecheck that the game really has exited
        sleep(3);
	snprintf(processID, 20, getSystemOutput(processCheckCmd));
        if (strcmp(processID, "")) {goto besure;}
	else {printf("PID still not found for \"%s\", game exited?\nTerminating launCharc...\n", processName);}


        // Kill any remaining/orphaned game processes before exit
        char killAllCmd[100];
      	memset (killAllCmd, 0, sizeof(killAllCmd));
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

        system("stty sane");
        //system("clear");
    }

    return 0;
}
