.PHONY: all clean

CFLAGS=-g -Wall
LDLIBS=-lsqlite3

objs=pokecalc.o sqlite_helper.o nature.o


all:
	make -j pokecalc

createDB.sql: scrape.sh
	./$< $@

poke.db: createDB.sql
	sqlite3 $@ < $<

pokecalc: $(objs)
clean::
	rm -vf pokecalc $(objs)
