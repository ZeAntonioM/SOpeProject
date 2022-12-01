#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

long verifyInputLong(char* arg, long *n) {
    char* endptr;
    *n = strtol(arg, &endptr, 10);
    
    return *endptr == '\0';
}

double verifyInputDouble(char* arg, double *n) {
    char* endptr;
    *n = strtod(arg, &endptr);
    
    return *endptr == '\0';
}


void createWriteString(char* write, int process,int n){
        char number[10];

        strcpy(write,"");
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

int rollTheDice(double prob){
    double r = (double)random()/(pow(2,31)-1);
    return r <= prob;
}



int main(int argc, char* argv[]) {
    /*Initializing needed variables */
    
    char *checkInt;
    long n, t;
    pid_t pid;
    long token = 0;
    double p;

    //srandom(0);// Setting a seed for the random of the main process
    
    /* Cheching if the number of arguments is valid */
    if (argc != 4) {
        printf("Usage: tokenring numberOfProcesses probability timeToStop\n");
        return EXIT_FAILURE;
    }

   
    /*verify if the number of processes and the time of lock are valid and give them their values to the variables n and t respectively if so*/
    if (!verifyInputLong(argv[1], &n) || !verifyInputLong(argv[3], &t)) {
        fprintf(stderr, "numberOfProcesses and timeToStop must be long\n");
        return EXIT_FAILURE;
    }

    /*verify if the probability is valid and give his value to the variables p if so*/
    if (!verifyInputDouble(argv[2],&p)) {
        fprintf(stderr,"probability must be double\n");
        return EXIT_FAILURE;
    }

    /*The probability and the timeToStop must be positive or zero*/
    if ( p < 0 || t < 0 ) {
        fprintf(stderr,"Arguments must be positive\n");
        return EXIT_FAILURE;
    }
    
    /*The numberOfProcesses must be at least 2 so that they can comunicate with each other*/    
    if ( n < 2 ) {
        fprintf(stderr, "numberOfProcesses must be at least 2");
        return EXIT_FAILURE;   
    }
    
    /*Initializing the variables to store the FIFOS in wich each pid is going to read and write*/
    char fifoWrite[1024], fifoRead[1024];

    /*Associating a FIFO to write and a FIFO to read to the initial process*/
    strcpy(fifoRead, "");
    strcpy(fifoWrite, "");
    createWriteString(fifoWrite, 1,n);
    createReadString(fifoRead,1,n);
    
    /*Creating the FIFO*/
    mkfifo(fifoRead, 0777);
   
   /*Creating all and FIFOS*/
    for (int i = 2; i <= n; i++) {
        
        pid = fork();//crate a pid

        /*if it fails to create the pid gives error message*/
        if (pid == -1) {
            printf("Erro a criar processo bubuntu sucks");
            return EXIT_FAILURE;
        }
        else if (pid == 0) {
            /*Associates a process to a random seed and to their respectively FIFO to read and FIFO to write */
            srandom(i*100);
            createWriteString(fifoWrite, i,n);
            createReadString(fifoRead,i,n);
            break;
        }

        else {
            /*Code only executed by initial process creates the pipe*/
            char fifopath[6 + 2*(n/10 + 1) + 1];
            strcpy(fifopath, "");
            createReadString(fifopath,i,n);
            mkfifo(fifopath, 0777);
        }
    }

    if (pid > 0) {
        int fd1 = open(fifoWrite, O_WRONLY);
        write(fd1, &token, sizeof(long));
    }

    while (1) {
        int fd1 = open(fifoRead, O_RDONLY);
        long token=-1;
        while(token==-1){
        if (read(fd1, &token, sizeof(long)) == -1) {
            printf("Token: %ld\n", token);
            perror("Couldn't read on pipe\n");
            return EXIT_FAILURE;
        }
        }
        
        close(fd1);
        token++;
        
        if(rollTheDice(p)){
            fprintf(stderr, "[p%ld] lock on token (val = %ld)\n", token % n + 1, token);
            sleep(t);   
            fprintf(stderr, "[p%ld] unlock token\n", token % n + 1);
        }
        
        int fd2 = open(fifoWrite, O_WRONLY);
        if (write(fd2, &token, sizeof(long)) == -1){
            perror("Couldn't write on pipe\n");
            return EXIT_FAILURE;
        }
        close(fd2);
    }
    return EXIT_SUCCESS;
}