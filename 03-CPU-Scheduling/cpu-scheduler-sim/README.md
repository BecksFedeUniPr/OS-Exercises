# CPU Scheduler Simulator

Un simulatore di algoritmi di scheduling della CPU implementato in C. Questo progetto implementa e confronta diversi algoritmi di scheduling della CPU.

## Algoritmi Implementati

- **FCFS** (First-Come First-Served) - Non preemptive
- **SJF** (Shortest Job First) - Non preemptive
- **SRTF** (Shortest Remaining Time First) - Preemptive
- **RR** (Round Robin) - Preemptive con time quantum
- **Priority** (Priority Scheduling) - Versioni preemptive e non preemptive

## Prerequisiti

- GCC (GNU Compiler Collection)
- Make

## Compilazione

Prima di iniziare, assicurati che l'indentazione nel Makefile usi tab e non spazi:

```bash
# Correggere il Makefile
sed -i '' 's/^    /\t/g' Makefile
```

Poi compila il progetto:

```bash
# Compila il programma principale
make all

# Compila gli esempi
make examples

# Compila i test
make test
```

## Utilizzo del Simulatore

### Programma Principale

```bash
# Esecuzione del programma principale interattivo
./bin/cpu_scheduler
```
Nel programma interattivo:
1. Inserisci il numero di processi
2. Per ogni processo, inserisci: arrival time, burst time, priority
3. Seleziona l'algoritmo di scheduling da utilizzare
4. Per Round Robin, inserisci il time quantum
5. Per Priority Scheduling, scegli tra preemptive o non preemptive

### Demo di Esempio

```bash
# Esegui la demo con esempi predefiniti per tutti gli algoritmi
./bin/basic_demo
```

Questo eseguirà dei casi di esempio predefiniti per ciascun algoritmo.

### Utilizzo con File di Input

```bash
# Carica ed esegui l'esempio 1
./bin/file_loader examples/data/process_set1.txt

# Carica ed esegui l'esempio 2 (più complesso)
./bin/file_loader examples/data/process_set2.txt

# Carica ed esegui l'esempio 3 (per test speciali)
./bin/file_loader examples/data/process_set3.txt
```

I file di input sono nel formato:
```
[numero di processi]
[id] [arrival_time] [burst_time] [priority]
...
```

### Test

```bash
# Esegui i test per FCFS
./bin/test_fcfs
```

## Struttura del progetto

- **src/**
  - **algorithms/**: Implementazioni degli algoritmi di scheduling
  - **utils/**: Utilities (process, queue, metrics)
  - **main.c**: Programma principale interattivo

- **include/**
  - **common.h**: Definizioni e costanti comuni

- **examples/**
  - **basic_demo.c**: Demo di tutti gli algoritmi
  - **file_loader.c**: Caricamento processi da file
  - **data/**: Set di dati di esempio
    - **process_set1.txt**: Set di base (4 processi)
    - **process_set2.txt**: Set complesso (12 processi)
    - **process_set3.txt**: Set per casi speciali (10 processi)

- **tests/**
  - **test_fcfs.c**: Test per FCFS

## Metriche Calcolate

Per ogni algoritmo di scheduling, il simulatore calcola e visualizza:
- **Average Waiting Time**: Tempo medio atteso dai processi
- **Average Turnaround Time**: Tempo medio di completamento totale
- **Average Response Time**: Tempo medio di prima risposta
- **Throughput**: Numero di processi completati per unità di tempo

## Esempi

### Esempio 1: FCFS con input interattivo

```
$ ./bin/cpu_scheduler
CPU Scheduling Simulator
Enter number of processes (max 10): 3
Process 1:
  Arrival time: 0
  Burst time: 10
  Priority: 3
Process 2:
  Arrival time: 1
  Burst time: 4
  Priority: 1
Process 3:
  Arrival time: 2
  Burst time: 6
  Priority: 2

Select Scheduling Algorithm:
1. FCFS (First-Come First-Served)
2. SJF (Shortest Job First)
3. SRTF (Shortest Remaining Time First)
4. RR (Round Robin)
5. Priority Scheduling
Enter your choice: 1
```

### Esempio 2: Utilizzo di un file di dati predefinito

```
$ ./bin/file_loader examples/data/process_set2.txt
Loaded 12 processes from file examples/data/process_set2.txt

Process | Arrival | Burst | Priority
--------|---------|-------|--------
P1      | 0       | 10    | 3
P2      | 2       | 5     | 1
P3      | 4       | 8     | 4
...
```

## Risoluzione Problemi

- Se il Makefile produce errori, assicurati che l'indentazione usi tab e non spazi
- Se un algoritmo fallisce, verifica che i processi abbiano valori ragionevoli (burst_time > 0)

## Autore

Federico Schianchi