#!/usr/bin/make -f

BIN=hi
DEST=/usr/local/bin/hi
CHOWN=root:root

OBJS =src/main.o src/color.o src/keyword.o src/malloc.o src/nstr.o
DEPS =src/*.h

CC=gcc
LD=gcc
CFLAGS =-O3 -std=c99 -Wall -Wextra -pedantic -D_FORTIFY_SOURCE=1 -fPIE
CFLAGS+=-DPROGNAME="\"$(BIN)\"" -D_POSIX_C_SOURCE=200809L

.PHONY : all install clean


all: $(BIN)

$(BIN): $(OBJS)
	$(LD) -o $(BIN) $(CFLAGS) $(LDFLAGS) $(OBJS)

%.o: %.c $(DEPS)
	$(CC) -c -o ${*}.o $(CFLAGS) $<

install:
	strip $(BIN)
	cp $(BIN) $(DEST)
	chown $(CHOWN) $(DEST)

clean:
	rm -f $(BIN) *.o a.out *~ src/*.o src/a.out src/*~

