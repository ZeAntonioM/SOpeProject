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

long verifyInput(char* arg, long *n) {
    char* endptr;
    *n = strtol(arg, &endptr, 10);
    
    return *endptr == '\0';
}

int main(int argc, char* argv[]) {
    FILE* file;
    char *theStr;
    int alreadyStored, arraySize = 0;
    long numberFrags, maxFragSize, test1, test2, tam, idx;
    long *alreadySeen; // Que capacidade?
    srandom(0);

    // Checks if number of arguments is correct (argv[], argc, n, m)
    if (argc != 4) {
        printf("Invalid number of arguments\nUsage: samples file numberfrags maxfragsize\n");
        return EXIT_FAILURE;
    }

    // Checks if the last two arguments are numbers
    if (!verifyInput(argv[2], &numberFrags) || !verifyInput(argv[3], &maxFragSize)) {
        printf("numberfrags and maxfragsize must be integers\n");
        return EXIT_FAILURE;
    }
    
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Provided file argument isn't a file\n");
        return EXIT_FAILURE;
    }

    test1 = ftell(file);
    fseek(file, 0, SEEK_END);
    test2 = ftell(file);
    tam = test2 - test1; // Rever tam

    if (tam < numberFrags + maxFragSize - 1) {
        printf("Can't get number of number of different fragments you asked with the fragment provided\n");
        return EXIT_FAILURE;
    }

    // Getting the fragments
    theStr = (char*) malloc(maxFragSize * sizeof(char));
    alreadySeen = (long*) malloc((tam-maxFragSize) * sizeof(long));

    while (numberFrags != 0) {
        long aTry;

        // Determinar inicio de proximo fragmento
        while (1) {
            alreadyStored = 0;
            aTry = random() % (tam-maxFragSize); // Rever isto
            for (int i = 0; i < arraySize; i++) {
                if (aTry == alreadySeen[i]) {
                    alreadyStored = 1;
                    break;
                }
            }
            if (!alreadyStored)
                break;
        }
        
        fseek(file, aTry, SEEK_SET);
        fread(theStr, maxFragSize, 1, file);

        // Print the string
        idx = 0;
        printf(">");
        while (*(theStr + idx) != '\0') {
            if (*(theStr + idx) == '\n')
                printf(" ");
            else
                printf("%c", *(theStr + idx));
            idx++;
        }
        printf("<\n");
        alreadySeen[arraySize++] = aTry;
        numberFrags--;
    }

    return 0;

}