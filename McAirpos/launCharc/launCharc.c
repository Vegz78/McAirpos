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
#include <stdarg.h>
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
FILE *fl, *popen(), *pclose();
int fd;
int kdkb;
int pulseaudioStatus = 0;


// Function which returns results from shell commands run through system()
char *getSystemOutput(char *inputCommand)
{

	FILE *fp;
	static char systemOutput[100];
	memset(systemOutput, 0, sizeof(systemOutput));
	fp = popen(inputCommand, "r");
	if (fp == NULL)
	{
		fprintf(fl, "Failed to run system() command\n");
		exit(1);
	}

	while (fgets(systemOutput, sizeof(systemOutput), fp) != NULL)
	{
	}
	pclose(fp);

	return systemOutput;
}


// The code in this function is borrowed from https://github.com/hobbitalistair/termfix:
// Terminal Fixer's cleanup part, which returns control of the
// framebuffer and input mode to calling process after game's exit
int termFix(char *path, int noOfArguments, ...)
{

	// Open the tty for ioctl
	if (strcmp("not a tty", getSystemOutput("tty | tr -d [:cntrl:]")))
	{
		fd = open(path, O_RDWR, 0);
		if (fd < 0)
		{
			fprintf(fl, "Unable to open s% for ioctl...\n", path);
			return 1;
		}
		// Declaring pointer to the argument list
		va_list ptr;
		// Initializing argument to the list pointer
		va_start(ptr, noOfArguments);
		for (int i = 1; i < (noOfArguments + 1); i++)
		{
			// Accessing current variable and pointing to next one in switch
			switch (va_arg(ptr, int))
			{
			case 1:
				// Sets the Linux console to graphics mode
				if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0)
				{
					fprintf(fl, "Warn: ioctl KDSETMODE failed...\n");
					fprintf(fl, "Are you on a terminal emulator(X-Windows) instead of the Linux console?\n");
				}
				break;
			case 2:
				// Sets the Linux console back to text mode
				if (ioctl(fd, KDSETMODE, KD_TEXT) < 0)
				{
					fprintf(fl, "Warn: ioctl KDSETMODE failed...\n");
					fprintf(fl, "Are you on a terminal emulator(X-Windows) instead of the Linux console?\n");
				}
				break;
			case 3:
				// Get the current keyboard mode
				if (ioctl(fd, KDGKBMODE, &kdkb) < 0)
				{
					fprintf(fl, "Warn: ioctl KBGKBMODE failed...\n");
					fprintf(fl, "Unable to get your keyboard mode.\n");
				}
				break;
			case 4:
				// Set the current keyboard mode
				if (ioctl(fd, KDSKBMODE, (kdkb >= 0) ? kdkb : K_XLATE) < 0)
				{
					fprintf(fl, "Warn: ioctl KBSKBMODE failed...\n");
					fprintf(fl, "Unable to set your keyboard mode.\n");
				}
				break;
			case 5:
				// Unlocks switching between virtual terminals with CTRL+ALT+FX
				// This one fails without sudo, but doesn't seem needed for
				// MakeCode Arcade games(comment out or leave as and option?).
				if (ioctl(fd, VT_UNLOCKSWITCH, 1) < 0)
				{
					fprintf(fl, "Warn: ioctl VT_UNLOCKSWITCH failed...\n");
					fprintf(fl, "Are you not a super-user? (This should normally not affect game launch...)\n");
				}
				break;
			case 6:
				// Sets the Linux console VT_MODE to VT_AUTO
				if (ioctl(fd, VT_SETMODE, VT_AUTO) < 0)
				{
					fprintf(fl, "Warn: ioctl VT_SETMODE failed...\n");
					fprintf(fl, "Unable to set your console mode.\n");
				}
				break;
			default:
				fprintf(fl, "Not a valid choice for termFix()\n");
				return 1;
			}
		}
		// Ending argument list traversal
		va_end(ptr);
		close(fd);
	}

	return 0;
}


