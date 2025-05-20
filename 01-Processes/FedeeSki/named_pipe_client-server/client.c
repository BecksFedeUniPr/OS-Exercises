#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define REQUEST_PIPE "/tmp/request_pipe"
#define RESPONSE_PIPE "/tmp/response_pipe"
#define BUFFER_SIZE 256

//CLIENT INVIA RICHIESTE

int main(int argc, char const *argv[])
{
    
    int request_fd, response_fd;
    char buffer[BUFFER_SIZE];
    
    printf("== Starting del CLIENT: == \n");
    
    // Apri la pipe di richiesta in scrittura
    printf("CLIENT: Connessione al server...\n");
    request_fd = open(REQUEST_PIPE, O_WRONLY);
    if (request_fd == -1) {
        perror("CLIENT: Errore nell'apertura della request pipe");
        printf("CLIENT: Assicurati che il server sia in esecuzione.\n");
        exit(EXIT_FAILURE);
    }
    
    // Apri la pipe di risposta in lettura
    response_fd = open(RESPONSE_PIPE, O_RDONLY);
    if (response_fd == -1) {
        perror("CLIENT: Errore nell'apertura della response pipe");
        close(request_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("CLIENT: Connesso al server!\n");
    printf("CLIENT: Puoi inviare messaggi. Scrivi 'exit' per uscire.\n");
    
    // Loop di invio messaggi
    while (1) {
        printf("Messaggio > ");
        fgets(buffer, BUFFER_SIZE, stdin);
        //stdin (stream input standard (keyboard))
        buffer[strcspn(buffer, "\n")] = 0; // Rimuovi il newline
        
        // Invia il messaggio al server
        write(request_fd, buffer, strlen(buffer));
        
        // Esci se richiesto
        if (strcmp(buffer, "exit") == 0) {
            printf("CLIENT: Uscita...\n");
            break;
        }
        //leggi risposta server
             int bytes_read = read(response_fd, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            printf("CLIENT: Server disconnesso.\n");
            break;
        }
        
        buffer[bytes_read] = '\0';
        printf("Risposta: %s\n", buffer);
    }
    
    // Chiudi le pipe
    close(request_fd);
    close(response_fd);
    
    printf("CLIENT: Client terminato.\n");

    return EXIT_SUCCESS;
}
