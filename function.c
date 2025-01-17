#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "function.h"
#include "utility.h"

/**
 * Funzione per inizializzare una stanza specifica con oggetti, descrizioni e localizzazioni
 * @param id ID della stanza da inizializzare. Attualmente supporta solo l'ID 1.
 * @return Puntatore alla struttura 'Room' appena inizializzata con oggetti e localizzazioni
 */
struct Room* initializeRoom(int id) {
    /* Allocazione della memoria per la stanza*/
    struct Room *room = malloc(sizeof(struct Room));

    if (id == 1) {
        /* ID della stanza */
        room->id = 1;

        /* Descrizione della stanza */
        strcpy(room->description,   "Ti trovi in una stanza con un ++tavolo++, una ++mensola++ e una ++poltrona++.\n"
                                    "Strano mi sembra di sentire un ticchettio.\n");

        /* Inizializzazione degli oggetti*/
        strcpy(room->items[0].name, "bomba");
        strcpy(room->items[0].description,  "Una bomba...oh no il timer è già partito.\n"
                                            "Ci sono un cavo da tagliare e un tastierino con un codice segreto.\n");
        strcpy(room->items[0].take, "\0");
        room->items[0].isLocked = false;
        room->items[0].answering = false;
        room->items[0].taked = false;
        room->items[0].token = false;
        room->items[0].hidden = false;
        room->items[0].used = false;
        strcpy(room->items[0].enigma.question, "\0");
        strcpy(room->items[0].enigma.solution, "\0");

        strcpy(room->items[1].name, "scatola");
        strcpy(room->items[1].description, "Una scatola chiusa con un lucchetto.\n");
        strcpy(room->items[1].take, "\0");
        room->items[1].isLocked = true;
        room->items[1].answering = false;
        room->items[1].taked = false;
        room->items[1].token = false;
        room->items[1].hidden = false;
        room->items[1].used = false;
        strcpy(room->items[1].enigma.question, "Continuare questa sequenza per aprire la scatola: '1 - 2 - 4 - 8 - ?', qual è la risposta? (Inserisci solo il numero mancante)");
        strcpy(room->items[1].enigma.solution, "16\n");

        strcpy(room->items[2].name, "torcia");
        strcpy(room->items[2].description, "Una torcia, però sembra che non si accenda.\n");
        strcpy(room->items[2].take, "Sembra che manchi la batteria, ecco perché non si accende.\n");
        room->items[2].isLocked = false;
        room->items[2].answering = false;
        room->items[2].taked = false;
        room->items[2].token = false;
        room->items[2].hidden = false;
        room->items[2].used = false;
        strcpy(room->items[2].enigma.question, "\0");
        strcpy(room->items[2].enigma.solution, "\0");

        strcpy(room->items[3].name, "batteria");
        strcpy(room->items[3].description, "Una batteria, chissà come potrei usarla?\n");
        strcpy(room->items[3].take, "Sembra proprio della dimensione giusta per la torcia.\n");
        room->items[3].isLocked = true;
        room->items[3].answering = false;
        room->items[3].taked = false;
        room->items[3].token = false;
        room->items[3].hidden = true;
        room->items[3].used = false;
        strcpy(room->items[3].enigma.question, "\0");
        strcpy(room->items[3].enigma.solution, "\0");

        strcpy(room->items[4].name, "cassetto");
        strcpy(room->items[4].description, "Un cassetto chiuso da un indovinello.\n");
        strcpy(room->items[4].take, "\0");
        room->items[4].isLocked = true;
        room->items[4].answering = false;
        room->items[4].taked = false;
        room->items[4].token = false;
        room->items[4].hidden = false;
        room->items[4].used = false;
        strcpy(room->items[4].enigma.question, "Risolvere il seguente indovinello per aprire il cassetto: 'è tuo, ma lo usano di più gli altri', qual è la risposta?");
        strcpy(room->items[4].enigma.solution, "nome\n");

        strcpy(room->items[5].name, "forbici");
        strcpy(room->items[5].description, "Delle forbici molto affilate\n");
        strcpy(room->items[5].take, "Queste forbici potrebbero tagliare molte cose, un cavo per esempio.\n");
        room->items[5].isLocked = true;
        room->items[5].answering = false;
        room->items[5].taked = false;
        room->items[5].token = false;
        room->items[5].hidden = true;
        room->items[5].used = false;
        strcpy(room->items[5].enigma.question, "\0");
        strcpy(room->items[5].enigma.solution, "\0");

        strcpy(room->items[6].name, "bitcoin1");
        strcpy(room->items[6].description, "Uno strano codice utilizzabile per ottenere dei bitcoin\n");
        strcpy(room->items[6].take, "I bitcoin si utilizzano anche nel dark web, chissà se mai mi serviranno?.\n");
        room->items[6].isLocked = false;
        room->items[6].answering = false;
        room->items[6].taked = false;
        room->items[6].token = true;
        room->items[6].hidden = false;
        room->items[6].used = false;
        strcpy(room->items[6].enigma.question, "\0");
        strcpy(room->items[6].enigma.solution, "\0");

        strcpy(room->items[7].name, "bitcoin2");
        strcpy(room->items[7].description, "Uno strano codice utilizzabile per ottenere dei bitcoin\n");
        strcpy(room->items[7].take, "I bitcoin si utilizzano anche nel dark web, chissà se mai mi serviranno?.\n");
        room->items[7].isLocked = false;
        room->items[7].answering = false;
        room->items[7].taked = false;
        room->items[7].token = true;
        room->items[7].hidden = false;
        room->items[7].used = false;
        strcpy(room->items[7].enigma.question, "\0");
        strcpy(room->items[7].enigma.solution, "\0");

        strcpy(room->items[8].name, "bitcoin3");
        strcpy(room->items[8].description, "Uno strano codice utilizzabile per ottenere dei bitcoin\n");
        strcpy(room->items[8].take, "I bitcoin si utilizzano anche nel dark web, chissà se mai mi serviranno?.\n");
        room->items[8].isLocked = true;
        room->items[8].answering = false;
        room->items[8].taked = false;
        room->items[8].token = true;
        room->items[8].hidden = false;
        room->items[8].used = false;
        strcpy(room->items[8].enigma.question, "\0");
        strcpy(room->items[8].enigma.solution, "\0");

        strcpy(room->items[9].name, "bitcoin4");
        strcpy(room->items[9].description, "Uno strano codice utilizzabile per ottenere dei bitcoin\n");
        strcpy(room->items[9].take, "I bitcoin si utilizzano anche nel dark web, chissà se mai mi serviranno?.\n");
        room->items[9].isLocked = true;
        room->items[9].answering = false;
        room->items[9].taked = false;
        room->items[9].token = true;
        room->items[9].hidden = false;
        room->items[9].used = false;
        strcpy(room->items[9].enigma.question, "\0");
        strcpy(room->items[9].enigma.solution, "\0");

        strcpy(room->items[10].name, "portatile");
        strcpy(room->items[10].description, "Un computer portatile\n");
        strcpy(room->items[10].take, "Potrei comunicare con qualcuno tramite questo portatile.\n");
        room->items[10].isLocked = false;
        room->items[10].answering = false;
        room->items[10].taked = false;
        room->items[10].token = false;
        room->items[10].hidden = false;
        room->items[10].used = false;
        strcpy(room->items[10].enigma.question, "\0");
        strcpy(room->items[10].enigma.solution, "\0");

        strcpy(room->locations[0].name, "tavolo");
        strcpy(room->locations[0].description, "Sul tavolo ci sono una **bomba**, una **torcia**, una **scatola** e sembra che ci sia un **cassetto** qui sotto\n");

        strcpy(room->locations[1].name, "mensola");
        strcpy(room->locations[1].description, "Sulla mensola c'è un codice segreto per ottenere dei **bitcoin1**\n");

        strcpy(room->locations[2].name, "poltrona");
        strcpy(room->locations[2].description, "Sulla poltrona c'è un **portatile** e un codice segreto per ottenere dei **bitcoin2**\n");

        /* Imposta il numero di oggetti nella stanza*/
        room->itemCount = 11;
        room->tokenCount = 4;
    }

