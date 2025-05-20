#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>



#define BUFFER_SIZE 10
#define NUM_PRODUCERS 10
#define NUM_CONSUMERS 10
#define ITEMS_PER_PRODUCER 497
#define TOTAL_ITEMS (NUM_PRODUCERS * ITEMS_PER_PRODUCER)

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
pthread_mutex_t mutex;
pthread_cond_t empty;
pthread_cond_t full;

// Contatori di thread bloccati (per rilevare deadlock)
int blocked_producers = 0;
int blocked_consumers = 0;
pthread_mutex_t counter_mutex;

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
    sprintf(buffer + strlen(buffer), ".%03ld", tv.tv_usec / 1000);
    return buffer;
}

// Funzione per contare elementi nel buffer
int buffer_count() {
    return (in - out + BUFFER_SIZE) % BUFFER_SIZE;
}

// Struct per passare un ID a ciascun thread
typedef struct {
    int id;
} ThreadParams;

void* producer(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;
    int id = params->id;
    
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = (id * ITEMS_PER_PRODUCER) + i;

        // INIZIO SEZIONE CRITICA (buffer)
        pthread_mutex_lock(&mutex);
        printf("[%s] Producer %d: tentativo di produrre item %d [buffer: %d/%d]\n", 
               get_timestamp(), id, item, buffer_count(), BUFFER_SIZE);
               
        // Controllo buffer pieno
        if ((in + 1) % BUFFER_SIZE == out) {
            // INIZIO SEZIONE CRITICA (contatori)
            pthread_mutex_lock(&counter_mutex);
            blocked_producers++;
            pthread_mutex_unlock(&counter_mutex);
            // FINE SEZIONE CRITICA (contatori)
            
            printf("[%s] Producer %d: BLOCCATO - buffer pieno [P_blocked: %d, C_blocked: %d]\n", 
                   get_timestamp(), id, blocked_producers, blocked_consumers);
                   
            pthread_cond_wait(&empty, &mutex);  // Rilascia mutex durante l'attesa
            
            // INIZIO SEZIONE CRITICA (contatori)
            pthread_mutex_lock(&counter_mutex);
            blocked_producers--;
            pthread_mutex_unlock(&counter_mutex);
            // FINE SEZIONE CRITICA (contatori)
            
            printf("[%s] Producer %d: SBLOCCATO dopo attesa [P_blocked: %d, C_blocked: %d]\n", 
                   get_timestamp(), id, blocked_producers, blocked_consumers);
        }
        
        // Modifica del buffer - operazione critica
        buffer[in] = item;
        printf("[%s] Producer %d: prodotto item %d alla posizione %d [buffer: %d/%d]\n", 
               get_timestamp(), id, item, in, buffer_count()+1, BUFFER_SIZE);
        in = (in + 1) % BUFFER_SIZE;  // Modifica dell'indice - operazione critica
        
        // Segnala consumer
        printf("[%s] Producer %d: segnala consumer\n", get_timestamp(), id);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        // FINE SEZIONE CRITICA (buffer)
        
        usleep(rand() % 50000);
    }
    printf("[%s] Producer %d: TERMINATO\n", get_timestamp(), id);
    free(params);
    return NULL;
}

