
# Semafori contantori senza busy waiting:

Semafori contatori: La tua implementazione attuale usa semafori contatori (non binari):

- `empty` inizializzato a `BUFFER_SIZE (10)`
- `full` inizializzato a `0`
I valori possono oscillare tra `0` e `BUFFER_SIZE`

1. Busy waiting: L'implementazione attuale già evita il busy waiting. Le funzioni standard POSIX `sem_wait` e `sem_post` sono implementate in modo che:

- Quando un thread chiama `sem_wait` su un semaforo con valore 0, il thread viene messo in uno stato di sleep dal kernel
- Il thread non consuma cicli CPU mentre è in attesa
- Il thread viene risvegliato solo quando un altro thread incrementa il semaforo con `sem_post`.
Questo è uno dei vantaggi principali dei semafori POSIX: il sistema operativo gestisce automaticamente le code di attesa dei thread, evitando che i thread in attesa consumino risorse inutilmente con busy waiting.

Se il buffer è pieno, i producer si bloccheranno su `sem_wait(&b->empty)` senza consumare CPU. Allo stesso modo, se il buffer è vuoto, i consumer si bloccheranno su `sem_wait(&b->full)` senza busy waiting.


# Buffer circolare thread-safe

Documentazione => commenti sul codice

# Funzione `pthread_mutex_init` e livello di astrazione

La riga `pthread_mutex_init(&b->mutex, NULL);` inizializza un mutex (mutual exclusion) nella struttura Buffer. Questa operazione si colloca a un livello intermedio di astrazione:

È una soluzione di livello intermedio
1. ### Componente Alto Livello:

    - La gestione effettiva della sincronizzazione (blocco dei thread, code di attesa, risveglio) è gestita dal kernel.
    - Non c'è busy waiting - il sistema operativo sospende i thread in attesa.
    - Non devi implementare manualmente le primitive di basso livello.

2. ### Componente Basso Livello:

    - Devi esplicitamente dichiarare e inizializzare il mutex.
    - Devi chiamare manualmente le funzioni lock/unlock.
    - Devi identificare le sezioni critiche del codice.
    - Devi gestire il ciclo di vita del mutex (init/destroy).

Il `NULL` come secondo parametro indica che stai utilizzando gli attributi predefiniti per il mutex. Potresti specificare attributi personalizzati (come tipo di mutex, condivisione tra processi, ecc.) usando una struttura `pthread_mutexattr_t`.

## Riassumendo

Cosa gestisci tu:

1. Dichiarare le strutture dati di sincronizzazione
2. Inizializzarle e distruggerle
3. Chiamare esplicitamente `sem_wait/sem_post` o `pthread_mutex_lock/unlock`
4. Identificare le sezioni critiche

Cosa gestisce il kernel automaticamente:

- Mettere i thread in sleep quando necessario
- Mantenerli in code di attesa
- Risvegliarli senza busy waiting
- Gestire i dettagli dell'implementazione di basso livello

È un compromesso efficace: hai il controllo preciso di cosa sincronizzare, ma deleghi l'implementazione complessa dei meccanismi di attesa al sistema operativo.