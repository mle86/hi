#!/usr/bin/make -f

BIN=hi
DEST=/usr/local/bin/hi
CHOWN=root:root

OBJS =hi.o
OBJS+=aux.o
OBJS+=scan.o
OBJS+=lib/malloc.o

DEP=*.h lib/*.h

CC=gcc
LD=gcc
CFLAGS =-O3 -Wall -I. -Ilib
CFLAGS+=-DPROGNAME='"$(BIN)"'

.PHONY : all install complete clean


all: $(BIN)

$(BIN): $(OBJS)
# Default goal:
# Link all objects to final binary.
# All needed objects are prerequisites here,
# so they will be compiled.
	$(LD) -o $(BIN) $(CFLAGS) $(LDFLAGS) $(OBJS)

%.o: %.c $(DEP)
# This rule compiles one source file into one object file.
# All existing header files are prerequisites.
# $* is the stem (everything which matched %, including path),
# $< is the first prerequisite (here: the needed source file!).
	$(CC) -c -o ${*}.o $(CFLAGS) $<

install:
	strip $(BIN)
	cp $(BIN) $(DEST)
	chown $(CHOWN) $(DEST)

complete: clean $(BIN) install

clean:
	rm -f $(BIN) *.o a.out *~
	rm -f lib/*.o lib/a.out lib/*~
