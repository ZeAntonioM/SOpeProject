#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

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
        
        strcpy(write,"");
      
        char number[10];
        strcpy(number, "");
        sprintf(number,"%d",process);
        strcat(write,"pipe");
        strcat(write,number);
        strcat(write,"to");
        sprintf(number,"%d", process == n ? 1 : (process + 1));
        strcat(write,number);

}
void createReadString(char* read, int process,int n){
        strcpy(read, "");
        char number[10];
       
        strcpy(number, "");
        sprintf(number,"%d", process == 1 ? n : process-1);
        strcat(read,"pipe");
        strcat(read,number);
        strcat(read,"to");
        sprintf(number,"%d",process);
        strcat(read,number);

}

int roolTheDice(double prob){
    double r = (double)random()/(pow(2,31)-1);
    printf("Probability: %lf\n", r);
    return r <= prob;
}



int main(int argc, char* argv[]) {
    int fd;
    char *checkInt;
    srandom(0);
    //int process=1;
    //TODO verificações de argumentos 

    if (argc != 4) {
        printf("Usage: tokenring numberOfProcesses probability timeToStop\n");
        return EXIT_FAILURE;
    }

    int n = strtol(argv[1], &checkInt, 10),  t = strtol(argv[3], &checkInt, 10);
    pid_t pid;
    long token = 1;
    double p = strtod(argv[2], &checkInt);
  
    char fifoWrite[6 + 2*(n/10 + 1) + 1], fifoRead[6 + 2*(n/10 + 1) + 1];
    strcpy(fifoRead, "");
    strcpy(fifoWrite, "");
   // printf("THE STRING: %s\n", fifoWrite);
    createWriteString(fifoWrite, 1,n);
    createReadString(fifoRead,1,n);
    mkfifo(fifoRead, 0777);
    printf("Pid: %d; fileToWrite: %s; fileToRead: %s\n", getpid(), fifoWrite, fifoRead);
    //printf("fifoPath: %s\n", fifoRead);

    for (int i = 2; i <= n; i++) {
        
        pid = fork();

        if (pid == -1) {
            printf("Erro a criar processo bubuntu sucks");
            return EXIT_FAILURE;
        }
        else if (pid == 0) {
            //printf("PID: %d, return pid: %d\n", getpid(), pid);
            
            //strcpy(fifoWrite, str do fifoWrite)
            //strcpy(fifoRead, str do fifoRead)
            //char fifoWrite[6 + 2*(n/10 + 1)], fifoRead[6 + 2*(n/10 + 1)];
            srandom(i*10);
            createWriteString(fifoWrite, i,n);
            createReadString(fifoRead,i,n);
            
            printf("Pid: %d; fileToWrite: %s; fileToRead: %s\n", getpid(), fifoWrite, fifoRead);
            break;
        }

        else {

            char fifopath[6 + 2*(n/10 + 1) + 1];
            strcpy(fifopath, "");
            createReadString(fifopath,i,n);
            //printf("fifoPath: %s\n", fifopath);
            mkfifo(fifopath, 0777);
        }
    }

    // First Process must start the writing
    if (pid > 0) {
        printf("%s\n",fifoWrite);
        fd = open(fifoWrite, O_WRONLY);
        write(fd, &token, sizeof(long));
        close(fd);
    }

    while (1) {
       
        //printf("Pid: %d, Stuck\n", getpid());
        long token = -1;
        fd = open(fifoRead, O_RDONLY);
        fprintf(stderr, "Trying to read: %d\n", getpid());

        // Not having the token == -1 was giving some errors, we suspect the a process can 
        while (token == -1) {
            if (read(fd, &token, sizeof(long)) == -1) {
                fprintf(stderr, "Couldn't read");
                return EXIT_FAILURE;
            }
        }
        close(fd);
        fprintf(stderr, "Check token: %ld, pid: %d\n", token, getpid());
        token++;
        if(roolTheDice(p)){ 

            sleep(t);
            
            }
        fprintf(stderr,"Pid: %d; token: %ld\n", getpid(), token);
        fd = open(fifoWrite, O_WRONLY);
        if (write(fd, &token, sizeof(long)) == -1){
            fprintf(stderr, "Couldn't write");
            return EXIT_FAILURE;
        }
        close(fd);
        //printf("aaa\n");
    }
    return EXIT_SUCCESS;
}