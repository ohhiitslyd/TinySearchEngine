CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

.PHONY: all clean

all: common.a

common.a: pagedir.o
	ar rcs $@ $^

pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *.a