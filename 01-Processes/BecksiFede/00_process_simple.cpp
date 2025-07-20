#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#define N_PROCESS 2

int main() {
    pid_t pid[N_PROCESS];
    int status;

    for(int i = 0; i < N_PROCESS; i++) {
        pid[i] = fork();
        
        if(pid[i] < 0) {
            std::cerr << "Errore nella fork() numero " << i << std::endl;
            return 1;
        }
        if(pid[i] == 0) {
            // Codice del figlio
            std::cout << "Sono il figlio " << i 
                      << " (PID: " << getpid() 
                      << ", Parent PID: " << getppid() << ")" << std::endl;
            exit(EXIT_SUCCESS);  // Il figlio termina qui!
        }
    }
    
    // Solo il padre esegue questo codice
    for(int i = 0; i < N_PROCESS; i++) {
        waitpid(pid[i], &status, 0);
    }
    
    std::cout << "Tutti i figli sono terminati" << std::endl;
    return 0;
}