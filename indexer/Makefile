CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = ../libcs50/libcs50-given.a ../common/common.a

PROGS = indexer indextest
OBJS = indexer.o indextest.o

.PHONY: all clean test

all: $(PROGS)

indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indexer.o: indexer.c
	$(CC) $(CFLAGS) -c $<

indextest.o: indextest.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROGS)
	rm -f core.*

test: $(PROGS)
	bash -v testing.sh