uinput-mapper
=============

uinput-mapper maps input devices to new virtual input devices, using (as implied
by the name /dev/uinput[1]).

Configuration is done in the C language (preprocessor mostly) to keep things
simple and clean.

Comments, feedback, success stories, bugs and rants all welcome.


[1] Or /dev/input/uinput depending on the OS.

Building
========

Simply:

    make

to build the program. But you probably want to configure it first.

Configuration
=============

Configuration is done in the C preprocessor (and just C), recompile if you make
changes. Symlink "config.h" to an configuration file in confs/ (or just create
it). The example config confs/techinc-arcade.h is documented and should
(hopefully) be clear enough.

Mind you that invalid configurations can lead to compile-time and run-time
errors.

Bugs
====

None that I am aware of, but if you find some please drop me a mail at
http://wizzup.org.
