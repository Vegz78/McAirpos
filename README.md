# McAirpos
MakeCode Arcade games in RetroPie, running natively as ELF executables on Raspberry Pi OS with gamepad support.
![](docs/MakeCode_Arcade_RetroPie.gif)

Very happy to finally present a working solution for running MakeCode Arcade games natively in RetroPie/Raspberry Pi OS!
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
2. Copy the repo folder _./McAirpos/MakeCode/sd_ with all its contents into root the folder _/_
3. Make sure the contents of _/sd_ are owned and writable for your user(normally _pi_):
From _/sd/_: ```chown pi *&&chgrp pi *&&chmod 644 *```
4. 
```curl -sL https://raw.githubusercontent.com/Vegz78/retro2png/master/installer.sh | bash -```
