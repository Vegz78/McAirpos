# McAirpos
[MakeCode Arcade](https://arcade.makecode.com?nolocalhub=1&hw=rpi&compile=rawELF) games in [RetroPie](https://retropie.org.uk), running natively as ELF executables on Raspberry Pi OS with gamepad and keyboard support for up to 2 players.<br>
![](docs/MakeCode_Arcade_RetroPie.gif)

Very happy to finally present a working solution for running MakeCode Arcade games natively in RetroPie/Raspberry Pi OS!<br>

Many thanks to [@willmtemple](https://github.com/willmtemple), [@mmoskal](https://github.com/mmoskal) and [@pelikhan](https://github.com/pelikhan) for all their help and support, and [@hobbitalastair](https://github.com/hobbitalastair) and [@MerlijnWajer](https://github.com/MerlijnWajer) whose projects, [termfix](https://github.com/hobbitalastair/termfix) and [uinput-mapper](https://github.com/MerlijnWajer/uinput-mapper), I have used in my solution!!!<br>

[**2021.01.24:**](https://github.com/Vegz78/McAirpos/commit/c1350073bc3ba21be16c098a44f46469a3658fd4) Stability improvements for game launch and exit, more accurate keyboard autodiscovery, defaults to lowest keyboard input event handler, new ```keybswap``` command line option to swap to highest.<br>
[**2021.01.18:**](https://github.com/Vegz78/McAirpos/commit/932740f491cf1afede31578a6077f6ab5c2994f8) Bugfixes, improvements and further testing, most notably fixed the occasional mismatch of /dev/input/eventX between uinput-mapper and launCharc in /sd/arcade.cfg, and now possible to map 1 & 2 controllers without keyboard connected. Feedback about additional working controllers and about possible reasons for occasional instability/game exit, especially on older RPi devices and RPi OS/RetroPie distros, is still appreciated.<br>
[**2021.01.15:**](https://github.com/Vegz78/McAirpos/commit/fd8ac34ed5891ed8fa860b41d586f8fdc976a1aa) Added _nomap_ option for manual configuration and use of _/sd/arcade.cfg_ directly for controllers, instead of auto-detection and uinput-mapper. With some great help from [@Kay-Wolfe](https://github.com/Kay-Wolfe), gamepad support is now expanded and testet ok with various combinations of:
- [PS3 controller](https://www.sony.com/sna/EN/graphics/info/pairps-psbutton.gif),
- [XBox One controller](https://img-prod-cms-rt-microsoft-com.akamaized.net/cms/api/am/imageFileData/RE1rfiZ?ver=03f2&q=90&m=6&h=705&w=1253&b=%23FFF0F0F0&f=jpg&o=f&p=140&aim=true),
- [SteelSeries Nimbus](https://www.techradar.com/reviews/steelseries-nimbus)(remember to set _invertUp_ to 1 in the _arcadeX.py_ files to invert the Y-axis),
- [retro-bit controller](https://translate.google.com/translate?sl=no&tl=en&u=https://cdon.no/spill/retro-bit-nes-usb-controller-41757027)
- [8BitDo N30 Pro 2 controller](https://translate.google.com/translate?sl=no&tl=en&u=https://retrospill.ninja/2020/08/8bitdo-n30-pro-2/), and
- [Hori Pokken Tournament DX Pro](https://translate.google.com/translate?sl=no&tl=en&u=https://retrospill.ninja/2018/11/hori-pokken-tournament-dx-pro-pad/)(Thanks for testing, [@Gauteweb](https://github.com/Gauteweb)!)

It should also work with other controllers with little or no modification of the _arcadeX.py_ files(appreciate feedback about controllers that work for you, so I can expand this list of supported controllers!...). 

Feedback is valueable for fixing bugs, so please don't hesitate to [open an issue here](https://github.com/Vegz78/McAirpos/issues) if something doesn't work for you.

Please scroll down this page or click the following links for: [Downloading game files](https://vegz78.github.io/McAirpos), [Contents](https://github.com/Vegz78/McAirpos#contents), [Features](https://github.com/Vegz78/McAirpos#features), [Why McAirpos?](https://github.com/Vegz78/McAirpos#why-the-need-for-mcairpos), [Prerequisites](https://github.com/Vegz78/McAirpos#prerequisites), [Installation](https://github.com/Vegz78/McAirpos#Installation), [Updates](https://github.com/Vegz78/McAirpos#Updates), [Usage](https://github.com/Vegz78/McAirpos#Usage), [Controller layout](https://github.com/Vegz78/McAirpos#default-layout-for-controls), [Issues](https://github.com/Vegz78/McAirpos#issues), [Todo list](https://github.com/Vegz78/McAirpos#todo) and [Retro gaming](https://translate.google.com/translate?sl=auto&tl=en&u=https://retrospill.ninja).

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
- A game launcher, [_launCharc_](https://github.com/Vegz78/McAirpos/tree/master/McAirpos/launCharc), for:
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
- [ALSA sound driver?](https://forum.makecode.com/t/how-to-launch-makecode-arcade-uf2s-in-raspbian-retropie/2725/2)
- Recommended(not required):
  - Newer hardware(RPi 4)
  - Newer Raspberry Pi OS(Buster)
  - Newer RetroPie(4.7+)
  - FKMS graphics driver enabled

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

## Updating
Bugfixes, changes and additions are released sporadically as updates to the main branch, without any formal system for releases and history. News about the latest and most important updates are published with dates on the top of this page with links to further details in the commit commemts. 

Updates to included files located outside the _~/home/pi/McAirpos_ are seldomly expected, so to update, from inside this folder, simply type: ```git pull```

Alternatively, if you've made changes to any of the files in this folder, simply back up those files, and then remove the folder an clone again, from _/home/pi_: ```rm -rf ./McAirpos&&git clone https://github.com/Vegz78/McAirpos.git```

## Usage
- Fire up EmulationStation and navigate to your MakeCode Arcade games menu and select the game to play, or
- Run from the text console(games do not run within X):<br>
```~/McAirpos/McAirpos/launCharc/launCharc ~/RetroPie/roms/makecode/_gamefile.elf_```

  - ```nomap``` command line option for manual configuration of 1 keyboard(2 players) or 1 gamepad(1 player) using the _/sd/arcade.cfg_ file directly:<br>
```~/McAirpos/McAirpos/launCharc/launCharc nomap ~/RetroPie/roms/makecode/_gamefile.elf_```<br> and similarly inside _/etc/emulationstation/es_systems.cfg_.
  - ```keybswap``` command line option for swapping from lowest(default) autodiscovered keyboard input handler to highest.

**NB: The _game\_files.elf_ only work from the [Linux console/CLI](https://en.wikipedia.org/wiki/Linux_console) and NOT with either launCharc or RetroPie started from a desktop/gui/X/terminal emulator environment.**

If something goes wrong and the screen/keyboard freezes inside the game, it should be possible to regain control of the console/RetroPie by a combination of _CTRL+C, CTRL+D and CTRL+C_.

## Default layout for controls
|Move|Keyb pl1|Keyb pl2|Gamepads pl1&pl2|
|----|--------|--------|--------|
|Up  |W       |Up arrow|DPAD/Hat/Axis UP|
|Down|S       |Down arrow|DPAD/Hat/Axis DOWN|
|Left|A       |Left arrow|DPAD/Hat/Axis LEFT|
|Right|D      |Right arrow|DPAD/Hat/Axis RIGHT|
|Fire|Left Ctrl|Right ALT|BTN_SOUTH/A|
|Fire2|Left Shift|Space bar|BTN_EAST/B|
|Exit|Esc||BTN_START|
|Restart|F1||BTN_SELECT|
|Menu|F2||BTN_MODE/PS|

When changing button layouts, edit _/sd/arcade.cfg_ for keyboard(or 1 gamepad) and edit the uinput mapping files _arcade1.py_ and _arcade2.py_ under _~/McAirpos/McAirpos/uinput-mapper/configs/_ for 2 gamepads. When using gamepads, always remember to edit the corresponding gamepad to keyboard key mappings in both the _arcade1&2.py_ files with changes made in _/sd/arcade.cfg_.

## Issues
Don't hesitate to [open an issue](https://github.com/Vegz78/McAirpos/issues) if it doesn't work as expected or you have suggestions for improvements.<br>
I'm still just learning to code and I don't mind a little [spaghetti code](https://en.wikipedia.org/wiki/Spaghetti_code) as long as it just works.. ;-)<br>

I would really appreciate feedbacks from your own experiences with McAirpos and maybe pick up some tricks of the trade while we sort out the bugs together!

## Todo
Some thoughts and ideas I might start grappling with in the near or distant future. Don't hesitate to post good ideas in the issues section. If it's to big or difficult for me, maybe you want to contribute directly with a pull request?
- Follow up bugs and improve stability
- Expand gamepad support further
- More command line options;
  - Specify wanted /dev/input/eventXs manually,
  - Incorporate/allow to connect via [other mapping apps/scripts](https://github.com/Vegz78/McAirpos/issues/5#issuecomment-760950633)
  - Try to [use some of the controller configurations already set up in RetroPie](https://github.com/Vegz78/McAirpos/issues/5#issuecomment-761888132)
- Collect a library of well working controller configurations(Please share working _/sd/arcade.cfg_ and _arcade1&2.py_ files with the name and type of controllers in [the issues section](https://github.com/Vegz78/McAirpos/issues))<br>
<br>

**Happy retro gaming and retro game making!!!**<br><br>
[@Vegz78](https://github.com/Vegz78)<br>
(Thank you for awarding McAirpos a star if you like and use it!)<br>
(If you're interested in retro gaming in general, please also check out the blog, [retrospill.ninja](https://translate.google.com/translate?sl=auto&tl=en&u=https://retrospill.ninja))<br>
![](docs/retro2png.png)
(In-game screenshots provided by [retro2png](https://github.com/Vegz78/retro2png))
