# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC = gcc
LINKERFLAGS = -lm

all: launch

compile:
	${CC} ${LINKERFLAGS} src/noted.c -o noted

debug:
	${CC} ${LINKERFLAGS} -g src/noted.c -o noted

launch: compile
	@echo 
	@./noted file.txt

clean:
	rm noted