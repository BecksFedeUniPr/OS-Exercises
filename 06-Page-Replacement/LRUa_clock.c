#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10

// Struttura per rappresentare un frame
typedef struct {
    int page_number; 
    bool referenced;
} Frame;

// Variabili globali
Frame frames[MAX_FRAMES];
int numFrames;
int clock_pointer = 0;

// La tua funzione findVictim è corretta - mantengo la logica originale
int findVictim(){
    while(true){
        if(frames[clock_pointer].referenced == false){
            int victim = clock_pointer;
            clock_pointer = (clock_pointer+1) % numFrames; //movimento circolare dei frame
            return victim;
        } else {
            frames[clock_pointer].referenced = false;  // Seconda chance
            clock_pointer = (clock_pointer+1) % numFrames; //Movimento cricolare dei frame
        }
    }
}

// Cerca una pagina nei frame
int findPage(int page) {
    for (int i = 0; i < numFrames; i++) {
        if (frames[i].page_number == page) {
            frames[i].referenced = true;  // Marca come recentemente usata
            return i;
        }
    }
    return -1;
}

// Controlla se ci sono frame liberi 
int findFreeFrame() {
    for (int i = 0; i < numFrames; i++) {
        if (frames[i].page_number == -1) {
            return i;
        }
    }
    return -1;
}

// Stampa lo stato corrente
void printState() {
    printf("Frames: ");
    for (int i = 0; i < numFrames; i++) {
        if (frames[i].page_number == -1) {
            printf("[ - ]");
        } else {
            printf("[%d:%d]", frames[i].page_number, frames[i].referenced ? 1 : 0);
        }
    }
    printf(" | Clock→%d\n", clock_pointer);
}

// Gestisce l'accesso a una pagina [IMPORTANTE]
void accessPage(int page) {
    printf("\n→ Accesso pagina %d: ", page);
    
    // Controlla se la pagina è già in memoria
    int frame_idx = findPage(page);  //cerco pagina nel frame
    if (frame_idx != -1) {
        printf("HIT (frame %d)\n", frame_idx);
        printState();
        return;
    }
    
    printf("MISS ");
    
    // Cerca un frame libero
    int free_frame = findFreeFrame(); //controllo se esisotno frame liberi
    if (free_frame != -1) {
        printf("→ Carico in frame libero %d\n", free_frame);
        frames[free_frame].page_number = page;
        frames[free_frame].referenced = true;
    } else {
        // Usa l'algoritmo Second Chance per trovare vittima
        int victim_frame = findVictim();
        printf("→ Sostituisco pag.%d con pag.%d (frame %d)\n", 
               frames[victim_frame].page_number, page, victim_frame);
        frames[victim_frame].page_number = page;
        frames[victim_frame].referenced = true;
    }
    
    printState();
}

int main() {
    numFrames = 3;
    
    // Inizializza tutti i frame come vuoti
    for (int i = 0; i < numFrames; i++) {
        frames[i].page_number = -1;
        frames[i].referenced = false;
    }
    
    printf("=== SECOND CHANCE (Clock) - Approssimazione LRU ===\n");
    printf("Frame disponibili: %d\n", numFrames);
    printState();
    
    // Sequenza di test che dimostra il comportamento LRU-like
    int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5, 3, 1};
    int num_pages = sizeof(pages) / sizeof(pages[0]);
    
    for (int i = 0; i < num_pages; i++) {
        accessPage(pages[i]);
    }
    
    printf("\n=== DIMOSTRAZIONE: Comportamento simile a LRU ===\n");
    printf("- Le pagine con bit=1 sono state usate recentemente\n");
    printf("- L'algoritmo evita di rimuovere pagine recenti (LRU-like)\n");
    printf("- Molto più efficiente del vero LRU\n");
    
    return 0;
}