void* consumer(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;
    int id = params->id;
    int items_to_consume = TOTAL_ITEMS / NUM_CONSUMERS;
    
    for (int i = 0; i < items_to_consume; i++) {
        // INIZIO SEZIONE CRITICA (buffer)
        pthread_mutex_lock(&mutex);
        printf("[%s] Consumer %d: tentativo di consumare [buffer: %d/%d]\n", 
               get_timestamp(), id, buffer_count(), BUFFER_SIZE);
               
        // Controllo buffer vuoto
        if (in == out) {
            // INIZIO SEZIONE CRITICA (contatori)
            pthread_mutex_lock(&counter_mutex);
            blocked_consumers++;
            pthread_mutex_unlock(&counter_mutex);
            // FINE SEZIONE CRITICA (contatori)
            
            printf("[%s] Consumer %d: BLOCCATO - buffer vuoto [P_blocked: %d, C_blocked: %d]\n", 
                   get_timestamp(), id, blocked_producers, blocked_consumers);
                   
            pthread_cond_wait(&full, &mutex);  // Rilascia mutex durante l'attesa
            
            // INIZIO SEZIONE CRITICA (contatori)
            pthread_mutex_lock(&counter_mutex);
            blocked_consumers--;
            pthread_mutex_unlock(&counter_mutex);
            // FINE SEZIONE CRITICA (contatori)
            
            printf("[%s] Consumer %d: SBLOCCATO dopo attesa [P_blocked: %d, C_blocked: %d]\n", 
                   get_timestamp(), id, blocked_producers, blocked_consumers);
        }
        
        // Accesso al buffer - operazione critica
        int item = buffer[out];
        printf("[%s] Consumer %d: consumato item %d dalla posizione %d [buffer: %d/%d]\n", 
               get_timestamp(), id, item, out, buffer_count()-1, BUFFER_SIZE);
        out = (out + 1) % BUFFER_SIZE;  // Modifica dell'indice - operazione critica
        
        // Segnala producer
        printf("[%s] Consumer %d: segnala producer\n", get_timestamp(), id);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        // FINE SEZIONE CRITICA (buffer)
        
        usleep(rand() % 50000);
    }
    printf("[%s] Consumer %d: TERMINATO\n", get_timestamp(), id);
    free(params);
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    
    // Inizializzazione delle variabili di sincronizzazione
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&counter_mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    
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
    
    // Attesa del completamento di tutti i thread
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
        printf("[%s] Producer %d completato\n", get_timestamp(), i);
    }
    
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
        printf("[%s] Consumer %d completato\n", get_timestamp(), i);
    }
    
    // Pulizia delle risorse
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&counter_mutex);
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    
    //=============== TO DO =============== (thread orfani a causa dei thread consumer bloccati in attesa)

// Verifica che non ci siano consumer ancora bloccati
    if (blocked_consumers > 0) {
        printf("[%s] ATTENZIONE: %d consumer ancora bloccati alla fine dell'esecuzione!\n", 
            get_timestamp(), blocked_consumers);
            
        // Invia un segnale broadcast per sbloccare tutti i consumer
        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&full);
        pthread_mutex_unlock(&mutex);
        
        // Attendi un attimo che i thread si sblocchino
        usleep(200000);
        
        printf("[%s] Tentativo di sblocco forzato dei consumer completato.\n", get_timestamp());
    }




    // Resoconto finale
    printf("\n\n=========== RESOCONTO FINALE ===========\n");
    printf("Numero totale di blocchi producer: %d\n", total_producer_blocks);
    printf("Numero totale di blocchi consumer: %d\n", total_consumer_blocks);
    printf("Numero massimo di producer bloccati contemporaneamente: %d\n", max_producers_blocked);
    printf("Numero massimo di consumer bloccati contemporaneamente: %d\n", max_consumers_blocked);
    printf("Produttività media: %.2f elementi/blocco\n", 
           (total_producer_blocks + total_consumer_blocks > 0) ? 
           (float)TOTAL_ITEMS/(total_producer_blocks + total_consumer_blocks) : 0);

    if (total_producer_blocks == 0 && total_consumer_blocks == 0) {
        printf("Nessun blocco rilevato: esecuzione ottimale!\n");
    } else if (max_producers_blocked == NUM_PRODUCERS && max_consumers_blocked == NUM_CONSUMERS) {
        printf("ATTENZIONE: Rilevata possibile situazione di deadlock completo durante l'esecuzione\n");
    }
    printf("=========================================\n");
    
    printf("[%s] Programma terminato con successo\n", get_timestamp());


    return 0;
}