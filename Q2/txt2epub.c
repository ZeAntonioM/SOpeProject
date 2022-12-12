#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



// Changes the extension of the file from ".txt" to ".epub"
void changeNameToEpub(char* name){
    int size = strlen(name);
    int counter = 0;
    // Copies the name of the file
    while(counter < size - 3){
        name++;
        counter++;
    }
    // Change extension to epub
    *name = 'e';
    name++;
    *name = 'p';
    name++;
    *name = 'u';
    name++;
    *name = 'b';
}


int main(int argc, char* argv[]) {
   
    char* filesInEpub[argc-1];  // Holds the initial files with .epub extenson
    int size_for_zip;

    // Creates child processes and executes the pandoc command on each of the children
    for (int i = 1; i < argc; i++) {
        int pid = fork();
        char *aFile= (char*)malloc(strlen(argv[i]));

        strcpy(aFile ,argv[i]);
        changeNameToEpub(aFile);

        filesInEpub[i - 1] = aFile;
        size_for_zip += strlen(filesInEpub[i - 1]) + 1;
        
        if (pid < 0) {
            perror("Couldn't create a process with fork\n");
            return EXIT_FAILURE;
        }
        else if (pid == 0) {
            char *args[] = {"pandoc", argv[i], "-o", filesInEpub[i-1], NULL};
            execvp(args[0], args); 
        }
        else {
            printf("[pid %d] converting %s\n", pid, argv[i]);
        }
    }
    
    // Checks to see if any of the attempted file conversions resulted in an error
    for(int i = 0; i < argc - 1; i++) {
        int status;
        pid_t check_error = wait(&status);

        if (check_error == -1){
            perror("There was an error involving pandoc.");
            return EXIT_FAILURE;
        }
    }   

    // Creates the string to then use for the "zip" command
    char* str_to_zip = (char*) malloc((14 + size_for_zip) * sizeof(char));
    strcat(str_to_zip, "zip ebooks.zip");
    for (int i = 0; i < argc - 1; i++){
        strcat(str_to_zip, " ");
        strcat(str_to_zip, filesInEpub[i]);
    }
    // Execute zip command with created string
    system(str_to_zip);

    return EXIT_SUCCESS;
}
