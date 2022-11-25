#include <stdio.h>



int main(int argc, char* argv[]) {

    char* filesEmEpub[argc-1]; //A ideia é irmos colocando aqui os ficheiros à medida que eles são criados para depois dar exec deste vetor no zip.


    //ter um vetor de pids dos filhotes
    
    for (int i = 1; i < argc; i++) {
        int pid = fork();
        //adicionar ao vetor o pid
        if (pid == 0) {
            //exec(pandoc argv[i]);
        }
    }
    
    /*
    for(todos os pids do vetor) {
        int erro ou nao erro = wait(pid);
    }
    */

    //exec (zip  piça/cona);
    

}