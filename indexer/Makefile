# Makefile for CS50 Indexer
#
# Lydia Jin, May 2024

# Compiler options
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

# Paths to libraries and includes
LIBCS50 = ../libcs50/libcs50-given.a
COMMON = ../common/common.a

# Target executables
TARGETS = indextest indexer

# Source files
SRCS = indextest.c indexer.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rule to build the target executables
all: $(TARGETS)

indextest: indextest.o $(COMMON) $(LIBCS50)
	$(CC) $(CFLAGS) $^ -o $@

indexer: indexer.o $(COMMON) $(LIBCS50)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target to clean up the directory
clean:
	rm -f $(OBJS) $(TARGETS)

# Phony target to run tests
.PHONY: test
test:
	chmod +x testing.sh
	./testing.sh > testing.out 2>&1