    /* else if(id == ...){} Inizializzazione per altre stanze  */

    return room; /* Restituisci il puntatore alla stanza inizializzata*/
}

/**
 * Funzione per gestire il comando 'signup' nel gioco
 * @param message Messaggio contenente i dettagli dell'utente (username, password)
 * @param socket Socket attraverso il quale inviare la risposta al client
 * @param session* La sessione corrente del gioco
 */
void commandSignup(char *target1, char *target2, int client_socket){
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER); 

    /* Verifica se i parametri target1 (username) e target2 (password) sono vuoti */
    if(target1[0] == '\0' || target2[0] == '\0'){
        strcpy(buffer, "Formato del comando signup non valido, il formato corretto è -> signup username password\n");
        send(client_socket, buffer, sizeof(buffer), 0);
        printf("Formato non valido"); 
        return;
    }

    /* Se il formato è valido, chiama la funzione registerUser per registrare l'utente */
    registerUser(target1, target2, client_socket);
}


/**
 * Funzione per gestire il comando 'login' nel gioco
 * @param message Messaggio contenente le credenziali di login (username, password)
 * @param socket Socket attraverso il quale inviare la risposta al client
 * @param session* La sessione corrente del gioco
 */
void commandLogin(char *target1, char *target2, int client_socket){
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER); 

    struct User *u = findUserBySocket(client_socket); 
    /* Verifica se l'utente non è stato trovato */
    if(u == NULL){
        strcpy(buffer, "Non sei ancora registrato\n");
        send(client_socket, buffer, sizeof(buffer), 0);  
        printf("Utente non trovato\n");
        return;  
    }

    /* Verifica se le credenziali (username e password) non corrispondono */
    if(strcmp(u->username, target1) || strcmp(u->password, target2)){
        strcpy(buffer, "Non sei ancora registrato\n");
        send(client_socket, buffer, sizeof(buffer), 0);  
        printf("Utente non trovato\n");
        return;  
    }

    /* Verifica se l'utente ha già effettuato il login */
    if(u->logged == 1){
        strcpy(buffer, "Login già effettuato, puoi iniziare a disinnescare bombe\n");
        send(client_socket, buffer, sizeof(buffer), 0); 
        printf("Login già effettuato\n");
        return;  
    }

    /* Verifica se i parametri (username e password) sono vuoti */
    if(target1[0] == '\0' || target2[0] == '\0'){
        strcpy(buffer, "Formato del comando login non valido, il formato corretto è -> login username password\n");
        send(client_socket, buffer, sizeof(buffer), 0); 
        printf("Formato non valido");  
        return; 
    }

    /* Se tutto è valido, esegue il login dell'utente */
    login(target1, target2, client_socket);
}


