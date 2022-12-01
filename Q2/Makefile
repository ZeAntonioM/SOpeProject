# we vibin

CXXFLAGS = -Wall -Wuninitialized -Werror

txt2epub: code.o
	gcc $(CXXFLAGS) code.o -o txt2epub

code.o:
	gcc -c $(CXXFLAGS) txt2epub.c -o code.o

clean:
	rm -f txt2epub code.o 

clean_epub:
	rm -f *.epub

clean_txt:
	rm -f *.txt

clean_zip:
	rm -f *.zip