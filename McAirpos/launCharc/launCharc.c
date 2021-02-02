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


// Global variables
FILE *fl;


// Function which returns results from shell commands run through system()
char* getSystemOutput(char* inputCommand) {

    FILE *fp;
    static char systemOutput[100];
    memset (systemOutput, 0, sizeof(systemOutput));
    fp = popen(inputCommand, "r");
    if (fp == NULL) {
        fprintf(fl, "Failed to run command\n");
        exit(1);
    }

    while (fgets(systemOutput, sizeof(systemOutput), fp) != NULL) {
    }
    pclose(fp);

    return systemOutput; 
}


// Main function
int main(int argc, char** argv) {


    // Variables for whole main function scope
    char* path = "/dev/tty";
    int fd;


    // Read game file argument to execute
    char game[200];
    memset (game, 0, sizeof(game));
    char options[10];
    memset (options, 0, sizeof(options));
    if (argc == 2) {
	strcat(game, argv[1]);
    } else if (argc == 3) {
	strcat(game, argv[2]);
	strcat(options, argv[1]);
    } else if ((argc > 3) || (argc < 2)) {
        printf("usage: launCharc [nomap / keybswap / verbose] [/path/to/arcadegame.elf]\n");
        return 1;
    }


    // Check if run on Recalbox
    system("clear");
    int isRecalbox = 0;
    if (!strcmp("RECALBOX", getSystemOutput("uname -a | tr ' ' '\\n' | grep RECALBOX | tr -d [:cntrl:]"))) {
	isRecalbox = 1;
	// Copy game_file.elf to /tmp(.../roms folder mount exFAT and cannot execute files)
	char copyCmd[300];
	memset (copyCmd, 0, sizeof(copyCmd));
	snprintf(copyCmd, 300, "cp %s /tmp/arcade.elf&&chmod +x /tmp/arcade.elf", game);
	system(copyCmd);
	system("/usr/bin/fbv2 /home/pi/McAirpos/McAirpos/MakeCode/MakeCode_Arcade.png >>/dev/null 2>&1");
    }


    // Check for verbose option
    if (!strcmp("verbose", options)) {
	fl = stdout;
    }else {
	fl = fopen("/tmp/McAirpos.log", "w+");
	// Switch console to graphics mode to avoid disturbing text output in borders
	if (strcmp("not a tty", getSystemOutput("tty | tr -d [:cntrl:]"))) {
	    fd = open(path, O_RDWR, 0);
            if (fd < 0) {
            	perror("unable to open tty");
            	return 1;
            }
            if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0) {
            	perror("warn: ioctl KDSETMODE failed");
            }
	    close(fd);
	}
    }


    // Check for nomap option
    if (!strcmp(options, "nomap")) {
	fprintf(fl, "%s argument detected,\nlaunCharc starting %s with no automatic gamepad mappings...\n", options, game);
	sleep(1);
    } else {
	// Determine the number of connected gamepads
	fprintf(fl, "launCharc starting %s with automatic gamepad mappings...\n", game);
	char eventPaths[100];
	memset (eventPaths, 0, sizeof(eventPaths));
	int numberOfPads = 0;
	int numberOfEvents = 1 + atoi(getSystemOutput("ls /dev/input | sed 's/event//' | sort -n | tail -1 | tr -d [:cntrl:]"));
       	fprintf(fl, "\nHighest found input event number: %d\n", numberOfEvents);
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
	    		fprintf(fl, "Gamepad search hits:\n");
		    }
         	    fprintf(fl, "%s, Output:%s", padCommand, getSystemOutput(padCommand));
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
    	    strcat(keybCommand, "cat /proc/bus/input/devices | grep -B 5 -A 5 Handlers=sysrq | grep -B 7 -A 3 -e EV=12001 -e EV=10001 | grep -B 2 -A 8 -E 'Phys=(usb\\S+\\/input1:1|usb\\S+\\/input0|[a-zA-Z0-9]{2}(:[a-zA-Z0-9]{2}){5}.*)' | tr ' ' '\\n' | grep event | tail -1 | tr -d [:cntrl:]");
	} else {
    	    strcat(keybCommand, "cat /proc/bus/input/devices | grep -B 5 -A 5 Handlers=sysrq | grep -B 7 -A 3 -e EV=12001 -e EV=10001 | grep -B 2 -A 8 -E 'Phys=(usb\\S+\\/input1:1|usb\\S+\\/input0|[a-zA-Z0-9]{2}(:[a-zA-Z0-9]{2}){5}.*)' | tr ' ' '\\n' | grep event | head -1 | tr -d [:cntrl:]");
	}
    	char* keybEvent = getSystemOutput(keybCommand);
	if (strcmp(keybEvent, "")) {
	    fprintf(fl, "Keyboard search hit:\n");
       	    fprintf(fl, "%s, Output:%s", keybCommand, getSystemOutput(keybCommand));
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
	fprintf(fl, "\n");
	if ((numberOfPads == 0) && !strcmp(keybEvent, "")) {
	    fprintf(fl, "\nFound no gamepads or keyboards to configure...\n");
	    fprintf(fl, "\nPlease try the \"nomap\" option and configure /sd/arcade.cfg manually. If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	    sleep(1);
	    goto cleanup;
	}
	else if (numberOfPads == 2) {
	    fprintf(fl, "\nFound %d gamepads to configure on:\n%s, and\n%s\n\n", numberOfPads, padEvent[0], padEvent[1]);
	    sprintf(stringNumberOfPads, "%d.py &)",numberOfPads);
	}
	else if ((numberOfPads == 1) && strcmp(keybEvent, "")) {
	    fprintf(fl, "\nFound %d gamepad to configure on:\n%s\n", numberOfPads, padEvent[0]);
	    fprintf(fl, "\nFound 1 keyboard to configure on:\n/dev/input/%s\n\n", keybEvent);
            strcat(strcat(eventPaths, "/dev/input/"), keybEvent);
            strcat(eventPaths, " ");
	    sprintf(stringNumberOfPads, "%d.py &)",numberOfPads);
	}
	else if (strcmp(keybEvent, "")) {
	    fprintf(fl, "\nFound 1 keyboard to configure on:\n/dev/input/%s\n\n", keybEvent);
      	    strcat(defaultEvent, keybEvent);
	    strcat(uinputMapperOrKeyboard, "keyboard");
	}
	else if (numberOfPads == 1) {
	    fprintf(fl, "\nFound %d gamepad to configure on:\n%s\n\n", numberOfPads, padEvent[0]);
	    sprintf(stringNumberOfPads, "%d.py &)",numberOfPads);
	}
	else {
	    fprintf(fl, "\nSomething went wrong, exiting...\n");
	    fprintf(fl, "If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	    sleep(1);
	    goto cleanup;
	}


	// Launching uinput-mapper
	if (numberOfPads > 0) {
	    if (isRecalbox == 1) {
            	strcat(strcat(strcat(strcat(uiMapCommand, "(/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -C -D "), eventPaths), "| /home/pi/McAirpos/McAirpos/uinput-mapper/input-create -C -S /home/pi/McAirpos/McAirpos/uinput-mapper/configs/arcade"), stringNumberOfPads);
	    } else {
            	strcat(strcat(strcat(strcat(uiMapCommand, "(/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -C -D "), eventPaths), "| sudo /home/pi/McAirpos/McAirpos/uinput-mapper/input-create -C -S /home/pi/McAirpos/McAirpos/uinput-mapper/configs/arcade"), stringNumberOfPads);
	    }
      	    if (system(uiMapCommand) == 0) {
	    	fprintf(fl, "Starting UInput-Mapper with command:\n%s\n", uiMapCommand);
	    	int whileCount = 0;
	    	while (!strcmp("", getSystemOutput("cat /proc/bus/input/devices | grep -A 8 \"UInputMapper: MakeCode_Arcade\" | tr ' ' '\n' | grep event"))) {
	    	    if (whileCount > 500) {
	       	    	fprintf(fl, "\nTimed out trying to set up UInput-Mapper...\n");
		        fprintf(fl, "If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	       	    	goto cleanup;
	    	    }
	    	whileCount++;
	    	}
	    	char* uinputEvent = getSystemOutput("cat /proc/bus/input/devices | grep -A 8 \"UInputMapper: MakeCode_Arcade\" | tr ' ' '\n' | grep event | tr -d [:cntrl:]");
      	    	strcat(defaultEvent, uinputEvent);
		strcat(uinputMapperOrKeyboard, "UInputMapper");
	    } else {
	    	fprintf(fl, "\nUInput-Mapper failed to start...\n");
	        fprintf(fl, "If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
	    	goto cleanup;
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
	if (isRecalbox == 1) {
	    system("mount -o remount,rw /");
	}
    	if (system(sedCommand) == 0) {
    	    fprintf(fl, "Setting up %s in MakeCode Arcade game's /sd/arcade.cfg with:\n%s\n\n", uinputMapperOrKeyboard, defaultEvent);
	} else {
	    fprintf(fl, "Please check path or write permissions for /sd/arcade.cfg and try again.\n\n");
	    if (isRecalbox == 1) {
	   	system("mount -o remount,ro /");
	    }
	    goto cleanup;
	}
	if (isRecalbox == 1) {
	    system("mount -o remount,ro /");
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
	if (isRecalbox == 1) {
	    system("killall pulseaudio >>/dev/null 2>&1");  //Kill PulseAudio if running, can sometimes halt game looking for ALSA 
	} else {
	    system("sudo killall pulseaudio >>/dev/null 2>&1");  //Kill PulseAudio if running, can sometimes halt game looking for ALSA 
	}
    }
    fflush(stdout);


    // Fork game execution on launch, so that it is executed
    // the same way it's done in-game on reset
    if  (!fork()) {

	// Switch console to graphics mode to avoid disturbing text output in borders
	if (strcmp("not a tty", getSystemOutput("tty | tr -d [:cntrl:]"))) {
	    fd = open(path, O_RDWR, 0);
            if (fd < 0) {
            	perror("unable to open tty");
            	return 1;
            }
            if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0) {
            	perror("warn: ioctl KDSETMODE failed");
            }
	    close(fd);
	}

	// Run copy of game to circumvent Recalbox' read-only file system
	if (isRecalbox == 1) {
	    memset (game, 0, sizeof(game));
	    strcat(game, "/tmp/arcade.elf");
	}

	// Silence the game launch
	char gameString[200];
	memcpy(gameString, game, strlen(game)+1);
	if (strcmp("verbose", options)) {
	    fclose(fl);
	    strcat(game, " >>/tmp/McAirpos.log 2>&1");
	}

	// Launch the game
	int launchInt = system(game);
        if ((launchInt == 36608) || (launchInt == 15)) {
	    fprintf(fl, "%s was exited by the user or reset in-game\n\n", gameString);
	} else {
	    fprintf(fl, "Please check path to and executable permissions for game_file.elf and try again.\n\n");
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
       	    	fprintf(fl, "\nTimed out trying to find game's process ID...\n");
	        fprintf(fl, "If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
       	    	goto cleanup;
    	    }
	    // Check until found if /tmp/pxt-pid file exists without opening it, time out otherwise
            if (0 == foundPxtFile) {
        	if (0 != stat("/tmp/pxt-pid", &pxtFileBuffer)) {
		    if (whileCount > maxCount -1) {
			fprintf(fl, "\nDid not find the file /tmp/pxt-pid...\n");
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
	fprintf(fl, "\nFound running game process \"%s\" on PID=%s\n\n", processName, processID);


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
            //fprintf(fl, "%s@PID=%s is keeping launCharc alive...\n", processName, processID);
            sleep(2);
	    snprintf(newProcessID, 20, getSystemOutput(processCheckCmd));
	    if (strcmp(processID, newProcessID)) {
		if (strcmp(newProcessID, "")) {
		    fprintf(fl, "\"%s\" restarted on PID=%s\n", processName, newProcessID);
		} else {
		    fprintf(fl, "PID not found for \"%s\", trying again...\n", processName);
		}
		snprintf(processID, 20, "%s", newProcessID);
	    }
        }

        // Doublecheck that the game really has exited
        sleep(3);
	snprintf(processID, 20, getSystemOutput(processCheckCmd));
        if (strcmp(processID, "")) {goto besure;}
	else {fprintf(fl, "PID still not found for \"%s\", game exited?\nTerminating launCharc...\n", processName);}


        // Kill any remaining/orphaned game processes before exit
        char killAllCmd[100];
      	memset (killAllCmd, 0, sizeof(killAllCmd));
	if (strcmp("verbose", options)) {
	    snprintf(killAllCmd, 100, "killall -q %s >>/tmp/McAirpos.log 2>&1", processName);
	}else{
	    snprintf(killAllCmd, 100, "killall %s 2>&1", processName);
	}
	system(killAllCmd);
cleanup:
	if (strcmp("verbose", options)) {
	    if (isRecalbox == 1) {
            	system("killall -q input-create >>/tmp/McAirpos.log 2>&1 && killall -q input-read >>/tmp/McAirpos.log 2>&1");
	    }else {
            	system("sudo killall -q input-create >>/tmp/McAirpos.log 2>&1 && sudo killall -q input-read >>/tmp/McAirpos.log 2>&1");
	    }
	}else {
	    if (isRecalbox == 1) {
            	system("killall input-create 2>&1 && killall input-read 2>&1");
	    }else {
            	system("sudo killall input-create 2>&1 && sudo killall input-read 2>&1");
	    }
	}


        // The following code is borrowed from https://github.com/hobbitalistair/termfix:
        // Terminal Fixer's cleanup part, which returns control of the
        // framebuffer and input mode to calling process after game's exit
	if (strcmp("not a tty", getSystemOutput("tty | tr -d [:cntrl:]"))) {
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
	}else if (isRecalbox == 1) {
	    system("/usr/bin/fbv2 /recalbox/system/resources/splash/logo-version.png >>/dev/null 2>&1");
	}
        system("stty sane");
	fclose(fl);
	if (strcmp("verbose", options)) {
            system("clear");
	}
    }

    return 0;
}