/**
 * Funzione per avviare il gioco, resettando gli stati e inizializzando la sessione
 * @param message Messaggio contenente le informazioni di inizio gioco
 * @param socket Socket attraverso il quale inviare la risposta al client
 * @param session* La sessione corrente del gioco
 */
void commandStart(struct Game *game, char *target1, char *target2, int client_socket){
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER);
    struct Game *g;

    /* Cerca l'utente con il socket specificato */
    struct User *u = findUserBySocket(client_socket);
    if(u == NULL){
        /* Se l'utente non è registrato (non ha effettuato il login), invia un messaggio di errore */
        strcpy(buffer, "Login non effettuato, prima devi accedere per iniziare\n");
        send(client_socket, buffer, sizeof(buffer), 0);
        printf("Utente non registrato\n");
        return;
    }

    /* Verifica che i parametri del comando siano corretti (due parametri: giocatore e stanza) */
    if(target1[0] == '\0' || target2[0] == '\0'){
        /* Se uno dei parametri è vuoto, invia un messaggio di errore sul formato del comando */
        strcpy(buffer, "Formato del comando start non valido, il formato corretto è -> start player room\n");
        send(client_socket, buffer, sizeof(buffer), 0);
        printf("Formato non valido");
        return;
    }

    /*Descrizione iniziale*/
    if(!strcmp(target2, "1")){
        /* Se la stanza è la "1" */
        if(!strcmp(target1, "1")){
            /* Se il giocatore principale (player 1) tenta di iniziare una partita */
            if(game){
                /* Se il gioco è già attivo (game != NULL), invia un messaggio che la partita è già iniziata */
                strcpy(buffer, "Partita già iniziata!\n");
                send(client_socket, buffer, sizeof(buffer), 0);
                printf("Partita già iniziata\n");
                return;
            }
            /* Se il gioco non è attivo, inizia una nuova partita */
            printf("Partita iniziata\n");
            createGame(client_socket);
        }
        else if(!strcmp(target1, "2")){
            /* Se il secondo giocatore (player 2) tenta di entrare */
            g = freeGame(1);
            if(g){
                /* Se ci sono stanze disponibili, collega il secondo giocatore */
                g->second = findUserBySocket(client_socket);
                strcpy(buffer,  "Te sei un utente misterioso del dark web, e sai come poter aiutare il giocatore principale\n"
                                "a disinnescare la bomba, ma ovviamente non lo farai gratuitamente.\n"
                                "Se ti verra chiesto aiuto potrai scegliere quanto costa il tuo aiuto.\n");
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, DIM_BUFFER);
                strcpy(buffer, "Un misterioso utente si è collegato al portatile!\n");
                send(g->first->socket, buffer, sizeof(buffer), 0);
                printf("Partita iniziata\n");
            }
            else{
                /* Se non ci sono stanze disponibili, invia un messaggio di errore */
                strcpy(buffer, "Non ci sono room disponibili al momento\n");
                send(client_socket, buffer, sizeof(buffer), 0);
                printf("Room piene\n");
            }
        }
        else{
            /* Se il numero del giocatore non è né 1 né 2, invia un messaggio di errore */
            strcpy(buffer, "Numero di giocatore inesistente -> 1 per il giocatore principale, 2 per il secondario\n");
            send(client_socket, buffer, sizeof(buffer), 0);
            printf("Formato non valido");
        }
    }
    else{
        /* Se la stanza non esiste (target2 != 1), invia un messaggio di errore */
        strcpy(buffer, "Room non esistente -> 1 per giocare la prima room\n");
        send(client_socket, buffer, sizeof(buffer), 0);
        printf("Formato non valido\n");
    }
    /*else if(){} per aggiungere altre room*/
}


