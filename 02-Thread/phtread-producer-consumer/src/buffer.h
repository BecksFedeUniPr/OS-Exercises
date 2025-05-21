#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>

#define BUFFER_SIZE 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    pthread_cond_t empty;
    pthread_cond_t full;
    pthread_mutex_t mutex;
    int producers_done;
}Buffer;

Buffer* create_buffer();
void destroy_buffer(Buffer* b);
void add_item(Buffer* b, int item);
int remove_item(Buffer* b);
void signal_producers_done(Buffer* b);
int buffer_count(Buffer* b);


#endif 

/*

Effetto delle diverse dimensioni del buffer
Buffer size = 10 (piccolo)
    - Blocchi più frequenti: I producer si bloccheranno spesso perché il buffer si riempie velocemente
    - Context switch frequenti: Rapida alternanza tra producer e consumer
    - Parallelismo limitato: I thread lavoreranno in modo più serializzato
    - Statistiche: Numero elevato di blocchi di producer/consumer
    - Memoria: Utilizzo minimo di memoria

Buffer size = 256 (grande)
    - Blocchi meno frequenti: I producer possono produrre a lungo senza bloccarsi
    - Context switch ridotti: Meno cambi di contesto tra thread
    - Maggiore parallelismo: I producer possono avanzare molto prima che i consumer li raggiungano
    - Statistiche: Numero ridotto di blocchi, specialmente per i producer
    - Memoria: Maggiore utilizzo di memoria (non critico in questo caso)

*/