// Main function
int main(int argc, char **argv)
{

	// Variables for whole main function scope
	char *path = "/dev/tty";
	char logfile[20];
	memset(logfile, 0, sizeof(logfile));
	strcat(logfile, "/tmp/McAirpos.log");
	char sudoer[6];
	memset(sudoer, 0, sizeof(sudoer));
	strcat(sudoer, "sudo ");
	char gameString[200];
	memset(gameString, 0, sizeof(gameString));


	// Read and handle game file arguments to execute
	char game[200];
	memset(game, 0, sizeof(game));
	int nomap = 0, keybswap = 0, verbose = 0;
	struct stat gameFileBuffer;
	if ((argc < 2) || !(strstr(argv[argc - 1], ".elf") != NULL))
	{
		printf("usage: launCharc [nomap / keybswap / verbose] [/path/to/arcadegame.elf]\n");
		return 1;
	}
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp("nomap", argv[i])) nomap = 1;
		if (!strcmp("keybswap", argv[i])) keybswap = 1;
		if (!strcmp("verbose", argv[i])) verbose = 1;
		if ((argv[i] == argv[argc - 1]) && (strstr(argv[i], ".elf") != NULL)) strcat(game, argv[argc - 1]);
	}
	// Check if game file exists, exit error message otherwise
	if (0 != stat(game, &gameFileBuffer))
	{
		system("clear");
		snprintf(gameString, sizeof(gameString), "The game file %s was not found...\n\n", game);
		fprintf(stderr, gameString);
		snprintf(game, sizeof(game), "echo \"%s\" >%s 2<&1", gameString, logfile);
		system(game);
		sleep(1);
		return 1;
	}


	// Check if and cater for running on Recalbox
	// Run copy of game to circumvent Recalbox' read-only(/) and/or non-executablel(.../share/roms exFAT) file systems
	int isRecalbox = 0;
	struct stat recalboxFileBuffer;
	char copyCmd[300];
	memset(copyCmd, 0, sizeof(copyCmd));
	char basename[200];
	memset(basename, 0, sizeof(basename));
	snprintf(copyCmd, sizeof(copyCmd), "basename %s | tr -d [:cntrl:]", game);
	strcat(basename, getSystemOutput(copyCmd));
	if (0 == stat("/recalbox", &recalboxFileBuffer))
	{
		isRecalbox = 1;
		// Copy game_file.elf to new location, since default .../roms folder is a mounted exFAT and cannot execute files
		// /recalbox/share/bootvideos/makecode allows for saving game states in settings and DB extensions etc.
		memset(copyCmd, 0, sizeof(copyCmd));
		snprintf(copyCmd, sizeof(copyCmd), "rsync %s /recalbox/share/bootvideos/makecode/&&chmod +x /recalbox/share/bootvideos/makecode/%s", game, basename);
		system(copyCmd);
		// Change game's execution path accordingly
		memset(game, 0, sizeof(game));
		snprintf(game, sizeof(game), "/recalbox/share/bootvideos/makecode/%s", basename);
		// Show MakeCode Arcade splash screen on game loading
		system("/usr/bin/fbv2 -f /home/pi/McAirpos/McAirpos/MakeCode/MakeCode_Arcade.png >>/dev/null 2>&1");
		sleep(2);
	}

	// Check if and cater for running on Batocera
	int isBatocera = 0;
	if (0 == stat("/userdata/system/batocera.conf", &recalboxFileBuffer))
	{
		isBatocera = 1;
	}
	// No sudo for either Recalbox or Batocera
	if (isRecalbox || isBatocera) memset (sudoer, 0, sizeof(sudoer));

	// Make game file executable on Batocera and RetroPie, in case it isn't
	if (!isRecalbox)
	{
		memset(copyCmd, 0, sizeof(copyCmd));
		snprintf(copyCmd, sizeof(copyCmd), "%s chmod +x %s", sudoer, game);
		system(copyCmd);
	}


	// Ready game files and terminal
	// Check presence of and clear old pid from /tmp/pxt if present
	int foundPxtFile = 0;
	if (0 == system("head -1 /tmp/pxt-pid >>/dev/null 2>&1"))
	{
		system("sed -i \"1s&.*&\"\"&\" /tmp/pxt-pid");
		foundPxtFile = 1;
	}
	// Disable pause(CTRL+S), suspend(CTRL+Z), eof(CTRL+D) and interrupt(CTRL+C) in terminal
	system("stty -ixon -isig -icanon -iexten intr undef susp undef eof undef stop undef&&set -o ignoreeof");
	// Kill PulseAudio if running and kernel < 5, Pulseaudio can sometimes halt game looking for ALSA
	// if ((atoi(getSystemOutput("uname -r | grep -o -e '^[0-9]*' | tr -d [:cntrl:]")) < 5) && (strcmp("", getSystemOutput("ps -A | grep pulseaudio")))) {
	// Note: Pulseaudio used to restart automatically on kernels below 5, keep an eye on how this is handled > 5 on RPi OS/RetroPie
	// Kill PulseAudio if running, Pulseaudio can sometimes halt game looking for and getting access to ALSA
	if (strcmp("", getSystemOutput("ps -A | grep pulseaudio")))
	{
		pulseaudioStatus = 1;
		memset(copyCmd, 0, sizeof(copyCmd));
		snprintf(copyCmd, sizeof(copyCmd), "%skillall pulseaudio >>/dev/null 2>&1", sudoer);
		system(copyCmd);
	}


	// Check and cater for verbose option
	// Silence the game launch information to Linux console if verbose option is not given
	strcat(gameString, game);
	if (verbose)
	{
		system("clear");
		memset(logfile, 0, sizeof(logfile));
		strcat(logfile, getSystemOutput("tty | tr -d [:cntrl:]"));
		fl = fopen(logfile, "w");
	}
	else
	{
		// Create new or truncate existing file
		fl = fopen(logfile, "w+");
		if (fl < 0) fprintf(fl, "Unable to open %s\n", logfile);
		else fclose(fl);

		// Reopen log file in append mode to allow for writing across fork
		fl = fopen(logfile, "a");
		if (fl < 0) fprintf(fl, "Unable to open %s\n", logfile);

		// Switch console to graphics mode to avoid disturbing text output in borders
		if (!isBatocera) termFix(path, 1, 1);

		// Change game's execution path accordingly
		memset(game, 0, sizeof(game));
		snprintf(game, sizeof(game), "%s >>%s 2>&1", gameString, logfile);
	}


	// Check and cater for nomap option
	// Run with gamepad settings in /sd/arcade.cfg or find and configure gamepads automatically
	if (nomap)
	{
		fprintf(fl, "nomap argument detected,\nlaunCharc starting %s with no automatic gamepad mappings...\n", gameString);
		sleep(1);
	}
	else
	{
		// Determine the number of connected gamepads
		fprintf(fl, "launCharc starting %s with automatic gamepad mappings...\n", gameString);
		char eventPaths[100];
		memset(eventPaths, 0, sizeof(eventPaths));
		int numberOfPads = 0;
		int numberOfEvents = 1 + atoi(getSystemOutput("ls /dev/input | sed 's/event//' | sort -n | tail -1 | tr -d [:cntrl:]"));
		fprintf(fl, "\nHighest found input event number: %d\n", numberOfEvents);
		char padEvent[2][20];
		memset(padEvent, 0, sizeof(padEvent));
		for (int i = 0; i < numberOfEvents; i++)
		{
			if (numberOfPads < 2)
			{
				char padCommand[150];
				memset(padCommand, 0, sizeof(padCommand));
				snprintf(padCommand, sizeof(padCommand), "/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -vp /dev/input/event%d 2>&1 | grep -e BTN_START -e BTN_SOUTH -e BTN_PINKIE", i);
				char *event = getSystemOutput(padCommand);
				if (strcmp(event, ""))
				{
					if (numberOfPads == 0) fprintf(fl, "Gamepad search hits:\n");
					fprintf(fl, "%s, Output:%s", padCommand, getSystemOutput(padCommand));
					char iString[20];
					memset(iString, 0, sizeof(iString));
					sprintf(iString, "%d", i);
					strcat(strcat(strcat(eventPaths, "/dev/input/event"), iString), " ");
					strcat(strcat(strcat(eventPaths, "/dev/input/event"), iString), " ");
					memset(padEvent[numberOfPads], 0, sizeof(padEvent[numberOfPads]));
					strcat(strcat(padEvent[numberOfPads], "/dev/input/event"), iString);
					numberOfPads++;
				}
			}
		}


		// Determine if keyboard is connected
		char keybCommand[300];
		memset(keybCommand, 0, sizeof(keybCommand));
		// Check and cater for keybswap option
		// Chooses last keyboard found, first found otherwise
		if (keybswap) strcat(keybCommand, "cat /proc/bus/input/devices | grep -B 5 -A 5 Handlers=sysrq | grep -B 7 -A 3 -e EV=12001 -e EV=10001 | grep -B 2 -A 8 -E 'Phys=(usb\\S+\\/input1:1|usb\\S+\\/input0|[a-zA-Z0-9]{2}(:[a-zA-Z0-9]{2}){5}.*)' | tr ' ' '\\n' | grep event | tail -1 | tr -d [:cntrl:]");
		else strcat(keybCommand, "cat /proc/bus/input/devices | grep -B 5 -A 5 Handlers=sysrq | grep -B 7 -A 3 -e EV=12001 -e EV=10001 | grep -B 2 -A 8 -E 'Phys=(usb\\S+\\/input1:1|usb\\S+\\/input0|[a-zA-Z0-9]{2}(:[a-zA-Z0-9]{2}){5}.*)' | tr ' ' '\\n' | grep event | head -1 | tr -d [:cntrl:]");
		char *keybEvent = getSystemOutput(keybCommand);
		if (strcmp(keybEvent, ""))
		{
			fprintf(fl, "Keyboard search hit:\n");
			fprintf(fl, "%s, Output:%s", keybCommand, getSystemOutput(keybCommand));
		}


		// Automatically set up uinput-mapper with keyboard and/or 1 or 2 gamepads
		char stringNumberOfPads[20];
		memset(stringNumberOfPads, 0, sizeof(stringNumberOfPads));
		char uinputMapperOrKeyboard[20];
		memset(uinputMapperOrKeyboard, 0, sizeof(uinputMapperOrKeyboard));
		char defaultEvent[67];
		memset(defaultEvent, 0, sizeof(defaultEvent));
		strcat(defaultEvent, "SCAN_CODES=/dev/input/");
		fprintf(fl, "\n");
		if ((numberOfPads == 0) && !strcmp(keybEvent, ""))
		{
			fprintf(fl, "\nFound no gamepads or keyboards to configure...\n");
			fprintf(fl, "\nPlease try the \"nomap\" option and configure /sd/arcade.cfg manually. If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
			sleep(1);
			goto cleanup;
		}
		else if (numberOfPads == 2)
		{
			fprintf(fl, "\nFound %d gamepads to configure on:\n%s, and\n%s\n\n", numberOfPads, padEvent[0], padEvent[1]);
			sprintf(stringNumberOfPads, "%d.py &)", numberOfPads);
		}
		else if ((numberOfPads == 1) && strcmp(keybEvent, ""))
		{
			fprintf(fl, "\nFound %d gamepad to configure on:\n%s\n", numberOfPads, padEvent[0]);
			fprintf(fl, "\nFound 1 keyboard to configure on:\n/dev/input/%s\n\n", keybEvent);
			strcat(strcat(eventPaths, "/dev/input/"), keybEvent);
			strcat(eventPaths, " ");
			sprintf(stringNumberOfPads, "%d.py &)", numberOfPads);
		}
		else if (strcmp(keybEvent, ""))
		{
			fprintf(fl, "\nFound 1 keyboard to configure on:\n/dev/input/%s\n\n", keybEvent);
			strcat(defaultEvent, keybEvent);
			strcat(uinputMapperOrKeyboard, "keyboard");
		}
		else if (numberOfPads == 1)
		{
			fprintf(fl, "\nFound %d gamepad to configure on:\n%s\n\n", numberOfPads, padEvent[0]);
			sprintf(stringNumberOfPads, "%d.py &)", numberOfPads);
		}
		else
		{
			fprintf(fl, "\nSomething went wrong, exiting...\n");
			fprintf(fl, "If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
			sleep(1);
			goto cleanup;
		}


		// Launching uinput-mapper
		char uiMapCommand[400];
		memset(uiMapCommand, 0, sizeof(uiMapCommand));
		if (numberOfPads > 0)
		{
			snprintf(uiMapCommand, sizeof(uiMapCommand), "(/home/pi/McAirpos/McAirpos/uinput-mapper/input-read -C -D %s| %s/home/pi/McAirpos/McAirpos/uinput-mapper/input-create -C -S /home/pi/McAirpos/McAirpos/uinput-mapper/configs/arcade%s", eventPaths, sudoer, stringNumberOfPads);
			if (system(uiMapCommand) == 0)
			{
				fprintf(fl, "Starting UInput-Mapper with command:\n%s\n", uiMapCommand);
				int whileCount = 0;
				while (!strcmp("", getSystemOutput("cat /proc/bus/input/devices | grep -A 8 \"UInputMapper: MakeCode_Arcade\" | tr ' ' '\n' | grep event")))
				{
					if (whileCount > 500)
					{
						fprintf(fl, "\nTimed out trying to set up UInput-Mapper...\n");
						fprintf(fl, "If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
						goto cleanup;
					}
					whileCount++;
				}
				char *uinputEvent = getSystemOutput("cat /proc/bus/input/devices | grep -A 8 \"UInputMapper: MakeCode_Arcade\" | tr ' ' '\n' | grep event | tr -d [:cntrl:]");
				strcat(defaultEvent, uinputEvent);
				strcat(uinputMapperOrKeyboard, "UInputMapper");
			}
			else
			{
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

		// Set default input event for MakeCode Arcade elf game file in /sd/arcade.cfg
		char sedCommand[100];
		memset(sedCommand, 0, sizeof(sedCommand));
		snprintf(sedCommand, sizeof(sedCommand), "sed -i \"1s&.*&\"%s\"&\" /sd/arcade.cfg", defaultEvent);
		if (isRecalbox) system("mount -o remount,rw /");
		if (system(sedCommand) == 0)
		{
			fprintf(fl, "Setting up %s in MakeCode Arcade game's /sd/arcade.cfg with:\n%s\n\n", uinputMapperOrKeyboard, defaultEvent);
			if (isRecalbox) system("mount -o remount,ro /");
		}
		else
		{
			fprintf(fl, "Please check path or write permissions for /sd/arcade.cfg and try again.\n\n");
			goto cleanup;
		}
	}

	// Get terminal keyboard mode and flush buffers before fork
	termFix(path, 1, 3);
	fflush(stdout);
	fflush(fl);


	// Fork game execution on launch, so that it is executed
	// the same way it's done in-game on reset
	if (!fork())
	{

		// Switch console to graphics mode to avoid disturbing text output in borders and save original keyboard mode
		if (!isBatocera) termFix(path, 1, 1);

		// Close and reopen FILE fl inside fork to disconnect from pre fork state
		if (fl != NULL) fclose(fl);
		fl = fopen(logfile, "a");
		if (fl < 0) fprintf(fl, "Unable to open %s\n", logfile);

		// Launch the game
		int launchInt = system(game);
		// Check for known return codes from the MCA games, indicating that it - at least - was executed by system()
		// Due to strange zombie process behaviour, system will return values from the first and parent ended MCA game
		// process, even though spawned child processes and the game itself might actually still be running...
		if ((launchInt == 36608) || (launchInt == 15)) fprintf(fl, "\n%s was executed successfully and the parent process was exited by the user or reset in-game. The game might still be running in a child process...\n\n", gameString);
		else fprintf(fl, "\nERROR: Please first check path to and executable permissions for %s and try again.\nThere might also be other problems with the game file, the runtime or the shell availability...\n\n", gameString);

		// Alternative way to launch game, but I need to spawn new process in fork, not replace fork process
		// execl(game, game, NULL);

		// fprintf(fl, "DEBUG: \nLaunch command: %s, \nSystem() return value: %d\n\n\n", game, launchInt);

		// Flush buffers and close FILE fl before child process ends
		fflush(stdout);
		fflush(fl);
		if (fl != NULL) fclose(fl);

		// Main thread continues
	}
	else
	{

		// Wait for fork/game to launch and get running game's process name
		struct stat pxtFileBuffer;
		char processID[20];
		memset(processID, 0, sizeof(processID));
		char processCommand[100];
		memset(processCommand, 0, sizeof(processCommand));
		int whileCount = 0;
		int maxCount = 500;

		do
		{
			sleep(0); // Sleep OS arbitrary short time and yield open for other threads
			whileCount++;
			snprintf(processCommand, sizeof(processCommand), "head -1 /proc/%s/comm >>/dev/null 2>&1", processID);
			if (whileCount > maxCount)
			{
				fprintf(fl, "\nTimed out trying to find game's process ID...\n");
				fprintf(fl, "If stuck, please read or open a related issue at https://github.com/Vegz78/McAirpos.\n");
				goto cleanup;
			}
			// Check until found if /tmp/pxt-pid file exists without opening it, time out otherwise
			if (0 == foundPxtFile)
			{
				if (0 != stat("/tmp/pxt-pid", &pxtFileBuffer))
				{
					if (whileCount > (maxCount - 1)) fprintf(fl, "\nDid not find the file /tmp/pxt-pid...\n");
					continue;
				}
				else
				{
					foundPxtFile = 1;
					whileCount = 0;
				}
			}
			// Set new game's processID from file /tmp/pxt-pid
			snprintf(processID, sizeof(processID), "%s", getSystemOutput("head -1 /tmp/pxt-pid | tr -d [:cntrl:]"));
		}
		// Check whether processID from /tmp/pxt-pid is running/registered on system, time out otherwise
		while (0 != system(processCommand));

		// Get name of game process
		snprintf(processCommand, sizeof(processCommand), "head -1 /proc/%s/comm 2>&1 | tr -d [:cntrl:]", processID);
		char processName[50];
		memset(processName, 0, sizeof(processName));
		snprintf(processName, sizeof(processName), "%s", getSystemOutput(processCommand));
		fprintf(fl, "\nlaunCharc: Found running game process \"%s\" on PID=%s\n\n", processName, processID);
		fflush(fl);

		// Check every 2 secounds if a game process is still active
		// Busy waiting to continue and clean up when game is exited
		// Why does this not work without the printf?!?
		char processCheckCmd[100];
		memset(processCheckCmd, 0, sizeof(processCheckCmd));
		snprintf(processCheckCmd, sizeof(processCheckCmd), "pgrep -n %s 2>&1 | tr -d [:cntrl:]", processName);
		char newProcessID[20];
		memset(newProcessID, 0, sizeof(newProcessID));
	besure:
		while (strcmp(processID, ""))
		{
			// fprintf(fl, "%s@PID=%s is keeping launCharc alive...\n", processName, processID);
			sleep(2);
			snprintf(newProcessID, sizeof(newProcessID), getSystemOutput(processCheckCmd));
			if (strcmp(processID, newProcessID))
			{
				if (strcmp(newProcessID, "")) fprintf(fl, "\nlaunCharc: \"%s\" restarted on PID=%s\n", processName, newProcessID);
				else fprintf(fl, "\nPID not found for \"%s\", trying again...\n", processName);
				fflush(fl);
				snprintf(processID, sizeof(processID), "%s", newProcessID);
			}
		}

		// Doublecheck that the game really has exited
		sleep(3);
		snprintf(processID, sizeof(processID), getSystemOutput(processCheckCmd));
		if (strcmp(processID, "")) goto besure;
		else
		{
			fprintf(fl, "PID still not found for \"%s\", game exited?\nTerminating launCharc...\n", processName);
			fflush(fl);
		}

		// Kill any remaining/orphaned game processes before exit
		char killAllCmd[200];
		memset(killAllCmd, 0, sizeof(killAllCmd));
		if (!verbose) snprintf(killAllCmd, sizeof(killAllCmd), "killall -q %s >>%s 2>&1", processName, logfile);
		else snprintf(killAllCmd, sizeof(killAllCmd), "killall %s 2>&1", processName);
		system(killAllCmd);

	cleanup: 
		;// Kill uinput-mapper processes
		char killUInputCmd[200];
		memset(killUInputCmd, 0, sizeof(killUInputCmd));
		if (verbose) snprintf(killUInputCmd, sizeof(killUInputCmd), "%skillall input-create 2>&1 && %skillall input-read 2>&1", sudoer, sudoer);
		else snprintf(killUInputCmd, sizeof(killUInputCmd), "%skillall -q input-create >>%s 2>&1 && %skillall -q input-read >>%s 2>&1", sudoer, logfile, sudoer, logfile);
		system(killUInputCmd);

		// Flush buffers, clean up and restore terminal to original state before exit
		if (isRecalbox)
		{
			system("mount -o remount,ro /");
			// Show Recalbox loading screen while returning to EmulationStation
			system("/usr/bin/fbv2 /recalbox/system/resources/splash/logo-version.png >>/dev/null 2>&1");
			// Set terminal to text mode, keyboard to original mode and allow switching virtual terminals
			termFix(path, 3, 2, 4, 5);
		}
		else if (isBatocera)
		{
			// Set terminal to text mode, keyboard to original mode, allow VT switching and switch to VT no. 2
			termFix("/dev/tty1", 3, 2, 4, 5);
		}
		else {
			// Set terminal to text mode and keyboard to original mode
			termFix(path, 2, 2, 4);
		}
		// Restore terminal sane/default control character handling
		system("stty sane&&unset ignoreeof");
		// Restore Pulseaudio of it was present before game launch
		if (pulseaudioStatus)
		{
			memset(copyCmd, 0, sizeof(copyCmd));
			snprintf(copyCmd, sizeof(copyCmd), "%spulseaudio -D >>/dev/null 2>&1", sudoer);
			system(copyCmd);
		}
		// Final cleanup
		fflush(stdout);
		fflush(fl);
		if (fl != NULL) fclose(fl);
		if (!verbose) system("clear");
	}

	return 0;
}
