#include <stdio.h>



int main(int argc, char* argv[]) {

    /*
    for (int i = 1; i < argc; i++) {
        criar a string com os vários nomes em .pid para usar no cenos do zip e do pandoc? 
    }
    */

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