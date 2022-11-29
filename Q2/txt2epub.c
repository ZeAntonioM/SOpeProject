#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
 #include <sys/types.h>
#include <sys/wait.h>



// Changes the extension of the file from ".txt" to ".epub"
 void changeNameToEpub( char* name){
    int size = strlen(name);
    int counter = 0;
    //copies the name of the file
    while(counter < size - 3){
        name++;
        counter++;
    }
    //Change extension to epub
    *name='e';
    name++;
    *name='p';
    name++;
    *name='u';
    name++;
    *name='b';
}


int main(int argc, char* argv[]) {
   
    char* filesInEpub[argc-1]; 
    pid_t pidChilds[argc-1];
    int size_for_zip;

    // Creates child processes and execs the pandoc command on each of the children
    for (int i = 1; i < argc; i++) {
        int pid = fork();
         char *epub= (char*)malloc(strlen(argv[i]));
            strcpy(epub,argv[i]);
            changeNameToEpub(epub);
            filesInEpub[i - 1] = epub;
            size_for_zip += strlen(filesInEpub[i - 1]) + 1;
        if(pid!=0) printf("[pide %d] converting %s\n", pid, argv[i]);
        if (pid == 0) {
            
            //char * func= (char*) malloc((12+ 2*strlen(argv[i])) * sizeof(char)); // PERGUNTAR AO STOR COMO DAR FREE A ISTO???
            //createFunc(func, argv[i],filesEmEpub[i]);
            char *args[]={"pandoc", argv[i] ,"-o",filesInEpub[i-1],NULL};
            //execlp("pandoc", argv[i] ,"-o",filesEmEpub[i-1],NULL); // pandoc f1.txt -o f1.epub
            //execlp("ls"," -l",NULL);
            execvp(args[0],args);
            
        }
        
        pidChilds[i - 1] = pid;
    }
    
    // Checks to see if any of the attempted file conversions resulted in an error
    for(int i = 0; i < argc - 1; i++) {
        int status;
        pid_t check_error = wait(&status);

        if (check_error == 1){
            printf("there was an error while converting the files to EPUB");
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
    
    
    //Deletes the repetead epub files that stay outside the zip
   for(int i = 0; i < argc - 1; i++) {
        char *args[]={"rm",filesInEpub[i],NULL};
        pid_t pid=fork();
        if(pid==0)execvp(args[0],args);
    
   }

    return 0;
}