.PHONY: all clean

CFLAGS=-g -Wall
LDLIBS=-lsqlite3 -lcurl -ltidy

objs=pokecalc.o sqlite_helper.o nature.o tidy.o


all:
	make -j pokecalc

pokecalc: $(objs)
clean::
	rm -vf pokecalc $(objs)
