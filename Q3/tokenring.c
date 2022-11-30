#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

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
void createWriteString(char* write, int process,int n){
        char * number;
        sprintf(number,"%d",process);
        strcat(write,"pipe");
        strcat(write,number);
        strcat(write,"to");
        sprintf(number,"%d",(process+1)%(n));
        strcat(write,number);

}
void createReadString(char* write, int process){
        char * number;
        sprintf(number,"%d",process-1);
        strcat(write,"pipe");
        strcat(write,number);
        strcat(write,"to");
        sprintf(number,"%d",process);
        strcat(write,number);
}



int main(int argc, char* argv[]) {

    char *checkInt;
    int process=1;

    //TODO verificações de argumentos 

    if (argc != 3) {
        printf("Usage: tokenring numberOfProcesses probability timeToStop");
        return EXIT_FAILURE;
    }

    int n = strtol(argv[1], &checkInt, 10), p = strtol(argv[2], &checkInt, 10), t = strtol(argv[3], &checkInt, 10);
    pid_t pid;
    int token;

    char fifoWrite[6 + 2*(n/10 + 1)], fifoRead[6 + 2*(n/10 + 1)];


    for (int i = 2; i <= n; i++) {
        pid = fork();
        if (pid == -1) {
            printf("Erro a criar processo bubuntu sucks");
            return EXIT_FAILURE;
        }
        else if (pid == 0) {
            //strcpy(fifoWrite, /*str do fifoWrite*/)
            //strcpy(fifoRead, /*str do fifoRead*/)
            //char fifoWrite[6 + 2*(n/10 + 1)], fifoRead[6 + 2*(n/10 + 1)];
            createWriteString(fifoWrite, i,n);
            createReadString(fifoRead,i);
            break;
        }

        else {
            char fifopath[6 + 2*(n/10 + 1)];
            createReadString(fifopath,i);
            mkfifo(fifopath, 0777); 
        }
    }

    
    // First Process must start the writing
    if (pid > 0) {
        write(fifoWrite, token, sizeof(int));
    }

    while (1) {
        int fd;
        fd = open(fifoRead);
        if (read(fifoRead, token, sizeof(int) == -1))
            perror("Nao consegui ler :(");

        close(fd);
        token++;
        fd = open(fifoWrite);
        if (write(fifoWrite, token, sizeof(int)) == -1)
            perror("Nao consegui escrever ;-;");
        close(fd);
    }
    
}