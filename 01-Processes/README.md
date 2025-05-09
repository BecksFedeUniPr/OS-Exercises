# Processes (UK)

A process is an instance of a program in execution. It contains the program code and its current activity. Each process has its own memory space, system resources, and security attributes. Processes are fundamental units of execution in modern operating systems, providing isolation, resource management, and concurrency.

ICP: Shared memory, Message passing, Pipe

Socket, RPC


## Shared Memory

Shared memory is a method of inter-process communication (IPC) that allows multiple processes to access the same region of memory. This enables fast data exchange between processes, as they can read and write to the shared memory segment directly. However, it requires synchronization mechanisms (like semaphores or mutexes) to prevent data corruption due to concurrent access.


## Message Passing

Message passing is a communication method where processes send and receive messages to exchange data. This approach is more structured than shared memory, as it allows for better control over data flow and synchronization. Message passing can be implemented using various IPC mechanisms, such as message queues, sockets, or remote procedure calls (RPCs). It is suitable for distributed systems and applications where processes may not share the same memory space.

# Pipe

A pipe is a unidirectional communication channel that allows one process to send data to another. Pipes are commonly used for inter-process communication in Unix-like operating systems. They can be either anonymous (used for communication between related processes) or named (used for communication between unrelated processes). Pipes are typically used for simple data transfer and can be easily implemented using system calls like `pipe()`, `read()`, and `write()`.

# Socket

Sockets are a more advanced IPC mechanism that allows processes to communicate over a network. They provide a flexible and powerful way to exchange data between processes, whether they are on the same machine or across different machines. Sockets can be used for both connection-oriented (TCP) and connectionless (UDP) communication. They are widely used in client-server applications and distributed systems.

# Remote Procedure Call (RPC)

RPC is a communication method that allows a program to execute a procedure (or function) on a remote server as if it were a local call. This abstraction simplifies the process of building distributed applications, as developers can focus on the functionality rather than the underlying communication details. RPC can be implemented using various protocols, such as HTTP, gRPC, or SOAP.

# Processi (ITA)

Un processo è un'istanza di un programma in esecuzione. Contiene il codice del programma e la sua attività corrente. Ogni processo ha il proprio spazio di memoria, risorse di sistema e attributi di sicurezza. I processi sono unità fondamentali di esecuzione nei sistemi operativi moderni, che forniscono isolamento, gestione delle risorse e concorrenza.

ICP: Shared memory, Message passing, Pipe

Socket, RPC


## Shared Memory

Shared memory è un metodo di comunicazione inter-processo (IPC) che consente a più processi di accedere alla stessa regione di memoria. Ciò permette uno scambio rapido di dati tra processi, poiché possono leggere e scrivere direttamente nel segmento di memoria condivisa. Tuttavia, richiede meccanismi di sincronizzazione (come semafori o mutex) per prevenire la corruzione dei dati dovuta all'accesso concorrente.


## Message Passing

Message passing è un metodo di comunicazione in cui i processi inviano e ricevono messaggi per scambiare dati. Questo approccio è più strutturato rispetto alla memoria condivisa, poiché consente un migliore controllo del flusso di dati e della sincronizzazione. Il message passing può essere implementato utilizzando vari meccanismi IPC, come code di messaggi, socket o chiamate di procedura remota (RPC). È adatto per sistemi distribuiti e applicazioni in cui i processi potrebbero non condividere lo stesso spazio di memoria.

# Pipe

Una pipe è un canale di comunicazione unidirezionale che consente a un processo di inviare dati a un altro. Le pipe sono comunemente utilizzate per la comunicazione inter-processo nei sistemi operativi Unix-like. Possono essere anonime (utilizzate per la comunicazione tra processi correlati) o nominate (utilizzate per la comunicazione tra processi non correlati). Le pipe sono tipicamente utilizzate per il trasferimento semplice di dati e possono essere facilmente implementate utilizzando chiamate di sistema come `pipe()`, `read()` e `write()`.

# Socket

I socket sono un meccanismo IPC più avanzato che consente ai processi di comunicare attraverso una rete. Forniscono un modo flessibile e potente per scambiare dati tra processi, sia che si trovino sulla stessa macchina sia su macchine diverse. I socket possono essere utilizzati sia per comunicazioni orientate alla connessione (TCP) che senza connessione (UDP). Sono ampiamente utilizzati nelle applicazioni client-server e nei sistemi distribuiti.

# Remote Procedure Call (RPC)

RPC è un metodo di comunicazione che consente a un programma di eseguire una procedura (o funzione) su un server remoto come se fosse una chiamata locale. Questa astrazione semplifica il processo di costruzione di applicazioni distribuite, poiché gli sviluppatori possono concentrarsi sulla funzionalità piuttosto che sui dettagli di comunicazione sottostanti. RPC può essere implementato utilizzando vari protocolli, come HTTP, gRPC o SOAP.