#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

// Variabili condivise (DEVONO essere volatile)
volatile int turn;           // Di chi è il turno
volatile bool flag[2];       // Intenzione di entrare

// Contatore condiviso per testare la mutua esclusione
volatile int shared_counter = 0;

void enter_CriticalSection(int i) {
    int j = 1 - i;              // L'altro processo (0→1, 1→0)
    
    flag[i] = true;             // STEP 1: Dichiaro intenzione di entrare
    turn = j;                   // STEP 2: Cedo cortesemente il turno all'altro
    
    // STEP 3: Attendo SE l'altro vuole entrare E è il suo turno
    while (flag[j] && turn == j) {
        // Busy waiting - controllo attivo
    }
    
    printf("Processo %d: ENTRATO in sezione critica\n", i);
}

void exit_CriticalSection(int i) {
    printf("Processo %d: ESCO da sezione critica\n", i);
    flag[i] = false;            // Rilascio l'intenzione di entrare
}

// Funzione eseguita dai thread
void* process_function(void* arg) {
    int process_id = *(int*)arg;
    
    for (int iter = 0; iter < 5; iter++) {
        // --- SEZIONE NON CRITICA ---
        printf("Processo %d: lavoro non critico (iter %d)\n", process_id, iter);
        usleep(100000);  // Simula lavoro
        
        // --- ENTRA IN SEZIONE CRITICA ---
        enter_CriticalSection(process_id);
        
        // --- SEZIONE CRITICA ---
        int temp = shared_counter;
        printf("Processo %d: leggo counter = %d\n", process_id, temp);
        usleep(50000);   // Simula operazione critica
        shared_counter = temp + 1;
        printf("Processo %d: scrivo counter = %d\n", process_id, shared_counter);
        
        // --- ESCE DA SEZIONE CRITICA ---
        exit_CriticalSection(process_id);
        
        usleep(200000);  // Pausa tra iterazioni
    }
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int id1 = 0, id2 = 1;
    
    // Inizializzazione
    turn = 0;
    flag[0] = false;
    flag[1] = false;
    
    printf("=== SOLUZIONE DI PETERSON ===\n");
    printf("Test con 2 processi e sezione critica\n\n");
    
    // Creazione thread
    pthread_create(&thread1, NULL, process_function, &id1);
    pthread_create(&thread2, NULL, process_function, &id2);
    
    // Attesa terminazione
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("\n=== RISULTATO FINALE ===\n");
    printf("Counter finale: %d (dovrebbe essere 10)\n", shared_counter);
    
    if (shared_counter == 10) {
        printf("✓ MUTUA ESCLUSIONE GARANTITA!\n");
    } else {
        printf("✗ Race condition rilevata!\n");
    }
    
    return 0;
}