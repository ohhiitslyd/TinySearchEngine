# Makefile for crawler
# 
# David Kotz - April 2016, 2017, 2021

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = ../libcs50/bag.c ../libcs50/webpage.c ../libcs50/file.c
OBJS = crawler.o $(LIBS:.c=.o)
LLIBS = ../libcs50/libcs50-given.a ../common/common.a

all: crawler

crawler: crawler.o $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) -o $@ $^

crawler.o: crawler.c ../libcs50/bag.h ../libcs50/webpage.h ../common/pagedir.h ../libcs50/file.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.o
	rm -f crawler