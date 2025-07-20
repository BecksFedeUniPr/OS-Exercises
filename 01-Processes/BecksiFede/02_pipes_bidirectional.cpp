#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

int main()
{
    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];
    pid_t pid;
    int number;

    // Creazione delle pipe
    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1)
    {
        std::cerr << "Errore nella creazione delle pipe" << std::endl;
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid < 0){
        std::cerr << "Errore nella fork " << std::endl;
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);

        read(pipe_parent_to_child[0],&number,sizeof(number));

        std::cout << "Il figlio ha ricevuto: " << number << std::endl;

        number++;

        write(pipe_child_to_parent[1],&number,sizeof(number));

        close(pipe_child_to_parent[1]);
        close(pipe_parent_to_child[0]);
        exit(EXIT_SUCCESS);
    }
    else{
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);

        int number = 10;

        write(pipe_parent_to_child[1],&number,sizeof(number));

        read(pipe_child_to_parent[0],&number,sizeof(number));

        std::cout << "Il padre ha ricevuto: " << number << std::endl;

        close(pipe_child_to_parent[0]);
        close(pipe_parent_to_child[1]);
        wait(nullptr);
    }

    return 0;
}