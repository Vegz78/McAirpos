.PHONY: default clean


default: uinputmapper/uinput_gen.py

clean:
	rm uinputmapper/uinput_gen.py -f

uinputmapper/uinput_gen.py:
	gcc -E -dM /usr/include/linux/input.h | egrep ' (EV|SYN|KEY|BTN|REL|ABS|MSC|LED|SND|REP|SW)_[A-Za-z0-9_]+' | ( echo "#include <linux/input.h>" ; echo "input_constants_dict = {" ; sed -r 's/[^ ]+ +([^ ]+).*/"\1" : \1,/' ; echo "}" ) | gcc -E -o /dev/stdout - | grep 'input_constants_dict = {' -A 100000 > uinputmapper/uinput_gen.py