/**
 * Funzione per terminare una partita, chiudere le connessioni e rimuovere i giocatori dal gioco.
 * Se la partita è vinta, invia le informazioni finali. Se la partita è ancora in corso,
 * il comando 'end' deve essere utilizzato senza parametri.
 * @param game Puntatore alla struttura 'Game' che rappresenta la partita in corso.
 * @param target1 Primo parametro del comando (non utilizzato in questo caso).
 * @param target2 Secondo parametro del comando (non utilizzato in questo caso).
 * @param client_socket Socket del client che ha inviato il comando.
 * @param master Set di descrittori di file per il server, per gestire i client connessi.
 */
void commandEnd(struct Game *game, char *target1, char *target2, int client_socket, fd_set *master){
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER);
    
    /* Verifica se il gioco non esiste */
    if(game == NULL) {
        /* Se il gioco non esiste, chiudi il socket e rimuovi il client dalla lista dei file descriptor */
        close(client_socket);
        FD_CLR(client_socket, master);
        /* Esegui il logout dell'utente, poiché non è loggato in una partita */
        logout(findUserBySocket(client_socket)); 
        printf("Nessuna partita in corso\n");
        return;
    }
    
    /* Se il gioco non è vinto */
    if(game->win == false){
        /* Se ci sono parametri nel comando, invia un messaggio di errore sul formato del comando */
        if(target1[0] != '\0' || target2[0] != '\0'){
            strcpy(buffer, "\nFormato del comando end non valido, il formato corretto è -> end\n");
            send(client_socket, buffer, sizeof(buffer), 0);
            printf("Formato non valido\n");
            return;
        }
    }
    else{
        /* Se il gioco è vinto, invia le informazioni del gioco al client */
        gameInfos(game, target1, target2, client_socket, master);
    }

    /* Se il client è il primo giocatore (giocatore principale) */
    if(game->first->socket == client_socket){
        /* Chiudi il socket del primo giocatore (socket principale) */
        close(game->first->socket);
        FD_CLR(game->first->socket, master);
        /* Esegui il logout del primo giocatore */
        logout(game->first);

        /* Se esiste un secondo giocatore */
        if(game->second){
            /* Invia un messaggio al secondo giocatore che la partita è terminata */
            strcpy(buffer, "La partita è terminata\n");
            send(game->second->socket, buffer, sizeof(buffer), 0);
            /* Chiudi il socket del secondo giocatore */
            close(game->second->socket);
            FD_CLR(game->second->socket, master);
            /* Esegui il logout del secondo giocatore */
            logout(game->second);
        }

        /* Elimina il gioco dalla lista dei giochi */
        deleteGame(game->id);
    }
    else{
        /* Se il client non è il primo giocatore (è il secondo giocatore) */
        strcpy(buffer, "L'utente del dark web si disconnesso!\n");
        send(game->first->socket, buffer, sizeof(buffer), 0);
        /* Chiudi il socket del secondo giocatore */
        close(game->second->socket);
        FD_CLR(game->second->socket, master);
        /* Esegui il logout del secondo giocatore */
        logout(game->second);
    }
    printf("Partita terminata\n");
}


