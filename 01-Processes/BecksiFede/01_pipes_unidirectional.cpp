#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main(){
    int pipefd[2];
    pid_t pid;
    char buffer[200];
    char message[] = "Ciao Roberto Bagnara";

    if(pipe(pipefd) == -1){
        std::cerr << "Errore nella creazione";
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid < 0 ){
        std::cerr << "Errore nella fork" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        close(pipefd[1]);

        read(pipefd[0],buffer,sizeof(buffer));
        std::cout << "Figlio ha ricevuto : " << buffer << std::endl;
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }else{
        close(pipefd[0]);  // Chiude l'estremità di lettura

        // Scrive nella pipe
        write(pipefd[1], message, strlen(message) + 1);
        
        close(pipefd[1]);  // Chiude l'estremità di scrittura
        wait(nullptr);
    }
}