#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/**
 * Verifies if input given is actually a long integer and assigns it to "n".
 * endptr will hold the string after the first character that isn't a digit
*/
long verifyInput(char* arg, long *n) {
    char* endptr;
    *n = strtol(arg, &endptr, 10); 
    return *endptr == '\0';
}

int main (int argc, char* argv[]) {
    FILE* file;
    char *theStr;
    int alreadyStored, arraySize = 0;
    long numberFrags, maxFragSize, initialPos, finalPos, tam, idx;
    long *alreadySeen;

    srandom(0); // setting a seed for the random generation of numbers

    // Checks if number of arguments is correct (argc, File, n, m)
    if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments\nUsage: samples file numberfrags maxfragsize\n");
        return EXIT_FAILURE;
    }

    // Checks if the last two arguments are numbers
    if (!verifyInput(argv[2], &numberFrags) || !verifyInput(argv[3], &maxFragSize)) {
        fprintf(stderr, "numberfrags and maxfragsize must be integers\n");
        return EXIT_FAILURE;
    }

    if (numberFrags <= 0 || maxFragSize <= 0) {
        fprintf(stderr, "number of fragments and size of fragments must be more than 0\n");
        return 0;
    }
    
    // Checks if file is actually a file
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Coudln't read file with fopen");
        return EXIT_FAILURE;
    }
    
    // Get the size of the file
    initialPos = ftell(file);
    fseek(file, 0, SEEK_END);
    finalPos = ftell(file);
    tam = finalPos - initialPos; 
    
    // Checks if its possible to get n diferent strings with size s from the file
    if (tam < numberFrags + maxFragSize - 1) {
        fprintf(stderr,"It is impossible to get %ld diferent strings with size %ld from this file.\n", numberFrags, maxFragSize);
        return EXIT_FAILURE;
    }

    theStr = (char*) malloc(maxFragSize * sizeof(char));
    alreadySeen = (long*) malloc((numberFrags) * sizeof(long));  // Array of positions already accessed

    while (numberFrags != 0) {
        long aTry;

        // Generate "numberfrags" different numbers
        while (1) {
            alreadyStored = 0;
            aTry = random() % (tam - maxFragSize + 1); 
            for (int i = 0; i < arraySize; i++) {
                if (aTry == alreadySeen[i]) {
                    alreadyStored = 1;
                    break;
                }
            }
            if (!alreadyStored)
                break;
        }

        // Gets the string it should read
        fseek(file, aTry, SEEK_SET);
        fread(theStr, maxFragSize, 1, file);

        idx = 0;
        printf(">");
        
        // Prints the string replacing newlines with spaces
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
    
    // Frees memory allocated to pointers
    free(theStr);
    free(alreadySeen);

    return 0;
}