/**
 * Funzione per gestire il comando 'look', che mostra l'ambiente corrente
 * @param message Messaggio contenente il comando di esplorazione
 * @param socket Socket attraverso il quale inviare la risposta al client
 * @param session* La sessione corrente del gioco
 */
void commandLook(struct Game *game, char *target1, char *target2, int client_socket) {
    struct Room *room = game->room;
    char buffer[DIM_BUFFER];
    int i;
    memset(buffer, 0, DIM_BUFFER);

    /* Se ci sono parametri non validi nel comando 'look' */
    if(target2[0] != '\0'){
        /* Invia un messaggio di errore per formato non valido */
        strcpy(buffer, "Formato del comando look non valido, il formato corretto è -> look [loc | obj]\n");
        send(client_socket, buffer, DIM_BUFFER, 0);
        printf("Formato non valido\n");
        return;
    }

    /* Se non c'è un target (nessun oggetto o location specificato), mostra la descrizione della stanza */
    if(target1[0] == '\0'){
        strcpy(buffer, room->description);
        send(client_socket, buffer, DIM_BUFFER, 0);
        printf("Comando look andato a buon fine\n");
        return;
    }

    /* Controlla se il target è una location della stanza */
    for(i = 0; i < 3; i++){
        if(strcmp(room->locations[i].name, target1) == 0){
            /* Se la location corrisponde, invia la sua descrizione */
            strcpy(buffer, room->locations[i].description);
            send(client_socket, buffer, DIM_BUFFER, 0);
            printf("Comando look andato a buon fine\n");
            return;
        }
    }

    /* Controlla se il target è un oggetto della stanza */
    for(i = 0; i < room->itemCount; i++){
        if(strcmp(room->items[i].name, target1) == 0){
            /* Se l'oggetto non è nascosto, invia la sua descrizione */
            if(!room->items[i].hidden){
                strcpy(buffer, room->items[i].description);
                send(client_socket, buffer, DIM_BUFFER, 0);
                printf("Comando look andato a buon fine\n");
                return;
            }
            else{
                /* Se l'oggetto è nascosto, invia un messaggio che l'oggetto non è ancora visibile */
                strcpy(buffer, "Non hai ancora trovato questo oggetto\n");
                send(client_socket, buffer, DIM_BUFFER, 0);
                printf("Oggetto non trovato\n");
                return;
            }
        }
    }

    /* Se il target non è né una location né un oggetto, invia un messaggio di errore */
    strcpy(buffer, "Oggetto inesistente\n");
    send(client_socket, buffer, DIM_BUFFER, 0);   
    printf("Oggetto inesistente\n"); 
}

/**
 * Funzione per gestire il comando 'use', che consente di utilizzare un oggetto
 * @param message Messaggio contenente il comando 'use' e l'oggetto da utilizzare
 * @param socket Socket attraverso il quale inviare la risposta al client
 * @param session* La sessione corrente del gioco
 */
