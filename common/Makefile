CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = ../libcs50/libcs50.a
OBJS = pagedir.o

all: common.a pagedir

common.a: $(OBJS)
	ar rcs $@ $^

pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c $<

pagedir: pagedir.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *~
	rm -f *.o
	rm -f common.a
	rm -f pagedir