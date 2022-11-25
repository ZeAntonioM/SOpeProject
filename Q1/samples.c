#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


// fseek(FILE* stream, long offset, int whence -> ponto de começo pode ser SEEK_SET, SEEK_CUR E SEEK_END)
// random(void)
// srandom(seed)

// 1- se sample maior que texto, devemos dizer que nao pode ou ignorar e mandar a maior sample possivel
// 2- caso encontremos new line, descontamos como caraceters e continuamos a procura de outro, ou nao pode ter new line, ou
// nao mostramos e contamos a new line
// 3- 



int main(int argc, char* argv[]) {
    
    //Checks if number of arguments is correct (Argv[], argc, n, m)
    if (argc != 4) {
        printf("usage: samples file numberfrags maxfragsize");
        return 1;
    }


    FILE* file, test;
    char* theStr;
    int numberfrags, maxfragsize, tam;
    //
    srandom(0);
    
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Oh esupido do caralho mete a puta dum ficheiro men! pfv ;) :P :D B) xD :') :( ");
    }
    int test1 = ftell(file);
    fseek(file, 0, SEEK_END);
    int test2 = ftell(file);
    printf("%d\n", test2 - test1);
    tam = test2 - test1;

    if ( tam < ( atoi(argv[2]) + atoi(argv[3]) - 1 ) ) {
        printf("Dava jeito uma string em condições oh caralho, não peças merdas que não fazem sentido zézé ");
        return 1;
    }
    
    return 0;

}