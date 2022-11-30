#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
    Para fazer os fifos usar mkfifo(PATH, Visibilidade)
    
    0 - criar os processos
    1 - Criar os FIFOS
    2 - 

    n -> numero de processos
    p -> probabilidade de parar
    t -> tempo de espera quando para

    intImportante TempN = n;
    int pidInicial =  getpid(), token = 0, pathings = n;
    char* fifoEscrever[], * fifoLer[];


    while (tempN != 0) {
        strcpy(fifoLer, "pipe + pathings + to + (pathings + 1) % n")
        n = (n + 1) % n;
        int pid = fork();
        if (pid == -1) {
            ERROOOOOOOOOOOOOOOU
        }
        else if (pid == 0) {
            BRAZIU

        }
        else {
            if ( ( int succeeded = mkfifo() ) == -1) {
                ERRRRROOOOOOOOOOOOOOOOOOU
            }
            
            strcpy(fifoEscrever, "pipe + pathings + to + (pathings + 1) % n");
            write(fifoEscrever, token, sizeof(int))
            break;
        }
        tempN--;
    }
*/


int main(int argc, char* argv[]) {

    char *checkInt;

    //TODO verificações de argumentos 

    if (argc != 3) {
        printf("Usage: tokenring numberOfProcesses probability timeToStop");
        return EXIT_FAILURE;
    }

    int n = strtol(argv[1], &checkInt, 10), p = strtol(argv[2], &checkInt, 10), t = strtol(argv[3], &checkInt, 10);

    int token, pidINicial = getpid(), isFatherOfAll = 1, process = n;

    char fifoWrite[6 + 2*(n/10 + 1)], fifoRead[6 + 2*(n/10 + 1)];


    

    
    for (int i = 0; i < n; i++) {
        // mkfifo(/*str do fifo*/);
        int pid = fork();
        
        if (pid == -1) {
            printf("Erro a criar processo bubuntu sucks");
            return EXIT_FAILURE;
        }
        else if (pid == 0) {
            //strcpy(fifoWrite, /*str do fifoWrite*/)
            //strcpy(fifoRead, /*str do fifoRead*/)
            isFatherOfAll = 0;
            break;
        }
        else {
            
        }
    }



}