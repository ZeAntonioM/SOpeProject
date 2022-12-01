SHELL := /bin/bash
REGEX_MATCH = "pipe[0-9]{1,9}to[0-9]{1,9}$$"

CXXFLAGS = -Wall -Wuninitialized -Werror

tokenring: code.o
	gcc $(CXXFLAGS) code.o -o tokenring

code.o:
	gcc -c $(CXXFLAGS) tokenring.c -o code.o

clean:
	rm -f tokenring code.o

# ls |grep -P (regex) filtra todos os ficheiros que dêem match à regex numa lista, xargs -d"\n" rm executa o comando "rm" a todas as linhas da lista, neste caso os ficheiros filtrados
clean_fifos:
	ls | grep -P $(REGEX_MATCH) | xargs -d"\n" rm