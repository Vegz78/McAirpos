.PHONY: default clean

PREFIX=/usr/local
CFLAGS+=-ansi -pedantic -pipe -D_BSD_SOURCE \
	-Wall -Wextra -Werror -Wno-unused-result
OPT=-O2

debug: OPT=-O0
debug: CFLAGS+=-ggdb
debug: map

def_keys.h:
	echo '#include <linux/input.h>' | gcc -E -dM - \
	| grep '#define KEY_' | cut -f2 -d" " | sed 's/KEY_.*/DEF_KEY(&)/' \
	> $@

def_buttons.h:
	echo '#include <linux/input.h>' | gcc -E -dM - | grep '#define BTN_' | cut -f2 -d" " | sed 's/BTN_.*/DEF_BTN(&)/' > def_buttons.h

map: map.c def_keys.h def_buttons.h config.h config_functions.h
	$(CC) $(CFLAGS) ${OPT} map.c -o $@

all: map
	mv config.h config.bak
	for file in confs/*.h; \
	do \
	echo $$file; \
	cp $$file config.h; \
	$(CC) map.c $(CFLAGS) -o map$$(echo $$file | sed 's/confs\//_/;s/\.h//' ); \
	done
	mv config.bak config.h

clean:
	rm -f map def_keys.h map_*

install:
	install map ${PREFIX}/sbin/uinput-mapper