void commandUse(struct Game *game, char *target1, char *target2, int client_socket, fd_set *master) {
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER);
    struct Room *room = game->room; 

    /* Se c'è un target1 (oggetto o elemento da usare) */
    if(target1[0] != '\0'){
        /* Se target2 è vuoto, significa che il comando riguarda un solo oggetto */
        if(target2[0] == '\0'){
            /* Se l'oggetto è forbici */
            if(!strcmp(target1, "forbici")){
                /* Se le forbici sono state raccolte e non sono già state usate */
                if(room->items[5].used == false && room->items[5].taked == true){
                    /* Cavo tagliato */
                    game->solved++;
                    strcpy(buffer, "Hai tagliato il cavo della bomba!!\n");
                    send(client_socket, buffer, DIM_BUFFER, 0);
                    memset(buffer, 0, DIM_BUFFER);
                    /* Se il numero di problemi risolti è 2, il gioco è vinto */
                    if(game->solved == 2){
                        game->win = true;
                        strcpy(buffer, "Hai disinnescato la bomba in tempo, complimenti.\n");
                        send(client_socket, buffer, DIM_BUFFER, 0);
                        /* Fine del gioco con vittoria */
                        commandEnd(game, target1, target2, client_socket, master);
                    }
                    room->items[5].used = true;
                    printf("Comando use andato a buon fine\n");
                }
                else if(room->items[5].taked == false){
                    /* Le forbici non sono state raccolte */
                    strcpy(buffer, "Non hai raccolto questo oggetto\n");
                    send(client_socket, buffer, DIM_BUFFER, 0); 
                    printf("Oggetto non raccolto\n");
                }
                else{
                    /* Le forbici sono state già usate */
                    strcpy(buffer, "Hai già tagliato il cavo della bomba.\n");
                    send(client_socket, buffer, DIM_BUFFER, 0);
                    printf("Cavo già tagliato\n");
                }
            }
            /* Se l'oggetto è il portatile */
            else if(!strcmp(target1, "portatile")){
                /* Se il portatile è stato raccolto */
                if(room->items[10].taked == true){
                    /* Se esiste un secondo giocatore, invia il messaggio di connessione */
                    if(game->second){
                        printf("Comunicazione con il secondo giocatore - ");
                        strcpy(buffer, "connessione in corso");
                        send(game->second->socket, buffer, DIM_BUFFER, 0); 
                        memset(buffer, 0, DIM_BUFFER);
                        strcpy(buffer, "*** Connessione in corso con l'utente del dark web ***\n");
                        send(client_socket, buffer, DIM_BUFFER, 0);
                        game->texting = true;
                    }
                    else{
                        /* Se non c'è il secondo giocatore, invia il messaggio che il portatile è spento */
                        strcpy(buffer, "Il portatile è spento.\n");
                        send(client_socket, buffer, DIM_BUFFER, 0);
                        printf("Oggetto non utilizzabile\n");
                    }
                }
                else{
                    /* Se il computer non è stato raccolto */
                    strcpy(buffer, "Non hai raccolto questo oggetto\n");
                    send(client_socket, buffer, DIM_BUFFER, 0); 
                    printf("Oggetto non raccolto\n");
                }
            }
            else{
                /* Oggetto non riconosciuto o non utilizzabile */
                strcpy(buffer, "Oggetto non utilizzabile\n");
                send(client_socket, buffer, DIM_BUFFER, 0);
                printf("Oggetto non utilizzabile\n");
            }
            return;
        }
        else{
            /* Se ci sono due oggetti nel comando, verifica se sono torcia e batteria */
            if((!strcmp(target1, "torcia") && !strcmp(target2, "batteria")) || (!strcmp(target1, "batteria") && !strcmp(target2, "torcia"))){
                /* Se la torcia o la batteria non sono state usate */
                if((room->items[2].used == false || room->items[3].used == false) && room->items[2].taked == true && room->items[3].taked == true){
                    /* Luce ultravioletta attivata */
                    strcpy(buffer,  "Wow è una luce ultravioletta!!\n"
                                    "Ha rivelato il codice segreto da inserire sulla bomba.\n");
                    send(client_socket, buffer, DIM_BUFFER, 0);
                    memset(buffer, 0, DIM_BUFFER);
                    /* Torcia ultravioletta attivata */
                    game->solved++;
                    if(game->solved == 2){
                        game->win = true;
                        strcpy(buffer, "Hai disinnescato la bomba in tempo, complimenti.\n");
                        send(client_socket, buffer, DIM_BUFFER, 0);
                        /* Fine del gioco con vittoria */
                        commandEnd(game, target1, target2, client_socket, master);
                    }
                    room->items[2].used = true;
                    room->items[3].used = true;
                    printf("Comando use andato a buon fine");
                }
                else if(room->items[2].taked == false || room->items[3].taked == false){
                    /* La torcia e/o la batteria non sono tati raccolti */
                    strcpy(buffer, "Non hai raccolto questi oggetti\n");
                    send(client_socket, buffer, DIM_BUFFER, 0); 
                    printf("Oggetto non raccolto\n");
                }
                else{
                    /* La torcia e la batteria sono già stati usati */
                    strcpy(buffer, "Hai già inserito il codice segreto nel tastierino della bomba.\n");
                    send(client_socket, buffer, DIM_BUFFER, 0);
                    printf("Codice già inserito\n");
                }
            }
            else{
                /* Oggetti non utilizzabili insieme */
                strcpy(buffer, "Oggetti non utilizzabili\n");
                send(client_socket, buffer, DIM_BUFFER, 0);
                printf("Oggetto non utilizzabile\n");
            }
            return;
        }
    }
    else{
        /* Se il formato del comando non è valido invio un messaggio di errore */
        strcpy(buffer, "Formato del comando use non valido, il formato corretto è -> use obj1 [obj2]\n");
        send(client_socket, buffer, sizeof(buffer), 0);
        printf("Formato non valido");
        return;
    }

    /* Se l'oggetto non esiste invio un messaggio di errore*/
    strcpy(buffer, "Oggetto inesistente\n");
    send(client_socket, buffer, DIM_BUFFER, 0);
    printf("Oggetto inesistente\n"); 
}


