CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 
LIBS = ../libcs50/libcs50-given.a

.PHONY: all clean

all: common.a

common.a: pagedir.o index.o word.o
	ar rcs $@ $^

pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c $<

index.o: index.c index.h
	$(CC) $(CFLAGS) -c $<

word.o: word.c word.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f common.a