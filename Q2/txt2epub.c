#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
 #include <sys/types.h>
#include <sys/wait.h>



// Changes the name of the file from ".txt" to ".epub"
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

//BRO JUNTEM AS CENAS MALTAAAAAAAAAAAAA

//Create the function to pass as argument for the exec
void createFunc(char* func, char* argv, char* filesEmEpub){
   // strcat(func,"pandoc ");
    strcat(func,argv);
    strcat(func," -o ");
    strcat(func, filesEmEpub);
}

int main(int argc, char* argv[]) {
   /*
    char test[]= "JoeMama\0";
    *test = '4';
    printf("%s\n", test);
    */

    char* filesEmEpub[argc-1]; 
    pid_t pideFilhos[argc-1];//Alá salazaratings
    int size_for_zip;

    // Creates child processes and execs the pandoc command on each of the children
    for (int i = 1; i < argc; i++) {
        int pide = fork();

        if (pide == 0) {
            char * func= (char*) malloc((12+ 2*strlen(argv[i])) * sizeof(char)); // PERGUNTAR AO STOR COMO DAR FREE A ISTO???
            createFunc(func, argv[i],filesEmEpub[i]);
            execl("pandoc ", argv[i] ," -o ",filesEmEpub[i]); // pandoc f1.txt -o f1.epub
            
            
        }

        pideFilhos[i - 1] = pide;

        char *epub= (char*)malloc(strlen(argv[i]));
        strcpy(epub,argv[i]);
        changeNameToEpub(epub);

        filesEmEpub[i - 1] = epub;

        size_for_zip += strlen(filesEmEpub[i - 1]) + 1;

        printf("[pide%d] converting %s", pide, argv[i]);
        
        free(epub);
    }
    
    // Checks to see if any of the attempted file conversions resulted in an error
    for(int i = 0; i < argc - 1; i++) {
        int status;
        pid_t erro_ou_nao_erro = waitpid(pideFilhos[i],&status,WNOHANG);

        if (erro_ou_nao_erro == 1){
            printf("there was an error while converting the files to EPUB");
            return EXIT_FAILURE;
        }
    }   


    // Creates the string to then use for the "zip" command
    char* str_to_zip = (char*) malloc((14 + size_for_zip) * sizeof(char));
    strcat(str_to_zip, "zip ebooks.zip");
    for (int i = 0; i < argc - 1; i++){
        strcat(str_to_zip, " ");
        strcat(str_to_zip, filesEmEpub[i]);
    }


    // Execute zip command with created string
    system(str_to_zip);

    return 0;
}