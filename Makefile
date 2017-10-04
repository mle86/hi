CC=gcc
CFLAGS=-O1 -fomit-frame-pointer -I. -Wall -Ilib
BIN=hi
DEST=/usr/local/bin/hi
CHOWN=root:root
SRC=hi.c


default:
	$(CC) $(CFLAGS) -o $(BIN) -DPROGNAME='"$(BIN)"' $(SRC)

install:
	strip $(BIN)
	cp $(BIN) $(DEST)
	chown $(CHOWN) $(DEST)

complete: clean default install

clean:
	rm -f $(BIN) *.so *.o *.a *~ *.bak
