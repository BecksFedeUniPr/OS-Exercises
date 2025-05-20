// This file implements the buffer used for communication between producers and consumers. 
// It includes functions for adding and removing items from the buffer, as well as synchronization mechanisms.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

/*BUFFER CIRCOLARE THREAD SAFE SYNCRONIZZATO prevenendo:

- Overflow del buffer (un producer che cerca di scrivere in un buffer pieno)
- Underflow del buffer (un consumer che cerca di leggere da un buffer vuoto)
- Race condition nell'accesso agli indici e al buffer

*/
typedef struct {
    int buffer[BUFFER_SIZE]; //real buffer
    int in; //indice dove il producer insierà l'elemento "read pointer"
    int out; //indice dove il consumer preleverà un'elemento "write pointer"
    sem_t empty; 
    /*
    Semaforo contatore che tiene traccia degli slot vuoti;
    Inizializzato a BUFFER_SIZE (10).
    I producer attendono su questo semaforo prima di inserire un elemento.
    */
    sem_t full;
    /*
    Semaforo contatore che tiene traccia degli slot pieni.
    Inizializzato a BUFFER_SIZE(0).
    I consumer attendono su questo semaforo prima di prelevare un elemento. 
    */
    pthread_mutex_t mutex;
    /*
    Mutex che protegge l'accesso alla sezione critica,
    (il buffer stesso e gli indici). 
    Impedisce che più thread modifichino contemporaneamente il buffer.
    */

} Buffer;

Buffer* create_buffer() {
    Buffer* b = (Buffer*)malloc(sizeof(Buffer));
    b->in = 0;
    b->out = 0;
    sem_init(&b->empty, 0, BUFFER_SIZE);
    sem_init(&b->full, 0, 0);
    pthread_mutex_init(&b->mutex, NULL);
    return b;
}

void destroy_buffer(Buffer* b) {
    sem_destroy(&b->empty);
    sem_destroy(&b->full);
    pthread_mutex_destroy(&b->mutex);
    free(b);
}

void add_item(Buffer* b, int item) {
    sem_wait(&b->empty);
    pthread_mutex_lock(&b->mutex);
    
    b->buffer[b->in] = item;
    b->in = (b->in + 1) % BUFFER_SIZE;
    
    pthread_mutex_unlock(&b->mutex);
    sem_post(&b->full);
}

int remove_item(Buffer* b) {
    sem_wait(&b->full);
    pthread_mutex_lock(&b->mutex);
    
    int item = b->buffer[b->out];
    b->out = (b->out + 1) % BUFFER_SIZE;
    
    pthread_mutex_unlock(&b->mutex);
    sem_post(&b->empty);
    
    return item;
}