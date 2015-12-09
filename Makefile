# makefile for baseballDb code
#
# USAGE:    
#       make all    - make all required binaries
#       make clean  - delete all intermediate files and binaries
#

# specify libraries
LIBS =

# specify compiler flags
CFLAGS = -Wall

# supply source file here
SRC=$(wildcard *.c)

all: $(SRC)
	@echo "### Output generated from file(s): ###\n"$(SRC)
	$(foreach cmd, $(SRC:.c=),gcc $(CFLAGS) $(cmd).c $(LIBS) -o $(cmd) ;)

#baseball: $(SRC)
#	gcc -o $@ $^ $(CFLAGS) $(LIBS)

# delete all intermediate and output files
clean:
	$(foreach cmd, $(SRC:.c=),rm -f $(cmd).o $(cmd) ;)
