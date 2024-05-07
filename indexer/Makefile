# Compiler options
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

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
