#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strncpy()

#include <sys/mman.h> // f() shared memory podsix
#include <sys/stat.h> // mod constant
#include <fcntl.h>   // O_* constants
#include <unistd.h> // fork(), ftruncate()
#include <sys/wait.h> // wait()
#include <errno.h> //errno

/*
made by: Schianchi Federico 17/05/2025
gcc (or g++) -o <nome> <this.file> -lrt (per le librerie POSIX real-time linkata esplicitamente)

*/


typedef struct {
    int counter;
    char msg[100];
} shared_data_type;

int main(int argc, char *argv[])
{
    const char *shmemory_name = "/shared_memory";
    
    size_t shmemory_size = sizeof(shared_data_type);
    
    /*Create shared data object, file descriptor
    O_CREAT => create obj if not exist
    O_RDWR => open it in R & W
    3rd parameter => acces permission (0666 = R&W, for owner, Group and other)
    */

    int shmemory_fd = shm_open(shmemory_name, O_CREAT | O_RDWR, 0666);
    if(shmemory_fd == -1){
        perror("shm_open failed");
        exit(EXIT_FAILURE);
        
    }
    if (ftruncate(shmemory_fd, shmemory_size) == -1){
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    // MAP MEMORY in the address space of the process
    /* prm of mmap() => 
    1st = virtual address space (if NULL kernel decide)
    2nd = prot argument, not conflict with open mode of the fd, (PROT_ ... may be read, write, exec ...)
    3rd = flag argument, MAP_SHARED mapping visible to other process
    4th = fd ; 5th = offset, hide headers or debug info to other process that u dont need in memory
    */    

    shared_data_type *shdata = mmap(NULL, shmemory_size,
                            PROT_READ | PROT_WRITE, MAP_SHARED, shmemory_fd, 0);
    if (shdata == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    //initialize shared data
    shdata->counter = 0;
    strncpy(shdata->msg, "Init message", sizeof(shdata->msg)-1);

    pid_t pid = fork(); //create CHILD PROCESS => pid_t type for storing process IDs 

    if (pid < 0) { perror("Fork failed"); exit(EXIT_FAILURE); };
    if (pid == 0) {
        //CHILD PROCESS CODE
        printf("CHILD PROCESS: I read counter:%d, message='%s'\n",
                shdata->counter, shdata->msg);

        shdata->counter++; //modify shdata
        strncpy(shdata->msg, "Modify from child", sizeof(shdata->msg)-1);

        printf("CHILD PROCESS: I have updated counter:%d, message='%s'\n",
                shdata->counter, shdata->msg);

        if(munmap(shdata, shmemory_size) == -1) { //Unmaps a mapped memory region
            perror("munmap failed in CHILD");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } 
    else { // if (pid > 0)
        //PARENT PROCESS CODE, wait CHILD to complete (with mutex o semafori in real project)
        sleep(1); 

        printf("PARENT PROCESS: I read counter:%d, message='%s'\n",
                    shdata->counter, shdata->msg);

        wait(NULL); //WAIT CHILD

        //cleaning

        if(munmap(shdata, shmemory_size) == -1){
            perror("munmap failed in PARENT");
            exit(EXIT_FAILURE);
        }

        printf("Verify presence of shared memory:\n");
        system("ls -la /dev/shm/ | grep shared_memory");

        if(shm_unlink(shmemory_name) == -1){ //REMOVE fd shared memory, se non faccio la chiamata rimarrà l'oogetto in ~./shm
            perror("shm_unlink failed"); 
            exit(EXIT_FAILURE); 
        }

        printf("After shm_unlink():\n");
        system("ls -la /dev/shm/ | grep shared_memory || echo \"Shared memory removed\"");

    }
    
    return EXIT_SUCCESS;
}


/* MAYBE TO-DO:
Aggiungere sincronizzazione esplicita:

Implementare semafori POSIX (sem_open(), sem_wait(), sem_post())
Sostituire il sleep() con meccanismi di sincronizzazione appropriati
Creare più processi figli:

Modificare il codice per creare multipli processi che interagiscono con la stessa memoria condivisa
Implementare un esempio più complesso:

Creazione di un buffer circolare nella memoria condivisa
Implementare un modello producer/consumer
Monitorare la memoria condivisa:

Verificare la presenza dell'oggetto in shm durante l'esecuzione
Osservare cosa accade se non chiami shm_unlink()

*/