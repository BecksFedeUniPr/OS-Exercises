#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define W_END 1 //estremità scrittura
#define R_END 0 //estremità lettura

/*
Queste costanti rappresentano gli indici dell'array che costituisce una pipe POSIX:
Quando crei una pipe con pipe(), ottieni un array di due file descriptor:
- pipe_array[0] è sempre il descrittore per leggere
- pipe_array[1] è sempre il descrittore per scrivere

Infatti nel CHILD =>

close(parent_to_child[W_END]); // Non scrive su questa pipe
close(child_to_parent[R_END]); // Non legge da questa pipe

read(parent_to_child[R_END], ...); // Legge i messaggi del genitore
write(child_to_parent[W_END], ...); // Scrive messaggi al genitore

nel PARENT =>

close(parent_to_child[R_END]); // Non legge da questa pipe
close(child_to_parent[W_END]); // Non scrive su questa pipe

write(parent_to_child[W_END], ...); // Scrive messaggi al figlio
read(child_to_parent[R_END], ...); // Legge i messaggi del figlio

*/


#define MAX_ROUNDS 10

int main(int argc, char const *argv[])
{
    
    //declare PIPEs: PARENT->CHILD , CHILD->PARENT
    int parent_to_child[2];
    int child_to_parent[2];

    if(pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1){ //create pipe
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); //generate child process

    if (pid < 0){
        perror("Child fork failed");
        exit(EXIT_FAILURE);
    } 

    if (pid == 0){ //CHILD process code
        close(parent_to_child[W_END]); //close estremità che non uso
        close(child_to_parent[R_END]);

        printf("CHILD: pronto a calcolare! \n");

        while(1){ //Read number send by PARENT
            int number;
            read(parent_to_child[R_END], &number, sizeof(int));

            if(number == 0){ 
                printf("Received terminal signal (0): STOP");
                break; 
            }

            printf("CHILD: Ho ricevuto %d, calcolo il quadrato...\n", number);

            int res = number * number; //calcolo

            printf("CHILD: Invio risultato %d al padre\n", res);
            write(child_to_parent[W_END], &res, sizeof(int)); //Send res to PARENT

        }

        close(parent_to_child[R_END]); //close pipe still opened
        close(child_to_parent[W_END]);

        printf("CHILD termineted! \n");
        exit(EXIT_SUCCESS);

    } else { //PARENT CODE if(pid > 0)
        close(parent_to_child[R_END]); //close estremità che non uso
        close(child_to_parent[W_END]);

        printf("PARENT: start gioco numeri^2! \n");
        srand(time(NULL));

        for (int round = 1; round < MAX_ROUNDS; ++round) { // genero n da 1 a 10 
            int number = rand() % 10 + 1;
            int expected_res_CHILD = number * number;

            printf("PARENT: Round %d => Invio il numero %d al CHILD...\n", round, number);

            write(parent_to_child[W_END], &number, sizeof(int)); //send to CHILD

            int res;
            read(child_to_parent[R_END], &res, sizeof(int));

            printf("PARENT: Ho ricevuto %d come risposta, ", res);
            if (res == expected_res_CHILD) {
                printf("is CORRECT!\n\n");
            } else {
                printf("is WRONG! Doveva essere %d\n", expected_res_CHILD);
            }

            //sleep(1); //pausa da sostituire con semaforo
        }

        int end_signal = 0; //chiudo comunicazione inviando 0
        write(parent_to_child[W_END], &end_signal, sizeof(int));

        close(parent_to_child[R_END]); //close pipe still opened
        close(child_to_parent[W_END]);

        printf("PARENT: terminato!\n");
        
    }

    return EXIT_SUCCESS;
}

/*

Sefamori o mutex non servono perché:

Un solo scrittore e un solo lettore per ogni pipe,
Il pattern di comunicazione è semplice e alternato
Le pipe POSIX hanno già incorporati meccanismi di sincronizzazione a livello kernel:
    
    Operazioni bloccanti:
        - read() si blocca automaticamente se non ci sono dati da leggere
        - write() si blocca se il buffer della pipe è pieno (tipicamente 64KB)

    Atomicità garantita:
        - Scritture fino a PIPE_BUF bytes (tipicamente 4KB) sono atomiche
        - Il kernel gestisce l'accesso concorrente

*/

