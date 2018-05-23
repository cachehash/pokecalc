.PHONY: all clean

CFLAGS=-g -Wall
LDLIBS=-lsqlite3

objs=pokecalc.o sqlite.o nature.o


all:
	make -j pokecalc

pokecalc: $(objs)
clean::
	rm -vf pokecalc $(objs)
