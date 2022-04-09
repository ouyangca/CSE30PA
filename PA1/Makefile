# if you type 'make' without arguments, this is the default
PROG = omit
all: $(PROG)

# other source files and the associated object files (this can be blank) SRC is unused here
SRC     = omit.c subs.c
HEAD	= omit.h
OBJ     = omit.o subs.o

# Testing
TDIR	= ./tests

# special libraries
LIB	=

# select the compiler and flags yopu can over-ride on command line e.g. make DEBUG= 
CC      = gcc
DEBUG	= -ggdb
CSTD	=
WARN	= -Wall -Wextra -Werror
CDEFS	=
CFLAGS	= -I. $(DEBUG) $(WARN) $(CSTD) $(CDEFS)

$(OBJ):	$(HEAD)

# specify how to compile the target
$(PROG):	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $@

# test the program
test:	$(PROG)
	(cd $(TDIR); make $@)

# remove binaries
.PHONY: clean clobber
clean:
	rm -f $(OBJ) $(PROG)
	(cd $(TDIR); make $@)

# remove binaries and other junk
clobber: clean
	rm -f core
