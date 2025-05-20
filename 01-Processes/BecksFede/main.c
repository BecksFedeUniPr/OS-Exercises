#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>


int main(){
    int shmid;
    char* str;
    pid_t pid;

    shmid = shmget(IPC_PRIVATE,100,IPC_CREAT | 0666);

    if(smhid < 0){
        perror("Fallita la creazione di memoria condivisa");
    }

    pid = fork();

    if(pid < 0){
        perror("Fork failed");
    }else if(pid == 0){
        printf("Sono il processo figlio, PID = %d, padre = %d\n", getpid(), getppid());
        str = shmat(shmid,NULL,0);
        printf("Reader %s \n",str);
        shmdt(str);
        exit(0);
    }else{
        printf("Sono il processo padre, PID = %d, figlio = %d\n", getpid(),pid);
        str = (char *) shmat(shmid, NULL, 0);
        strcpy(str, "Messaggio condiviso!");
        shmdt(str);
        wait(NULL); // aspetta il figlio
        shmctl(shmid, IPC_RMID, NULL); // elimina il segmento
        exit(0);
    }

    return 0;
}