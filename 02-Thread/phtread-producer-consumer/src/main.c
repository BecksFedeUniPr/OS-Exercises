#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#include "buffer.h"


#define NUM_PRODUCERS 20
#define NUM_CONSUMERS 50
#define ITEMS_PER_PRODUCER 500
#define TOTAL_ITEMS (NUM_PRODUCERS * ITEMS_PER_PRODUCER)

Buffer* buffer;

// Contatori di thread bloccati (per rilevare deadlock)
int blocked_producers = 0;
int blocked_consumers = 0;
pthread_mutex_t counter_mutex;

// Struct per passare un ID a ciascun thread
typedef struct {
    int id;
} ThreadParams;

// Statistiche
int total_producer_blocks = 0;
int total_consumer_blocks = 0;
int max_producers_blocked = 0;
int max_consumers_blocked = 0;

// Funzione per ottenere timestamp
char* get_timestamp() {
    static char buffer[64];
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* tm_info = localtime(&tv.tv_sec);
    strftime(buffer, 26, "%H:%M:%S", tm_info);
    sprintf(buffer + strlen(buffer), ".%03d", (int)tv.tv_usec / 1000);
    return buffer;
}

void* producer(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;
    int id = params->id;
    
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = (id * ITEMS_PER_PRODUCER) + i;

        // INIZIO SEZIONE CRITICA (buffer)
        printf("[%s] Producer %d: tentativo di produrre item %d [buffer: %d/%d]\n", 
               get_timestamp(), id, item, buffer_count(buffer), BUFFER_SIZE);
               
        // Controllo buffer pieno (fuori dalla sezione critica)
        pthread_mutex_lock(&counter_mutex);
        blocked_producers++;
        if (blocked_producers > max_producers_blocked)
            max_producers_blocked = blocked_producers;
        total_producer_blocks++;
        pthread_mutex_unlock(&counter_mutex);
        
        // Aggiungi item al buffer (gestisce internamente il mutex)
        add_item(buffer, item);
        
        // Dopo l'aggiunta
        pthread_mutex_lock(&counter_mutex);
        blocked_producers--;
        pthread_mutex_unlock(&counter_mutex);
        
        printf("[%s] Producer %d: prodotto item %d\n", get_timestamp(), id, item);
        
        usleep(rand() % 50000); //velocità producer
    }

    printf("[%s] Producer %d: TERMINATO\n", get_timestamp(), id);
    free(params);
    return NULL;
}

void* consumer(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;
    int id = params->id;
    int items_consumed = 0;
    
    while (1) {
        // Prima di tentare di accedere al buffer
        printf("[%s] Consumer %d: tentativo di consumare\n", get_timestamp(), id);
        
        // Controllo buffer vuoto (fuori dalla sezione critica)
        pthread_mutex_lock(&counter_mutex);
        blocked_consumers++;
        if (blocked_consumers > max_consumers_blocked)
            max_consumers_blocked = blocked_consumers;
        total_consumer_blocks++;
        pthread_mutex_unlock(&counter_mutex);
        
        // Rimuovi item dal buffer (gestisce internamente il mutex)
        int item = remove_item(buffer);
        
        // Dopo la rimozione
        pthread_mutex_lock(&counter_mutex);
        blocked_consumers--;
        pthread_mutex_unlock(&counter_mutex);
        
        // Verifica se abbiamo terminato
        if (item == -1) {
            printf("[%s] Consumer %d: terminazione segnalata\n", get_timestamp(), id);
            break;
        }

        items_consumed++;
        printf("[%s] Consumer %d: consumato item %d\n", get_timestamp(), id, item);
        
        usleep(rand() % 50000); //velocità consumer
    }

    printf("[%s] Consumer %d: TERMINATO (consumati %d items)\n", 
       get_timestamp(), id, items_consumed);
    free(params);
    return NULL;
}

int main() {
    
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    // Inizializzazione delle variabili
    pthread_mutex_init(&counter_mutex, NULL);
    buffer = create_buffer();  // Crea il buffer

    srand(time(NULL));
    printf("[%s] Avvio del programma con %d producer e %d consumer\n", 
           get_timestamp(), NUM_PRODUCERS, NUM_CONSUMERS);

    // Creazione dei thread producer
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ThreadParams* params = malloc(sizeof(ThreadParams));
        params->id = i;
        pthread_create(&producers[i], NULL, producer, params);
        printf("[%s] Creato Producer %d\n", get_timestamp(), i);
    }
    
    // Creazione dei thread consumer
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        ThreadParams* params = malloc(sizeof(ThreadParams));
        params->id = i;
        pthread_create(&consumers[i], NULL, consumer, params);
        printf("[%s] Creato Consumer %d\n", get_timestamp(), i);
    }
    
    // Attesa del completamento di tutti i producer
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
        printf("[%s] Producer %d completato\n", get_timestamp(), i);
    }

    // IMPORTANTE: Segnala che i producer hanno finito
    signal_producers_done(buffer);
    
    // Ora attendi i consumer
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
        printf("[%s] Consumer %d completato\n", get_timestamp(), i);
    }
    
    // Pulizia delle risorse
    destroy_buffer(buffer);
    pthread_mutex_destroy(&counter_mutex);


     // Resoconto finale
    printf("\n\n=========== RESOCONTO FINALE ===========\n");
    printf("Numero totale di blocchi producer: %d\n", total_producer_blocks);
    printf("Numero totale di blocchi consumer: %d\n", total_consumer_blocks);
    printf("Numero massimo di producer bloccati contemporaneamente: %d\n", max_producers_blocked);
    printf("Numero massimo di consumer bloccati contemporaneamente: %d\n", max_consumers_blocked);
    
    printf("=========================================\n");
    printf("[%s] Programma terminato con successo\n", get_timestamp());
    return 0;
}
