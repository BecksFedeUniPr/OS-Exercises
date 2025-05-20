# L'importanza del buffer nella comunicazione con Named Pipes
Il buffer svolge un ruolo cruciale nel codice di comunicazione tramite named pipes per diverse ragioni fondamentali:

1. ## Natura della comunicazione tramite pipe
Le pipe (named o anonime) sono stream di byte non strutturati. Non esistono "messaggi" naturalmente delimitati - è solo un flusso continuo di dati. Il buffer ci permette di:

Leggere blocchi di dati di dimensione definita
Trattare questi blocchi come messaggi discreti
2. ## Operazioni di I/O di basso livello
Le funzioni `read()` e `write()` utilizzate con i file descriptor:

Richiedono un'area di memoria (buffer) dove scrivere o da cui leggere i dati
Non gestiscono automaticamente la formattazione o la struttura dei dati

3. ## Gestione della memoria temporanea
Nel tuo codice client, il buffer serve per:

```C
fgets(buffer, BUFFER_SIZE, stdin);  // Memorizza l'input dell'utente
write(request_fd, buffer, strlen(buffer));  // Invia i dati al server
read(response_fd, buffer, BUFFER_SIZE);  // Riceve la risposta
```

4. ## Controllo dei messaggi
Il buffer ti permette di:

Elaborare il messaggio prima di inviarlo `(buffer[strcspn(buffer, "\n")] = 0)` 
Controllare il contenuto per decisioni logiche `(strcmp(buffer, "exit") == 0)`
Assicurarsi che i messaggi siano correttamente terminati `(buffer[bytes_read] = '\0')`

5. Dimensionamento appropriato
La dimensione del buffer (BUFFER_SIZE 256) determina:

La quantità massima di dati che puoi leggere/scrivere in una singola operazione
Un compromesso tra efficienza (buffer più grandi) e uso della memoria
Senza un buffer adeguato, sarebbe praticamente impossibile implementare una comunicazione strutturata attraverso le named pipes, poiché non avresti modo di gestire, memorizzare temporaneamente o interpretare i dati che fluiscono attraverso la comunicazione.





# Monitor 
=> mutex per le shared resorces
=> operazioni sincronizzati
=> condition variable (pthread_cond_t)