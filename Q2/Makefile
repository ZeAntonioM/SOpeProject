# we vibin

CXXFLAGS = -Wall -Wuninitialized -Werror

txt2epub: code.o
	gcc $(CXXFLAGS) code.o -o txt2epub

code.o:
	gcc -c $(CXXFLAGS) txt2epub.c -o code.o

clean:
	rm -f txt2epub code.o 

cleanEpub:
	rm -f *.epub

cleanTxt:
	rm -f *.txt

cleanZip:
	rm -f *.zip

cleanAll: cleanEpub cleanZip
