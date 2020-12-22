# McAirpos
MakeCode Arcade games in RetroPie, running natively as ELF executables on Raspberry Pi OS with gamepad support.

**This repo is under construction.**
I have found a decently working work-around, but I am still figuring out the details...

In the meantime, you can try yourself to download some natively executable MakeCode Arcade game files for RetroPie/Raspberry Pi OS: https://vegz78.github.io/McAirpos<br><br>
**Warning:** The executables do not clean up the framebuffer and console input mode on exit without changes to the [MakeCode Arcade source code](https://github.com/microsoft/pxt-arcade) or without using my work-around(or other work-arounds I don't and would like to know about), and have to be killed with the keyboard combination _(CTRL+C), (CTRL+D), (CTRL+C) to return to the text console. 
