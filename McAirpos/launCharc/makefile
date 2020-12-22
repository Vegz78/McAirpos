CC = gcc
CFLAGS := -Wall -Werror -O2 -g
PREFIX := /usr
BINDIR := $(PREFIX)/bin
SBIN = termfix

all: $(SBIN)

%: %.c
	$(CC) -o $@ $< $(LIBS) $(CFLAGS)

install: $(SBIN)
	# Install SUID
	for f in $(SBIN); do install -Dm4755 $$f $(DESTDIR)$(BINDIR)/$$f; done

clean:
	rm -f $(SBIN)
