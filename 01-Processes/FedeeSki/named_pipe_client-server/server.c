#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define REQUEST_PIPE "/tmp/request_pipe"
#define RESPONSE_PIPE "/tmp/response_pipe"
#define BUFFER_SIZE 256

//SERVER ASCOLTA RICHIESTE

int main(int argc, char const *argv[])
{
    int request_fd, response_fd;
    char BUFFER[BUFFER_SIZE];

    printf("== Starting the SERVER: == \n");
    //Creazione named pipes
    if(mkfifo(REQUEST_PIPE, 0666) == -1 && errno != EEXIST){
        perror("Error crafting request pipe");
        exit(EXIT_FAILURE);
    }
    if(mkfifo(RESPONSE_PIPE, 0666) == -1 && errno != EEXIST){
        perror("Error crafting response pipe");
        exit(EXIT_FAILURE);
    }
    
     while (1) {
        // Apri la pipe di richiesta in lettura (bloccante finché un client non si connette)
        request_fd = open(REQUEST_PIPE, O_RDONLY);
        if (request_fd == -1) {
            perror("SERVER: Errore nell'apertura della request pipe");
            break;
        }
        
        // Apri la pipe di risposta in scrittura
        response_fd = open(RESPONSE_PIPE, O_WRONLY);
        if (response_fd == -1) {
            perror("SERVER: Errore nell'apertura della response pipe");
            close(request_fd);
            break;
        }
        
        printf("SERVER: Client connesso!\n");

        // Loop di elaborazione richieste
        while (1) {
            int bytes_read = read(request_fd, BUFFER, BUFFER_SIZE);
            
            if (bytes_read <= 0) {
                printf("SERVER: Client disconnesso.\n");
                break;
            }
            
            BUFFER[bytes_read] = '\0';
            printf("SERVER: Ricevuto dal client: %s\n", BUFFER);
            
            // Controllo se il client vuole uscire
            if (strcmp(BUFFER, "exit") == 0) {
                printf("SERVER: Richiesta di uscita ricevuta.\n");
                break;
            }
            
            // Elabora la richiesta (in questo caso, converte il messaggio in maiuscolo)
            for (int i = 0; i < bytes_read; i++) {
                if (BUFFER[i] >= 'a' && BUFFER[i] <= 'z')
                    BUFFER[i] = BUFFER[i] - 32;
            }

             // Invia la risposta
            sprintf(BUFFER, "SERVER: %s", BUFFER);
            write(response_fd, BUFFER, strlen(BUFFER));
            printf("Risposta inviata.\n");
        }

         // Chiudi le pipe per questo client
        close(request_fd);
        close(response_fd);
        
        printf("SERVER: In attesa di nuove connessioni...\n");
    }

        // Pulizia
    unlink(REQUEST_PIPE);
    unlink(RESPONSE_PIPE);
    printf("SERVER: Server terminato.\n");

    return EXIT_SUCCESS;
}
