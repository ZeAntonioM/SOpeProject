# we vibin
SHELL := /bin/bash

CXXFLAGS = -Wall -Wuninitialized -Werror

tokenring: code.o
	gcc $(CXXFLAGS) code.o -o tokenring

code.o:
	gcc -c $(CXXFLAGS) tokenring.c -o code.o

clean:
	rm -f tokenring code.o

clean_fifos:
	find . -name "pipe*" -type f
	