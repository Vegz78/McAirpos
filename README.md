# McAirpos
MakeCode Arcade games in RetroPie, running natively as ELF executables on Raspberry Pi OS with gamepad and keyboard control support for up to 2 players.<br>
![](docs/MakeCode_Arcade_RetroPie.gif)

Very happy to finally present a working solution for running MakeCode Arcade games natively in RetroPie/Raspberry Pi OS!<br>
Many thanks to [@willmtemple](https://github.com/willmtemple), [@mmoskal](https://github.com/mmoskal) and [@pelikhan](https://github.com/pelikhan) for all their help and support, and [@hobbitalastair](https://github.com/hobbitalastair) and [@MerlijnWajer](https://github.com/MarlijnWajer) whose projects, [termfix](https://github.com/hobbitalastair/termfix) and [uinput-mapper](https://github.com/MarlijnWajer/uinput-mapper), I have used in my solution!!!

## Make your own or dowload MakeCode Arcade games for RetroPie
I've made a solution for making [MakeCode Arcade](https://arcade.makecode.com) ready for downloading ELF executables of games here:<br>
https://vegz78.github.io/McAirpos

Either make your own games in the editor, or download from various places, like:

- [MakCode's official forum](https://forum.makecode.com/c/Share-your-Arcade-projects-here/show-tell/)
- [itch.io](https://itch.io) (search for MakeCode Arcade game jams)


## Contents
- [Tool for setting the MakeCode Arcade editor in native executable mode or downloading executables](https://vegz78.github.io/McAirpos)
- A game launcher, _launCharc_, 
  - for keeping the game in the foreground in RetroPie, 
  - for cleaning up the terminal on exit, 
  - automatically checking and setting up gampad and keyboard controls
- [uinput-mapper](https://github.com/MarlijnWajer/uinput-mapper) to map gamepad inputs to to keyboard 

## Features
- Launch and exit natively executable MakeCode Arcade games gracefully from RetroPie and the Raspberry Pi OS console
- Auto detect gamepad and keyboard controls for up to 2 players
- [Easily download and ready the editor for natively executable game files](https://vegz78.github.io/McAirpos)

## Installation
1. Clone this repo from and into _/home/pi/_: ```git clone https://github.com/Vegz78/McAirpos.git```
2. Copy the repo folder _./McAirpos/MakeCode/sd_ with all its contents into root the folder _/_.
3. Make sure the contents of _/sd_ are owned and writable for your user(normally _pi_):<br>
From _/sd/_: ```chown pi *&&chgrp pi *&&chmod 644 *```
4. Copy the _contents_ of the file _./McAirpos/EmulationStation/es_systems.cfg_MakeCode_ into the end of the file _/etc/emulationstation/es_systems.cfg_. Make sure not to delete the _</systemList>_ tag at the end of the file.
5. Copy the folder _./McAirpos/EmulationStation/makecode_ with all its contens into _/etc/emulationstation/themes/carbon/_.
6. Make a _makecode_ games folder under _/home/pi/RetroPie/roms/_ and [fill it up with native MakeCode Argade games](https://vegz78.github.io/McAirpos).

## Usage
- Fire up EmulationStation and navigate to your MakeCode Arcade games menu and select the game to play, or
- Run from the text console(games do not run within X), from the top of repo folder:
```./McAirpos/launCharc/launCharc ~/RetroPie/roms/makecode/_gamefile.elf_```

## Issues
Don't hesitate to [open an issue](https://github.com/Vegz78/McAirpos/issues) if it doesn't work as expected or you have suggestions for improvements.<br>
I'm still just learning to code and I don't mind a little [spaghetti code](https://en.wikipedia.org/wiki/Spaghetti_code) as long as it just work.. ;-)<br>
But I haven't testet on other systems than my own Raspberry Pi 4 yet, and I would really appreciate feedbacks from your own experiences with McAirpos and maybe pick up some tricks of the trade while we sort out the bugs together.<br>
<br>
**Happy retro gaming and retro game making!!!**<br><br>
[@Vegz78](https://github.com/Vegz78)<br>
(Thank you for awarding McAirpos a star if you like and use it!)<br>
(If you're interested in retro gaming in general, please also check out the blog, [retrospill.ninja](https://translate.google.com/translate?sl=auto&tl=en&u=https://retrospill.ninja))<br>
![](docs/retro2png.png)
(In-game screenshots provided by [retro2png](https://github.com/Vegz78/retro2png))
