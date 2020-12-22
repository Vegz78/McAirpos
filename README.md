# termfix

Ever found yourself with a broken terminal after an SDL framebuffer app
crashes?

This small application can be run after any SDL framebuffer app to at least
leave the console in a position where you can interact with it enough to run
commands (probably at least a `reset`).


Fix a broken terminal:

`termfix [/dev/tty]`

It may require some experimentation to find the correct terminal to use.

Suggestions and improvements welcome!

