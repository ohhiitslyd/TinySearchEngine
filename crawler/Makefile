# Makefile for crawler
# 
# David Kotz - April 2016, 2017, 2021

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
OBJS = crawler.o ../libcs50/bag.o ../libcs50/webpage.o ../libcs50/file.o
LIBS = ../libcs50/libcs50-given.a ../common/common.a

all: crawler

crawler: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

crawler.o: crawler.c ../libcs50/bag.h ../libcs50/webpage.h ../common/pagedir.h ../libcs50/file.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.o
	rm -f crawler