# CPU schedluing algorithms (UK)

CPU scheduling is the process of determining which process or thread should be allocated CPU time at any given moment. The goal of CPU scheduling is to maximize CPU utilization, minimize response time, and ensure fairness among processes.

## FCFS (First-Come, First-Served)
FCFS (First-Come, First-Served) is the simplest CPU scheduling algorithm where processes are executed in the order they arrive in the ready queue. It is non-preemptive, meaning once a process gets the CPU, it keeps it until it completes or blocks for I/O. While easy to implement, FCFS can lead to the "convoy effect" where short processes are forced to wait behind long processes, resulting in higher average waiting times and lower CPU efficiency.

## SJF (Shortest Job First)
SJF (Shortest Job First) is a CPU scheduling algorithm that selects the process with the smallest estimated execution time to run next. It can be either preemptive or non-preemptive. SJF minimizes average waiting time and is optimal for minimizing turnaround time. However, it can lead to starvation for longer processes if shorter processes keep arriving.

## SRTF (Shortest Remaining Time First)

SRTF (Shortest Remaining Time First) is the preemptive version of SJF, where the scheduler always chooses the process with the shortest remaining execution time. If a new process arrives with a shorter burst time than the remaining time of the currently running process, the current process is preempted. SRTF provides optimal average waiting time and is responsive to short processes. However, it suffers from the same drawbacks as SJF, including potential starvation of longer processes, and it incurs additional overhead due to frequent context switching and the need to continuously track remaining execution times. 

## RR (Round Robin)

RR (Round Robin) is a preemptive CPU scheduling algorithm that assigns a fixed time quantum to each process in the ready queue. When a process's time quantum expires, it is moved to the back of the queue, and the next process is given the CPU. This approach ensures fairness and responsiveness, making it suitable for time-sharing systems. However, if the time quantum is too small, it can lead to high context-switching overhead.

## Priority Scheduling

Priority scheduling is a CPU scheduling algorithm that assigns a priority level to each process. The process with the highest priority is selected for execution next. It can be preemptive or non-preemptive. While it allows for flexible scheduling based on process importance, it can lead to starvation for low-priority processes if high-priority processes keep arriving.

# Algoritmi di CPU scheduling (ITA)

Il CPU scheduling è il processo di determinazione di quale processo o thread debba ricevere tempo di CPU in un dato momento. L'obiettivo del CPU scheduling è massimizzare l'utilizzo della CPU, minimizzare i tempi di risposta e garantire equità tra i processi.

## FCFS (First-Come, First-Served)
FCFS (First-Come, First-Served) è l'algoritmo di CPU scheduling più semplice dove i processi vengono eseguiti nell'ordine in cui arrivano nella coda di attesa. È non-preemptive, il che significa che una volta che un processo ottiene la CPU, la mantiene fino al completamento o fino a quando non si blocca per operazioni di I/O. Sebbene sia facile da implementare, FCFS può portare all'"effetto convoglio" dove processi brevi sono costretti ad attendere dietro processi lunghi, risultando in tempi di attesa medi più elevati e minore efficienza della CPU.

## SJF (Shortest Job First)
SJF (Shortest Job First) è un algoritmo di CPU scheduling che seleziona il processo con il tempo di esecuzione stimato più breve per l'esecuzione successiva. Può essere preemptive o non-preemptive. SJF minimizza il tempo medio di attesa ed è ottimale per minimizzare il tempo di completamento. Tuttavia, può portare alla "fame" per i processi più lunghi se continuano ad arrivare processi più brevi.

## SRTF (Shortest Remaining Time First)
SRTF (Shortest Remaining Time First) è la versione preemptive di SJF, dove lo scheduler sceglie sempre il processo con il minor tempo di esecuzione rimanente. Se arriva un nuovo processo con un tempo di burst più breve rispetto al tempo rimanente del processo in esecuzione, il processo attuale viene interrotto. SRTF fornisce un tempo medio di attesa ottimale ed è reattivo ai processi brevi. Tuttavia, soffre degli stessi svantaggi di SJF, inclusa la potenziale "fame" per i processi più lunghi, e comporta un overhead aggiuntivo a causa dei frequenti cambi di contesto e della necessità di monitorare continuamente i tempi di esecuzione rimanenti.

## RR (Round Robin)
RR (Round Robin) è un algoritmo di CPU scheduling preemptive che assegna un quanto di tempo fisso a ciascun processo nella coda di attesa. Quando il quanto di tempo di un processo scade, viene spostato in fondo alla coda e il processo successivo ottiene la CPU. Questo approccio garantisce equità e reattività, rendendolo adatto per sistemi time-sharing. Tuttavia, se il quanto di tempo è troppo piccolo, può portare a un elevato overhead di cambio di contesto.

## Priority Scheduling
Priority scheduling è un algoritmo di CPU scheduling che assegna un livello di priorità a ciascun processo. Il processo con la priorità più alta viene selezionato per l'esecuzione successiva. Può essere preemptive o non-preemptive. Mentre consente una pianificazione flessibile basata sull'importanza del processo, può portare alla "fame" per i processi a bassa priorità se continuano ad arrivare processi ad alta priorità.

