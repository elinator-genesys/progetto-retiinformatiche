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

#include "function.h"
#include "utility.h"

#define MAX_CLIENT 4

int main(int argc, char *argv[])
{
    int fdmax, ret, i, socket_ascolto, socket_comunicazione, utente;
    fd_set  master, readfds;
    socklen_t addrlen;
    struct sockaddr_in  server_addr, client_addr;
    in_port_t porta = htons(atoi(argv[1]));
    char buffer[DIM_BUFFER];
    struct Game *current_game;
    struct User *current_user;


    /* ------------------------------------
        Avvio e inizializzazione del server 
       ------------------------------------ */ 
    printf("! Avvio e inizializzazione del server in corso...\n\n");

    socket_ascolto = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_ascolto < 0) {
        perror("ERRORE: Errore nella creazione del socket");
        exit(1);
    }

    /* permette il riavvio del server instantanemente senza problemi di porta*/
    ret = setsockopt(socket_ascolto, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    if(ret < 0) {
        perror("ERRORE: Errore setsockopt");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = porta;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(socket_ascolto, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret < 0) {
        perror("ERRORE: Errore bind");
        exit(1);
    }

    ret = listen(socket_ascolto, MAX_CLIENT);
    if(ret < 0) {
        perror("ERRORE: Errore listen");
        exit(1);
    }
    printf("OK: Socket di ascolto creato. fd: %d\n\n", socket_ascolto);

    /* Azzeramento e inizializzaizione dei set (princpale e di appoggio) */
    FD_ZERO(&master);
    FD_ZERO(&readfds);

    FD_SET(socket_ascolto, &master);
    FD_SET(STDIN_FILENO, &master);
    fdmax = socket_ascolto;

    printf("******************* SERVER AVVIATO *******************\n"
           "Comandi disponibili:\n"
           "- stop termina il server\n"
           "******************************************************\n\n");
    
    for(;;) 
    {
        memset(buffer, 0, DIM_BUFFER);
        readfds = master;
        select(fdmax + 1, &readfds, NULL, NULL, NULL);

        for(i = 0; i <= fdmax; i++) 
        {
            if(FD_ISSET(i, &readfds)) {
                /* Controllo dello STDIN per ricezione comandi diretti al server */
                if(i == STDIN_FILENO) {
                    memset(buffer, 0, DIM_BUFFER);
                    fgets(buffer, DIM_BUFFER, stdin);

                    printf("\n******************************************************\n");
                    if(strcmp(buffer, "stop\n") != 0) {
                        printf("Comando non riconosciuto.\n"
                            "******************************************************\n\n");
                        continue;
                    }
                    printf("! Chiusura del server...\n\n");
                    if(gameOn() == 1) { /* se è presente una partita non termino */
                        printf("Impossibile terminare il server, il gioco è in corso.\n"
                                "******************************************************\n\n");
                        continue;
                    }
                    deleteUsers(); /* basta eliminare gli utenti perchè se siamo arrivati qui le sessioni sono già state deallocate */
                    close(socket_ascolto);
                    FD_ZERO(&master);
                    FD_ZERO(&readfds);
                    printf("Server chiuso correttamente\n"
                            "******************************************************\n");
                    exit(0);
                }
                /* Controllo il socket di ascolto, addetto alle nuove connessioni*/
                else if(i == socket_ascolto) {
                    addrlen = sizeof(client_addr);
                    memset(&client_addr, 0, sizeof(client_addr));
                    socket_comunicazione = accept(socket_ascolto, (struct sockaddr*)&client_addr, &addrlen);
                    
                    if(socket_comunicazione < 0) {
                        perror("ERRORE: Errore accept");
                        exit(1);
                    }
                    printf("******************************************************\n"
                           "Nuovo client connesso, socket: %d\n"
                           , socket_comunicazione);

                    /* invio al client gli scenari disponibili */
                    memset(buffer, 0, DIM_BUFFER);
                    instructionLogin(buffer);
                    send(socket_comunicazione, buffer, DIM_BUFFER, 0); 
                    
                    FD_SET(socket_comunicazione, &master);
                    if(socket_comunicazione > fdmax) {
                        fdmax = socket_comunicazione;
                    }
                }
                /* Se non è nessuno dei socket precedenti è quello di comunicazione */
                else {
                    memset(buffer, 0, DIM_BUFFER);
                    ret = recv(i, (void*)buffer, DIM_BUFFER, 0);
                    current_game = getGame(i);
                    /* potrebbe returnane NULL se non trova niente (qualsiasi cosa prima di aver fatto comando start) */

                    if(ret == 0) {
                        /* se il client ancora non è entrato in nessuna partita */
                        if(current_game == NULL) {
                            
                            printf( "******************************************************\n"
                                    "! Sconnessione socket %d in corso...\n", i);

                            logout(findUserBySocket(i)); /* non essendo loggato */
                            close(i);  
                            FD_CLR(i, &master);
                            printf( "Socket %d chiuso.\n"
                                    "Il socket non faceva parte di nessuna partita.\n", i);

                            printf( "SUCCESSO: Disconessione eseguita con successo\n"
                                    "******************************************************\n\n");
                            continue;
                        }
                        
                        /* Se invece il client faceva parte di una sessione si disconnette il socket */
                        current_user = current_game->first->socket == i ? current_game->first : current_game->second;
                        utente = current_user == current_game->first ? 1 : 2;
                        printf("******************************************************\n"
                              "! Sconnessione socket %d in corso...\n"
                              "! Eliminazione utente: %s\n", i, current_user->username);
                        logout(current_user);
                        close(i);
                        printf("Socket %d chiuso.\n", i);
                        FD_CLR(i, &master);
                        printf("Socket %d rimosso dal set dei descrittori.\n"
                                "OK: Disconnessione client eseguita con successo.\n\n", i);
                               
                        /* se il client è principale va disconnesso anche il client secondario*/
                        if(utente == 1) {
                            if(current_game->second){
                                printf( "******************************************************\n"
                                        "! Sconnessione socket %d in corso...\n"
                                        "! Eliminazione utente: %s\n", current_game->second->socket, current_game->second->username);
                                close(current_game->second->socket);
                                printf("Socket %d chiuso.\n", current_game->second->socket);
                                FD_CLR(current_game->second->socket, &master);
                                printf("Socket %d rimosso dal set dei descrittori.\n"
                                        "OK: Disconnessione client eseguita con successo.\n\n", current_game->second->socket);
                                logout(current_game->second);
                            }
                            deleteGame(current_game->id);
                            printf("OK: Sessione eliminata con successo\n");
                        }
                        printf("SUCCESSO: Disconessione eseguita con successo\n"
                                "******************************************************\n\n");
                        continue;
                    }
                    
                    /* Informazioni generali per il server */
                    printf("******************************************************\n"
                          "Comando: %s\n"
                          "Game: %d\n"
                          "Room: %d\n"
                          "Utente: %s\n"
                          "Socket: %d\n"
                          "Risposta server: \n"
                          ,buffer, 
                          current_game ? current_game->id : -1,
                          current_game ? current_game->room->id : -1, 
                          current_game ? (current_game->first->socket == i ? current_game->first->username : current_game->second->username) : "non in sessione", 
                          i);
                
                    char action[20], target1[20], target2[20];
                    target1[0] = '\0';
                    target2[0] = '\0';
                    sscanf(buffer, "%s %s %s", action, target1, target2);

                    /* Switch per gestire il comando inserito */
                    switch (hashString(current_game, action)) {
                        case 1:
                            commandSignup(target1, target2, i);
                            break;
                        case 2:
                            commandLogin(target1, target2, i);
                            break;
                        case 3:
                            commandStart(current_game, target1, target2, i);
                            break;
                        case 4:
                            commandEnd(current_game, target1, target2, i, &master);
                            break;
                        case 5:
                            /* Controllo la room da giocare */
                            if(current_game->room->id == 1)
                                commandLook(current_game, target1, target2, i);
                            /*else if() per comandi di altre room*/
                            /* Invio le informazioni di gioco al client */
                            gameInfos(current_game, target1, target2, i, &master);
                            break;
                        case 6:
                            /* Controllo la room da giocare */
                            if(current_game->room->id == 1)
                                commandTake(current_game, target1, target2, i);
                            /*else if() per comandi di altre room*/
                            /* Invio le informazioni di gioco al client */
                            gameInfos(current_game, target1, target2, i, &master);
                            break;
                        case 7:
                            /* Controllo la room da giocare */
                            if(current_game->room->id == 1)
                                commandUse(current_game, target1, target2, i, &master);
                            /*else if() per comandi di altre room*/
                            /* Invio le informazioni di gioco al client */
                            gameInfos(current_game, target1, target2, i, &master);
                            break;
                        case 8:
                            /* Controllo la room da giocare */
                            if(current_game->room->id == 1)
                                commandObjs(current_game, target1, target2, i);
                            /*else if() per comandi di altre room*/
                            /* Invio le informazioni di gioco al client */
                            gameInfos(current_game, target1, target2, i, &master);
                            break;
                        case 9:
                            printf("Partita non ancora iniziata\n");
                            break;
                        case 10:
                            /* E in corso una connessione */
                            PcTexting(current_game, i, action, &master);
                            break;
                        default:
                            printf("Comando non riconosciuto\n");
                            strcpy(buffer, "Comando non riconosciuto\n");
                            send(i, buffer, sizeof(buffer), 0);
                            break;
                    }
                    printf("\n******************************************************\n\n");
                }
            }
        } 
    }
}