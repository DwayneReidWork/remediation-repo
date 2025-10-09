CC = gcc --std gnu99 -D_GNU_SOURCE

CFLAGS = -Wall -g -I ./inc

# Source and header files
SRCS = basic_interpreter.c
HDRS = basic_interpreter.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = basic_interpreter

# Default rule
all: clean $(EXEC)

# Link object files to create executable
$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean rule
clean:
	rm -f $(EXEC) $(OBJS)

# Clean build + valgrind with -v (verbose)
v: clean $(EXEC)
	valgrind -v ./$(EXEC)

# Clean build + valgrind with leak check
vv: clean $(EXEC)
	valgrind --leak-check=full ./$(EXEC)

# Clean build + valgrind with leak check + summary
vvv: clean $(EXEC)
	valgrind --leak-check=full -s ./$(EXEC)

.PHONY: all clean