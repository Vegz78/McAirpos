# McAirpos
[MakeCode Arcade](https://arcade.makecode.com?nolocalhub=1&hw=rpi&compile=rawELF) games in [RetroPie](https://retropie.org.uk), running natively as ELF executables on Raspberry Pi OS with gamepad and keyboard support for up to 2 players.<br>
![](docs/MakeCode_Arcade_RetroPie.gif)

Very happy to finally present a working solution for running MakeCode Arcade games natively in RetroPie/Raspberry Pi OS!<br>

Many thanks to [@willmtemple](https://github.com/willmtemple), [@mmoskal](https://github.com/mmoskal) and [@pelikhan](https://github.com/pelikhan) for all their help and support, and [@hobbitalastair](https://github.com/hobbitalastair) and [@MerlijnWajer](https://github.com/MerlijnWajer) whose projects, [termfix](https://github.com/hobbitalastair/termfix) and [uinput-mapper](https://github.com/MerlijnWajer/uinput-mapper), I have used in my solution!!!<br>

**2021.01.08:** Reveived 2 new gamepads today, and immediately discovered that the button layouts varied a lot more than I initially thought with only my PS3 controllers at hand. I'll try my best as soon as possible to hopefully improve gamepad support somewhat beyond the PS3 controller and maybe a few unknown others with similar layouts as the PS3(if they exist). Sorry about this!

**2021.01.02:** Having tested on a fresh/clean Raspbian Lite image today, some bugs where found and fixed, which hopefully will make McAirpos run with more certain discovery of keyboard devices and mapping of gamepads, and less chances of clutter/text inside the game screen borders on many systems. It now runs pretty stably with PS3 gamepads on my RPI3s and RPi4, and I will continue to test gamepad compatibility when I receive some new ones by mail shortly. 

Feedback is valueable for fixing bugs, so please don't hesitate to [open an issue here](https://github.com/Vegz78/McAirpos/issues) if something doesn't work for you.

## Make your own or dowload MakeCode Arcade games for RetroPie
I've made a solution for making [MakeCode Arcade](https://arcade.makecode.com?nolocalhub=1&hw=rpi&compile=rawELF) ready for downloading native executables of games here:<br>
https://vegz78.github.io/McAirpos

Either make your own games in the MakeCode web editor, or download from various places, like:

- [MakeCode Arcade home (https://arcade.makecode.com)](https://arcade.makecode.com?nolocalhub=1&hw=rpi&compile=rawELF)
- [MakCode's official forum](https://forum.makecode.com/c/Share-your-Arcade-projects-here/show-tell/)
- [itch.io](https://itch.io) (search for MakeCode Arcade game jams)
- [Here on GitHub](https://github.com/search?q=makecode+arcade+game&type=Repositories)
- Other suggestions where to find good games?

## Contents
- [Tool for setting the MakeCode Arcade editor in native executable mode for downloading executables](https://vegz78.github.io/McAirpos)
- A game launcher, _launCharc_, for:
  - keeping the game in the foreground in RetroPie, 
  - cleaning up the terminal on exit, 
  - automatically detecting and setting up gamepad and keyboard controls
- [uinput-mapper](https://github.com/MerlijnWajer/uinput-mapper) to map gamepad inputs to the keyboard 

## Features
- Launch and exit natively executable MakeCode Arcade games gracefully from RetroPie and the Raspberry Pi OS console
- Autodetect gamepad and keyboard controls for up to 2 players
- [Easily download and ready the editor for natively executable game files](https://vegz78.github.io/McAirpos)

## Why the need for McAirpos?
The current MakeCode Arcade natively executable game files compiled with the Raspberry Pi as target have 2 problems preventing a nice experience from within RetroPie:
- The parent game process ID shifts during game play, which makes RetroPie think the game has exited and then puts it still running in the background. On exit, the game files do not release and clean up the framebuffer and key inputs, giving the impression that the system has frozen/halted.<br>
https://github.com/microsoft/pxt-arcade/issues/2435
- The game files only supports 1 game controller and 1 input event, but multiple game players.<br>
https://github.com/microsoft/pxt-arcade/issues/2245

McAirpos works around both these issues, making MakeCode Arcade games play like other games in RetroPie.

## Prerequisites
- Git (```sudo apt install git```)
- Drivers for gamepads you have that don't work "out of the box"

## Installation
1. Clone this repo from and into _/home/pi/_:<br>
```git clone https://github.com/Vegz78/McAirpos.git```<br>
_(Paths are currently hardcoded and will only work with the repo directly under /home/pi/)_
2. Copy the folder _.../MakeCode/sd_ with all its contents into the root folder _/_:<br>
```sudo cp -r ~/McAirpos/McAirpos/MakeCode/sd /```<br>
_(The game files are hardcoded to write to and read from this root/sd folder)_
3. Make sure the contents of _/sd_ are owned and writable for your user(normally _pi_):<br>
From _/_: ```sudo chown -R pi /sd&&sudo chgrp -R pi /sd&&sudo chmod -R 755 /sd```
4. Copy the _contents_ of the file _./McAirpos/EmulationStation/es_systems.cfg_MakeCode_ into the end of the file _/etc/emulationstation/es_systems.cfg_. Make sure not to delete the _\</systemList\>_ tag at the end of the file.
5. Copy the folder _./McAirpos/EmulationStation/makecode_ with all its contens into _/etc/emulationstation/themes/carbon/_.<br>
```sudo cp -r ~/McAirpos/McAirpos/EmulationStation/makecode /etc/emulationstation/themes/carbon```
6. Create a _makecode_ games folder under _/home/pi/RetroPie/roms/_ and [fill it up with native MakeCode Arcade games](https://vegz78.github.io/McAirpos).<br>
Make sure that the _makecode_ games folder is owned by _pi_ and has the correct permissions. From inside this folder:<br>
```sudo chown -R pi .&&sudo chgrp -R pi .&&sudo chmod -R 755 .```<br>
Also, make sure that every _game\_file.elf_ has executable permissions if they aren't set automatically by [_umask_](https://en.wikipedia.org/wiki/Umask) when copying them to the _makecode_ games folder:<br>
```sudo chmod +x ~/RetroPie/roms/makecode/game_file.elf```

## Usage
- Fire up EmulationStation and navigate to your MakeCode Arcade games menu and select the game to play, or
- Run from the text console(games do not run within X):<br>
```~/McAirpos/McAirpos/launCharc/launCharc ~/RetroPie/roms/makecode/_gamefile.elf_```

**NB: The _game\_files.elf_ only work from the [Linux console/CLI](https://en.wikipedia.org/wiki/Linux_console) and NOT with either launCharc or RetroPie started from a desktop/gui/X/terminal emulator environment.**

If something goes wrong and the screen/keyboard freezes inside the game, it should be possible to regain control of the console/RetroPie by a combination of _CTRL+C, CTRL+D and CTRL+C_.

## Default layout for controls
|Move|Keyb pl1|Keyb pl2|Gamepads|
|----|--------|--------|--------|
|Up  |W       |Up arrow|DPAD_UP|
|Down|S       |Down arrow|DPAD_DOWN|
|Left|A       |Left arrow|DPAD_LEFT|
|Right|D      |Right arrow|DPAD_RIGHT|
|Fire|Left Ctrl|Right ALT|BTN_SOUTH/A|
|Fire2|Left Shift|Space bar|BTN_NORTH/X|
|Exit|Esc||BTN_START|
|Restart|F1||BTN_SELECT|
|Menu|F2||BTN_MODE/PS|

Should gamepad Fire2 button be changed to BTN_EAST/B for compatibility?

When changing button layouts, edit _/sd/arcade.cfg_ for keyboard and edit _arcade1.py_ and _arcade2.py_ under _~/McAirpos/McAirpos/uinput-mapper/configs/_ for gamepads. When using gamepads, always remember to edit the corresponding gamepad to keyboard key mappings in both the _arcade1&2.py_ files with changes made in _/sd/arcade.cfg_.

## Issues
Don't hesitate to [open an issue](https://github.com/Vegz78/McAirpos/issues) if it doesn't work as expected or you have suggestions for improvements.<br>
I'm still just learning to code and I don't mind a little [spaghetti code](https://en.wikipedia.org/wiki/Spaghetti_code) as long as it just works.. ;-)<br>

I would really appreciate feedbacks from your own experiences with McAirpos and maybe pick up some tricks of the trade while we sort out the bugs together!<br>
<br>
**Happy retro gaming and retro game making!!!**<br><br>
[@Vegz78](https://github.com/Vegz78)<br>
(Thank you for awarding McAirpos a star if you like and use it!)<br>
(If you're interested in retro gaming in general, please also check out the blog, [retrospill.ninja](https://translate.google.com/translate?sl=auto&tl=en&u=https://retrospill.ninja))<br>
![](docs/retro2png.png)
(In-game screenshots provided by [retro2png](https://github.com/Vegz78/retro2png))
