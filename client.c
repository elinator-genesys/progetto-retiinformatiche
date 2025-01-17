#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utility.h"
#include "function.h"

#define SERVER_IP "127.0.0.1"
#define DIM_BUFFER 1024

int main(int argc, char *argv[]) {
    int ret, fdmax, i;
    int utente = 1; /* Per capire se l'utente è del tipo che può mandare messaggi */ 
    struct sockaddr_in server_addr;
    fd_set read_fds, master;
    char buffer[1024];

    in_port_t porta = htons(4242);

    /*Creazione del socket*/
    fdmax = socket(AF_INET, SOCK_STREAM, 0);
    if (fdmax < 0) {
        perror("ERRORE: Errore nella creazione del socket");
        exit(1);
    }

    /*Configurazione dell'indirizzo del server*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = porta;  /*Porta dal comando*/
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);  /*IP dal comando*/

    /*Connessione al server*/
    ret = connect(fdmax, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret < 0) {
        perror("ERRORE: Errore di connessione al server");
        exit(1);
    }

    printf("Connesso al server.\n");

    printf("******************* CLIENT AVVIATO *******************\n");
    memset(buffer, 0, DIM_BUFFER);
    /* Ricezione del messaggio iniziale con le istruzioni*/
    ret = recv(fdmax, buffer, DIM_BUFFER, 0);
    if(ret < 0){
        perror("ERRORE: Errore nella ricezione delle istruzioni");
        exit(1);
    }
    printf("%s\n"
            "******************************************************\n\n", buffer);

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(fdmax, &master);
    FD_SET(STDIN_FILENO, &master);

    /*Ciclo principale del client*/
    while(1){
        memset(buffer, 0, DIM_BUFFER);
        read_fds = master;
        select(fdmax + 1, &read_fds, NULL, NULL, NULL);
        
        for(i = 0; i <= fdmax; i++){
            if(FD_ISSET(i, &read_fds)) {
                if(i == STDIN_FILENO) {
                    /* Controllo dello STDIN per ricezione comandi da terminale del client */
                    memset(buffer, 0, DIM_BUFFER);
                    fgets(buffer, DIM_BUFFER, stdin);

                    if(utente == 2  && strstr(buffer, "end") == 0) {
                        /* Se non è ancora stata iniziata una connessione con il giocatore secondario e non us ail comando end */
                        printf("Ancora non sei stato interpellato dal giocatore principale\n");
                        continue;
                    }

                    /*Invia il comando al server*/
                    ret = send(fdmax, buffer, DIM_BUFFER, 0);
                    if(ret < 0){
                        perror("ERRORE: Errore nell'invio del comando");
                        exit(1);
                    }

                    char command[20], player[20], room[20];
                    player[0] = '\0';
                    room[0] = '\0';
                    sscanf(buffer, "%s %s %s", command, player, room);

                    if(!strcmp(command, "start")){
                        if(!strcmp(player, "1"))
                            utente = 1;
                        else if(!strcmp(player, "2") && gameOn())
                            utente = 2;
                        /*else if() per room con più giocatori*/
                    }
                }
                else if(i == fdmax){
                    /* Altrimenti è il server che invia messaggi direttamente, come infos e join del giocatore secondario*/
                    memset(buffer, 0, DIM_BUFFER);
                    ret = recv(fdmax, buffer, DIM_BUFFER, 0);
                    if(ret == 0) {
                        printf("Disconnessione avvenuta\n");
                        exit(0);
                    }
                    if(ret < 0) {
                        perror("ERRORE: Errore in ricezione del messaggio");
                        exit(1);
                    }

                    if(strcmp(buffer, "connessione in corso") == 0){
                        /* Connessione in corso, anche il giocatore secondario può interagire */
                        utente = 1;
                        printf("Il disinnescatore si è connesso, quanti bitcoin gli vuoi chiedere?\n");
                    } 
                    else
                        printf("%s\n", buffer);

                    /* A fine connessione in qualunque caso il giocatore secondario non può più interagire */
                    if(strstr(buffer, "La richiesta è stata troppo elevata") != NULL || strstr(buffer, "Hai aiutato il disinnescatore") != NULL)
                        utente = 2;
                }
            }
        }
    }
    return 0;
}