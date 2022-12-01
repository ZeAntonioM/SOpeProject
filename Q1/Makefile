# we vibin

CXXFLAGS = -Wall -Wuninitialized -Werror

samples: code.o
	gcc $(CXXFLAGS) code.o -o samples

code.o:
	gcc -c $(CXXFLAGS) samples.c -o code.o

clean:
	rm -f samples code.o

clean_txt:
	rm -f *.txt