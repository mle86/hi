#!/usr/bin/make -f

BIN=hi
DEST=/usr/local/bin/hi
CHOWN=root:root

OBJS =src/main.o src/color.o src/keyword.o src/malloc.o src/nstr.o src/range.o
DEPS =src/*.h

CC=gcc
LD=gcc
CFLAGS =-O3 -std=c99 -Wall -Wextra -pedantic
CFLAGS+=-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -fPIE
CFLAGS+=-DPROGNAME="\"$(BIN)\"" -D_POSIX_C_SOURCE=200809L

.PHONY : all install clean test


all: $(BIN)

$(BIN): $(OBJS)
	$(LD) -o $(BIN) $(CFLAGS) $(LDFLAGS) $(OBJS)

%.o: %.c $(DEPS)
	$(CC) -c -o ${*}.o $(CFLAGS) $<

install: $(BIN)
	strip $(BIN)
	cp $(BIN) $(DEST)
	chown $(CHOWN) $(DEST)
	
	mkdir -p /usr/local/share/man/man1/
	cp man/hi.1 /usr/local/share/man/man1/
	chmod 0644 /usr/local/share/man/man1/hi.1
	gzip -f /usr/local/share/man/man1/hi.1

test:
	git submodule update --init test/framework/
	test/run-all-tests.sh

clean:
	rm -f $(BIN) *.o a.out *~ src/*.o src/a.out src/*~

