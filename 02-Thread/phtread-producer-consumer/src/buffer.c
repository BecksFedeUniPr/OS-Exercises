// This file implements the buffer used for communication between producers and consumers. 
// It includes functions for adding and removing items from the buffer, as well as synchronization mechanisms.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10

/*BUFFER CIRCOLARE THREAD SAFE SYNCRONIZZATO prevenendo:

- Overflow del buffer (un producer che cerca di scrivere in un buffer pieno)
- Underflow del buffer (un consumer che cerca di leggere da un buffer vuoto)
- Race condition nell'accesso agli indici e al buffer

*/
typedef struct {
    int buffer[BUFFER_SIZE]; //real buffer
    int in; //indice dove il producer insierà l'elemento "write pointer"
    int out; //indice dove il consumer preleverà un'elemento "read pointer"
    pthread_cond_t empty; 
    /*
    Condition variable su cui i producer attendono quando il buffer è pieno.
    I producer attendono su questa variabile quando non ci sono slot vuoti.
    */
    pthread_cond_t full;
    /*
    Condition variable su cui i consumer attendono quando il buffer è vuoto.
    I consumer attendono su questa variabile quando non ci sono elementi da consumare.
    */
    pthread_mutex_t mutex;
    /*
    Mutex che protegge l'accesso alla sezione critica,
    (il buffer stesso e gli indici). 
    Impedisce che più thread modifichino contemporaneamente il buffer.
    */
   int producers_done;

} Buffer;

int buffer_count(Buffer* b) {
    return (b->in - b->out + BUFFER_SIZE) % BUFFER_SIZE;
}

Buffer* create_buffer() {
    Buffer* b = (Buffer*)malloc(sizeof(Buffer));
    b->in = 0;
    b->out = 0;
    b->producers_done = 0; //flag
    pthread_cond_init(&b->empty, NULL);
    pthread_cond_init(&b->full, NULL);
    pthread_mutex_init(&b->mutex, NULL);
    return b;
}

void signal_producers_done(Buffer* b) {
    pthread_mutex_lock(&b->mutex);
    b->producers_done = 1;
    pthread_cond_broadcast(&b->full);
    pthread_mutex_unlock(&b->mutex);
}

void destroy_buffer(Buffer* b) {
    pthread_cond_destroy(&b->empty);
    pthread_cond_destroy(&b->full);
    pthread_mutex_destroy(&b->mutex);
    free(b);
}

void add_item(Buffer* b, int item) {
    
    pthread_mutex_lock(&b->mutex);
    
    //wait while buffer is full
    while ((b->in + 1) % BUFFER_SIZE == b->out) { 
        pthread_cond_wait(&b->empty, &b->mutex);
    }

    //add item
    b->buffer[b->in] = item;
    b->in = (b->in + 1) % BUFFER_SIZE;

    //signal consumer that there is a new element
    pthread_cond_signal(&b->full);
    pthread_mutex_unlock(&b->mutex);
    
}

int remove_item(Buffer* b) {

   pthread_mutex_lock(&b->mutex);
    
    // wait while buffer is empty
    while (b->in == b->out) {
        if(b->producers_done){
            pthread_mutex_unlock(&b->mutex);
            return -1; //no item left
        }
        pthread_cond_wait(&b->full, &b->mutex);
    }
    
    // Rimuovi item
    int item = b->buffer[b->out];
    b->out = (b->out + 1) % BUFFER_SIZE;
    
    //signal producer that there is new space
    pthread_cond_signal(&b->empty);
    pthread_mutex_unlock(&b->mutex);
    
    return item;
}