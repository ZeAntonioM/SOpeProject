ARGS1 = testfile.txt
ARGS2 = Q2/christmas.txt Q2/filosofia.txt Q2/guerra.txt Q2/lusiadas.txt Q2/maias.txt

all:

run_samples: Q1/samples 
	Q2

samples: Q1/samples.c
	gcc -o Q1/samples.c 

txt2epub: Q2/txt2epub.c
	gcc -o Q2/txt2epub.c

tokenring: Q3/tokenring.c
	gcc -o Q3/tokenring.c

clean:
	rom -f all samples txt2epub tokenring Q2/ebooks.zip Q2/christmas.epub Q2/filosofia.epub Q2/guerra.epub Q2/lusiadas.epub Q2/maias.epub tokenring