#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

/**
 * Verifies if input given is actually a long integer and assigns it to "n".
 * endptr will hold the string after the first character that isn't a digit
*/
long verifyInputLong(char* arg, long *n) {
    char* endptr;
    *n = strtol(arg, &endptr, 10);
    
    return *endptr == '\0';
}

/**
 * Verifies if input given is actually a double and assigns it to "n".
 * endptr will hold the string after the first character that isn't a digit
*/
double verifyInputDouble(char* arg, double *n) {
    char* endptr;
    *n = strtod(arg, &endptr);
    return *endptr == '\0';
}

// Creates a sring with the name of a pipe to which a process is going to write
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
// Creates a sring with the name of a pipe to which a process is going to read
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
// Returnn true with a probability of prob
int rollTheDice(double prob){
    double r = (double)random()/(pow(2,31)-1);
    return r <= prob;
}



int main(int argc, char* argv[]) {
    long n, t, fd1, fd2;
    pid_t pid;
    long token = 0;
    double p;

    
    // Checking if the number of arguments is valid
    if (argc != 4) {
        printf("Usage: tokenring numberOfProcesses probability timeToStop\n");
        return EXIT_FAILURE;
    }

   
    // Verify if the number of processes and the time to lock are valid
    if (!verifyInputLong(argv[1], &n) || !verifyInputLong(argv[3], &t)) {
        fprintf(stderr, "numberOfProcesses and timeToStop must be long integers\n");
        return EXIT_FAILURE;
    }

    // Verify if the probability is valid
    if (!verifyInputDouble(argv[2],&p)) {
        fprintf(stderr,"probability must be double\n");
        return EXIT_FAILURE;
    }

    // The timeToStop must be positive or zero
    if (t < 0) {
        fprintf(stderr,"timeToStop must be 0 or positive\n");
        return EXIT_FAILURE;
    }

    // The probability must be a value between 0 and 1 (inclusive)
    if (p < 0 || p > 1) {
        fprintf(stderr,"timeToStop must be 0 or positive\n");
        return EXIT_FAILURE;
    }
    
    //The numberOfProcesses must be at least 2 so that they can communicate with each other 
    if (n < 2) {
        fprintf(stderr, "numberOfProcesses must be greater than 2\n");
        return EXIT_FAILURE;   
    }
    
    // Initializing the variables to store the named pipes in which each process is going to read and write
    char fifoWrite[64], fifoRead[64];

    // Associating a named pipe to write and a named pipe to read to the initial process
    strcpy(fifoRead, "");
    strcpy(fifoWrite, "");
    createWriteString(fifoWrite, 1,n);
    createReadString(fifoRead,1,n);
    
    // Creating the first named pipe
    mkfifo(fifoRead, 0777);
   
   // Creating all processes and named pipes
    for (int i = 2; i <= n; i++) {
        pid = fork();   // Create a process

        // If it fails to create the process gives error message
        if (pid == -1) {
            perror("Error using fork(): ");
            return EXIT_FAILURE;
        }
        else if (pid == 0) {
            //Associates a process to a random seed and to their respectively named pipe to read and named pipe to write
            srandom(i*100);
            createWriteString(fifoWrite, i, n);
            createReadString(fifoRead, i, n);
            break;
        }

        else {
            // Initial process runs this code, creating all the named files
            char fifopath[64 + 1];
            strcpy(fifopath, "");
            createReadString(fifopath,i,n);
            mkfifo(fifopath, 0777);
        }
    }

    //Initial process is going to send the token to initiate the ring
    if (pid > 0) {
        fd1 = open(fifoWrite, O_WRONLY);
        write(fd1, &token, sizeof(long));
    }

    // Processes will send messages to other processes
    while (1) {
        // Process is going to read token from its reading pipe
        fd1 = open(fifoRead, O_RDONLY);
        long token=-1;

        // Process will continue trying to read until it is able to do so
        while(token==-1){
            if (read(fd1, &token, sizeof(long)) == -1) {
                printf("Token: %ld\n", token);
                perror("Couldn't read on pipe\n");
                return EXIT_FAILURE;
            }
        }
        
        close(fd1);

        token++; 
        
        // The code sleeps for t seconds with a probability of p
        if(rollTheDice(p)){
            fprintf(stderr, "[p%ld] lock on token (val = %ld)\n", token % n + 1, token);
            sleep(t);   
            fprintf(stderr, "[p%ld] unlock token\n", token % n + 1);
        }

        // Process if going to send the incremented token to another process via its writing pipe
        fd2 = open(fifoWrite, O_WRONLY);
        if (write(fd2, &token, sizeof(long)) == -1){
            perror("Couldn't write on pipe\n");
            return EXIT_FAILURE;
        }
        close(fd2);
    }
    return EXIT_SUCCESS;
}