/**
 * Funzione per gestire il comando 'take', che consente di raccogliere un oggetto
 * @param message Messaggio contenente il comando 'take' e l'oggetto da prendere
 * @param socket Socket attraverso il quale inviare la risposta al client
 * @param session* La sessione corrente del gioco
 */
void commandTake(struct Game *game, char *target1, char *target2, int client_socket){
    struct Room *room = game->room;
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER);

    /* Se l'inventario è pieno, il comando non può essere eseguito */
    if(game->itemTaked == 6){
        strcpy(buffer, "Il tuo inventario è pieno\n");
        send(client_socket, buffer, DIM_BUFFER, 0);
        printf("Inventario pieno\n");
        return;
    }

    /* Se target1 è vuoto o target2 è non vuoto, il comando ha un formato errato */
    if(target1[0] == '\0' || target2[0] != '\0'){
        strcpy(buffer, "Formato del comando take non valido, il formato corretto è -> take object\n");
        send(client_socket, buffer, sizeof(buffer), 0);
        printf("Formato non valido");
        return;
    }

    int i;
    /* Cicla attraverso gli oggetti nella stanza */
    for(i = 0; i < room->itemCount; i++){
        /* Se l'oggetto trovato corrisponde al nome di target1 */
        if(strcmp(room->items[i].name, target1) == 0){
            /* Se l'oggetto non è già stato preso */
            if(room->items[i].taked == false){
                /* Se l'oggetto non richiede una risposta a un indovinello */
                if(!room->items[i].answering){
                    /* Se l'oggetto ha un token e non è bloccato */
                    if(room->items[i].token){
                        if(!room->items[i].isLocked){
                            game->tokenTaked++;
                            if(room->items[i].take[0] != '\0'){
                                strcpy(buffer, room->items[i].take);
                                send(client_socket, buffer, DIM_BUFFER, 0);
                                room->items[i].taked = true;
                                printf("Comando take andato a buon fine\n");
                                return;
                            }
                        }
                        strcpy(buffer, "Questo oggetto non può essere raccolto\n");
                        send(client_socket, buffer, DIM_BUFFER, 0);
                        printf("Oggetto non raccoglibile\n");
                        return;
                    }
                    /* Se l'oggetto è bloccato e ha un indovinello */
                    if(room->items[i].isLocked && room->items[i].enigma.question[0] != '\0') {
                        strcpy(buffer, room->items[i].take);
                        send(client_socket, buffer, DIM_BUFFER, 0);
                        memset(buffer, 0, DIM_BUFFER);
                        sendEnigma(&room->items[i], client_socket);
                    } 
                    /* Se l'oggetto non è bloccato, può essere raccolto */
                    else if(!room->items[i].isLocked){
                        if(room->items[i].take[0] != '\0'){
                            strcpy(buffer, room->items[i].take);
                            send(client_socket, buffer, DIM_BUFFER, 0);
                            game->items[game->itemTaked] = room->items[i];
                            room->items[i].taked = true;
                            game->itemTaked++;
                            printf("Comando take andato a buon fine\n");
                            return;
                        }
                        else{
                            strcpy(buffer, "Questo oggetto non può essere raccolto\n");
                            send(client_socket, buffer, DIM_BUFFER, 0);
                            printf("Oggetto non raccoglibile\n");
                            return;
                        }
                    }
                    else{
                        strcpy(buffer, "Non puoi ancora prendere questo oggetto.\n");
                        send(client_socket, buffer, DIM_BUFFER, 0);
                        printf("Oggetto non raccoglibile\n");
                        return;
                    }
                }
                /* Se l'oggetto richiede una risposta a un indovinello */
                if(room->items[i].answering){
                    int ret;
                    ret = recv(client_socket, buffer, DIM_BUFFER, 0);
                    if(ret <= 0){
                        perror("ERRORE: Errore nella ricezione della risposta");
                    }
                    /* Controlla se la risposta dell'utente è corretta */
                    struct Enigma *enigma;
                    enigma = &room->items[i].enigma;
                    if(!strcmp(buffer, enigma->solution)){
                        /* Se la risposta è corretta, aggiorna la descrizione e sblocca l'oggetto */
                        if(!strcmp(room->items[i].name, "scatola")){
                            strcpy(buffer,  "Risposta corretta.\n"
                                            "La scatola si è aperta e dentro ci sono delle **forbici**\n"
                                            "e un codice segreto per ottenere dei **bitcoin4**\n");
                            strcpy(room->items[i].description,  "Una scatola con dentro delle **forbici**\n"
                                                                "e un codice segreto per ottenere dei **bitcoin4**\n");
                            room->items[5].isLocked = false;
                            room->items[5].hidden = false;
                            room->items[9].isLocked = false;
                            send(client_socket, buffer, DIM_BUFFER, 0);
                            printf("Risposta corretta\n");
                        }
                        else if(!strcmp(room->items[i].name, "cassetto")){
                            strcpy(buffer,  "Risposta corretta.\n"
                                            "Dentro al cassetto si trovano una **batteria**,\n"
                                            "e un codice per ottenere dei **bitcoin3**.\n"
                                            "Sicuramete la batteria serve a qualcosa\n");
                            strcpy(room->items[i].description,  "Un cassetto con dentro una **batteria\n"
                                                                "e un codice per ottenere dei **bitcoin3**\n");
                            room->items[3].isLocked = false;
                            room->items[3].hidden = false;
                            room->items[8].isLocked = false;
                            send(client_socket, buffer, DIM_BUFFER, 0);
                            printf("Risposta corretta\n");
                        }
                        room->items[i].isLocked = 0;
                        return;
                    }
                    else{
                        strcpy(buffer, "Risposta errata.\n");
                        send(client_socket, buffer, DIM_BUFFER, 0);
                        printf("Risposta errata\n");
                        return;
                    }
                    room->items[i].answering = 0;
                }
            }
            else{
                /* Se l'oggetto è già stato preso */
                strcpy(buffer, "Oggetto già raccolto.\n");
                send(client_socket, buffer, DIM_BUFFER, 0);
                printf("Oggetto già raccolto\n");
                return;
            }
        }
    }

    /* Se l'oggetto non esiste nella stanza */
    strcpy(buffer, "Oggetto inesistente\n");
    send(client_socket, buffer, DIM_BUFFER, 0); 
    printf("Oggetto inesistente\n");
}


