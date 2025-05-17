#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <semaphore.h> // Per i semafori POSIX

/*

Differenze chiave rispetto alla SHARED MEMORY
Comunicazione esplicita: I processi devono inviare/ricevere messaggi esplicitamente

Passaggio dati: I dati vengono copiati tra i processi invece di essere condivisi

Sincronizzazione: I messaggi forniscono sincronizzazione implicita (il ricevitore aspetta)

Paradigma diverso: La comunicazione avviene tramite scambio di messaggi anziché accesso a memoria comune

La message queue è particolarmente utile quando vuoi un meccanismo di comunicazione più strutturato con 
sincronizzazione integrata

Made by: Schianchi Federico 17/05/2025
gcc (or g++) -o <nome> <this.file> -lrt (per le librerie POSIX real-time linkata esplicitamente)

*/

typedef struct {
    int counter;
    char text[100];
} message_type;

int main(int argc, char const *argv[])
{
    const char *queue_name = "/m_message_queue";
    const char *sem_parent_name = "/sem_parent_done"; // Semaforo per quando il parent ha inviato
    const char *sem_child_name = "/sem_child_done";   // Semaforo per quando il child ha risposto
    
    // Creare i semafori
    sem_t *sem_parent = sem_open(sem_parent_name, O_CREAT, 0666, 0);
    sem_t *sem_child = sem_open(sem_child_name, O_CREAT, 0666, 0);
    
    if (sem_parent == SEM_FAILED || sem_child == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    
    // Prepara gli attributi della queue
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(message_type);
    attr.mq_curmsgs = 0;

    mqd_t mq = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid < 0){ perror("fork failed"); exit(EXIT_FAILURE); }

    if(pid == 0){ // CHILD PROCESS
        message_type msg_received;
        message_type msg_to_send;

        // Aspetta che il parent invii un messaggio
        sem_wait(sem_parent);
        
        // Ora è sicuro leggere
        if (mq_receive(mq, (char *)&msg_received, sizeof(message_type), NULL) == -1) {
            perror("mq_receive in child failed");
            exit(EXIT_FAILURE);
        }

        printf("CHILD: Received message - counter: %d, text: '%s'\n",
                msg_received.counter, msg_received.text);

        msg_to_send.counter = msg_received.counter + 1;
        strncpy(msg_to_send.text, "Messaggio dal CHILD", sizeof(msg_to_send.text)-1);

        if (mq_send(mq, (char *)&msg_to_send, sizeof(message_type), 0) == -1){
            perror("mq_send in child failed");
            exit(EXIT_FAILURE);
        }

        // Segnala al parent che ha risposto
        sem_post(sem_child);
        
        // Pulizia
        mq_close(mq);
        sem_close(sem_parent);
        sem_close(sem_child);
        exit(EXIT_SUCCESS);
        
    } else { // PARENT PROCESS
        message_type msg_received;
        message_type msg_to_send;

        msg_to_send.counter = 0;
        strncpy(msg_to_send.text, "Init message", sizeof(msg_to_send.text)-1);

        if (mq_send(mq, (char *)&msg_to_send, sizeof(message_type), 0) == -1) {
            perror("mq_send in parent failed");
            exit(EXIT_FAILURE);
        }

        // Segnala al figlio che il messaggio è pronto
        sem_post(sem_parent);
        
        // Aspetta che il figlio risponda
        sem_wait(sem_child);

        // Ora è sicuro leggere la risposta
        if (mq_receive(mq, (char *)&msg_received, sizeof(message_type), NULL) == -1) {
            perror("mq_receive in parent failed");
            exit(EXIT_FAILURE);
        }

        printf("PARENT: Received message - counter: %d, text; '%s'\n",
                msg_received.counter, msg_received.text);

        wait(NULL);

        // Pulizia
        mq_close(mq);
        sem_close(sem_parent);
        sem_close(sem_child);
        
        // Rimuovi i semafori e la queue
        sem_unlink(sem_parent_name);
        sem_unlink(sem_child_name);
        mq_unlink(queue_name);
    }

    return EXIT_SUCCESS;
}


