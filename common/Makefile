CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

.PHONY: all clean

all: common.a

common.a: pagedir.o index.o word.o
	ar rcs $@ $^

pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c $<

index.o: index.c
	$(CC) $(CFLAGS) -c $<

word.o: word.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *.a