/**
 * Funzione per gestire il comando 'objs', che mostra l'elenco degli oggetti raccolti dal giocatore
 * @param game Puntatore alla struttura 'Game' che contiene le informazioni sul gioco, inclusi gli oggetti raccolti
 * @param target1 Primo parametro del comando, che dovrebbe essere vuoto per un comando corretto
 * @param target2 Secondo parametro del comando, che dovrebbe essere vuoto per un comando corretto
 * @param client_socket Socket attraverso il quale inviare la risposta al client con l'elenco degli oggetti
 */
void commandObjs(struct Game *game, char *target1, char *target2, int client_socket){
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER);
    int i = 0;

    /* Verifica se i parametri target1 o target2 sono diversi da vuoti */
    if(target1[0] != '\0' || target2[0] != '\0'){
        strcpy(buffer, "Formato del comando objs non valido, il formato corretto è -> objs\n");
        send(client_socket, buffer, DIM_BUFFER, 0);  
        printf("Formato non valido\n");
        return; 
    }

    /* Costruisce la lista degli oggetti presi dal giocatore */
    while(i < game->itemTaked){
        strcat(buffer, game->items[i].name); 
        strcat(buffer, "\n"); 
        i++;
    }

    /* Invia la lista degli oggetti raccolti al client */
    send(client_socket, buffer, DIM_BUFFER, 0);

    printf("Comando objs andato a buon fine\n"); 
}