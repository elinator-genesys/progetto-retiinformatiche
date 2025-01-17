#include "utility.h"

struct User *users = NULL;
struct Game *games = NULL;
int numGame = 0;

/**
 * Funzione che invia le istruzioni per il login e la registrazione
 * al client, fornendo informazioni su come iniziare.
 * @param buffer Buffer in cui vengono scritte le istruzioni da inviare al client
 */
void instructionLogin(char *buffer){
    strcpy(buffer,  "Benvenuto, mettiti alla prova disinnescando diverse bombe, una più diffcile dell'altra.\n"
                    "Prima di tutto registrati oppure effettua il login\n"
                    "- login username password -> comando login\n"
                    "- signup username password -> comando registrazione\n"
                    "Se vuoi uscire usa il comando:\n"
                    "- end");
}

/**
 * Funzione che invia le istruzioni di gioco al client, descrivendo
 * i comandi disponibili per interagire con il gioco.
 * @param buffer Buffer in cui vengono scritte le istruzioni da inviare al client
 */
void instructionCommand(char *buffer){
    strcpy(buffer,  "\n******************************************************\n"
                    "Ora che hai effettuato il login ti spiego come funziona,\n"
                    "per disinnescare la bomba hai a disposizione i seguenti comandi:\n"
                    "- look [location | object]: senza argomenti rende la descrizione dello scenario, altrimenti la descrizione dell'oggetto/location in questione.\n"
                    "- take object: raccoglie l'oggetto <obj> indicato come parametro.\n"
                    "- use object1 [object2]: permette di utilizzare l'oggetto <obj> indicato come parametro o di usarlo in maniera combinata con l'oggetto <obj2>.\n"
                    "- objs: rende la lista degli oggetti raccolti.\n"
                    "Bene ora sai tutto ciò che ti serve.\n"
                    "Per iniziare usa il comando:\n"
                    "- start player room\n"
                    "Giocatori disponibili:\n"
                    "1 - Giocatore principale\n"
                    "2 - Giocatore secondario\n"
                    "Room disponibili:\n"
                    "1 - Bomba facile\n"
                    "Seleziona il tipo di giocatore e la room che vuoi giocare.\n"
                    "Il tempo partirà fin da subito, BUONA FORTUNA!\n"
                    "******************************************************\n");
}

/**
 * Funzione per gestire il login di un utente, confrontando il nome utente
 * e la password forniti con quelli registrati nel sistema.
 * @param username Nome utente fornito dall'utente per il login
 * @param password Password fornita dall'utente per il login
 * @param client_socket Socket utilizzato per inviare la risposta di login al client
 */
void login(char *username, char *password, int client_socket) {
    struct User *u;
    int ret;
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER);

    /*Controlla se l'utente esiste*/
    for (u = users; u; u = u->next) {
        if (strcmp(u->username, username) == 0 && strcmp(u->password, password) == 0) {
            u->logged = 1;
            printf("Login effettuato!\n");
            instructionCommand(buffer);
            ret = send(client_socket, buffer, DIM_BUFFER, 0);
            if(ret <= 0){
                printf("Errore nell'invio");
            }
        }
    }
}

/**
 * Funzione per registrare un nuovo utente, creando una nuova struttura
 * 'User' e aggiungendola alla lista degli utenti registrati.
 * @param username Nome utente scelto dall'utente per la registrazione
 * @param password Password scelta dall'utente per la registrazione
 * @param client_socket Socket utilizzato per inviare la conferma di registrazione al client
 */
void registerUser(char *username, char *password, int client_socket){
    struct User *u;

    u = malloc(sizeof(struct User));
    strcpy(u->username, username);
    strcpy(u->password, password);
    u->socket = client_socket;
    u->next = NULL;

    if(users){
        u->next = users;
        users = u;
    }
    else{
        users = u;
    }

    printf("Registrazione completata!\n");
}

/**
 * Funzione per gestire il logout di un utente, segnando l'utente come non loggato
 * e resettando il suo socket.
 * @param u Puntatore alla struttura 'User' che rappresenta l'utente da disconnettere
 */
void logout(struct User *u){
    if(u){
        u->logged = false;
        u->socket = -1;
    }
}

/**
 * Funzione per creare un nuovo gioco, assegnandogli una stanza e
 * inizializzando le informazioni relative ad esso.
 * @param client_socket Socket utilizzato per inviare la descrizione della stanza al client
 */
void createGame(int client_socket){
    char buffer[DIM_BUFFER];
    struct Game *g;
    g = malloc(sizeof(struct Game));
    g->id = ++numGame;

    g->room = malloc(sizeof(struct Room));
    g->room = initializeRoom(1);

    g->first = findUserBySocket(client_socket);
    g->second = NULL;
    g->itemTaked = 0;
    g->solved = 0;

    g->tokenTaked = 0;

    g->texting = false;
    
    g->startTime = time(NULL);

    g->next = NULL;

    if(games){
        g->next = games;
        games = g;
    }
    else {
        games = g;
    }

    memset(buffer, 0, DIM_BUFFER);
    strcpy(buffer, g->room->description);
    send(client_socket, buffer, sizeof(buffer), 0);
}

/**
 * Funzione per ottenere il gioco associato a un determinato client.
 * @param client_socket Socket del client di cui si vuole ottenere il gioco associato
 * @return Puntatore alla struttura 'Game' associata al client, oppure NULL se non trovato
 */
struct Game *getGame(int client_socket){
    struct Game *g = NULL;

    for (g = games; g; g = g->next)
    {
        if(g->first && g->first->socket == client_socket){ /* giocatore principale */
           break;
        }
        else if(g->second && g->second->socket == client_socket){ /* giocatore secondario */
            break;
        }
    }

    return g;
}

/**
 * Funzione per liberare la memoria associata a un gioco in base alla stanza.
 * @param room ID della stanza in cui il gioco è in corso
 * @return Puntatore al gioco associato alla stanza, o NULL se non trovato
 */
struct Game *freeGame(int room){
    struct Game *g;

    for(g = games; g; g = g->next){
        if(g->room->id == room && g->second == NULL){
            return g;
        }
    }

    return NULL;
}

/**
 * Funzione per eliminare un gioco dalla lista dei giochi attivi.
 * @param game ID del gioco da eliminare
 */
void deleteGame(int game){
    struct Game* current = games, *previous = NULL;

    while(current && current->id != game)
    {
        previous = current;
        current = current->next;
    }

    if(current)
    {
        if(!previous){
            games = games->next;
        } 
        else{
            previous->next = current->next;
        }

        free(current);
    }
}

/**
 * Funzione per trovare un utente in base al nome utente.
 * @param username Nome utente da cercare nella lista degli utenti
 * @return Puntatore alla struttura 'User' associata al nome utente, oppure NULL se non trovato
 */
struct User *findUserByUsername(char *username){
    struct User *u;

    for(u = users; u; u = u->next){
        if(!strcmp(u->username,username)){
            return u;
        }
    }

    return NULL;
}

/**
 * Funzione per trovare un utente in base al socket.
 * @param client_socket Socket del client da cercare nella lista degli utenti
 * @return Puntatore alla struttura 'User' associata al socket, oppure NULL se non trovato
 */
struct User *findUserBySocket(int client_socket){
    struct User *u;

    for(u = users; u; u = u->next){
        if(u->socket == client_socket){
            return u;
        }
    }

    return NULL;
}

/**
 * Funzione per eliminare tutti gli utenti dalla lista.
 */
void deleteUsers(){
    struct User* u;
    while(users){
        u = users;
        users = users->next;
        free(u);
    }
}

/**
 * Funzione per inviare un enigma al client.
 * @param item Puntatore all'oggetto 'Item' che contiene l'enigma da inviare
 * @param client_socket Socket del client a cui inviare l'enigma
 */
void sendEnigma(struct Item *item, int client_socket) {
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER);
    int ret;

    /* Invia l'enigma specifico al client*/
    strcpy(buffer, item->enigma.question);
    ret = send(client_socket, buffer, DIM_BUFFER, 0);
    if(ret <= 0){
        perror("ERRORE: Errore nell'invio dell'enigma");
    }
    item->answering = 1;
}

/**
 * Funzione per verificare se ci sono giochi attivi.
 * @return 1 se ci sono giochi attivi, 0 se non ci sono giochi attivi
 */
int gameOn(){
    return games != NULL ? 1 : 0;
}

/**
 * Funzione che restituisce un valore hash per il comando ricevuto, utilizzato per identificare
 * i comandi inviati dal client.
 * @param game Puntatore alla struttura 'Game' che contiene informazioni sul gioco
 * @param str Stringa del comando inviato dal client
 * @return Un valore intero che rappresenta il comando identificato
 */
int hashString(struct Game *game, const char *str) {
    if(game == NULL){
        if (strcmp(str, "signup") == 0) return 1;
        if (strcmp(str, "login") == 0) return 2;
        if (strcmp(str, "start") == 0) return 3;
        if (strcmp(str, "end") == 0) return 4;
        return 9;
    }
    if(game->texting == true) return 10;
    if (strcmp(str, "end") == 0) return 4;
    if (strcmp(str, "look") == 0) return 5;
    if (strcmp(str, "take") == 0) return 6;
    if (strcmp(str, "use") == 0) return 7;
    if (strcmp(str, "objs") == 0) return 8;
    return 0; /*Valore di default per comandi non riconosciuti*/
}

/**
 * Funzione per inviare informazioni sullo stato del gioco al client,
 * inclusi il tempo rimanente e il numero di token posseduti.
 * @param game Puntatore alla struttura 'Game' che contiene informazioni sul gioco in corso
 * @param target1 Parametro non utilizzato nella funzione
 * @param target2 Parametro non utilizzato nella funzione
 * @param client_socket Socket utilizzato per inviare le informazioni al client
 * @param master Puntatore al set di descrittori dei file
 */
void gameInfos(struct Game *game, char *target1, char *target2, int client_socket, fd_set *master){
    char buffer[DIM_BUFFER];
    if(game){
        memset(buffer, 0, DIM_BUFFER);
        
        time_t current = time(NULL);
        int p = (int)difftime(current, game->startTime);
        p = MAX_TIME - p;

        /* Se il tempo è scaduto e la partita non è stata vinta */
        if(p < 0 && game->win == false){
            strcpy(buffer, "BOOOOOOOOOM LA BOMBA È ESPLOSA!!!\n");
            send(client_socket, buffer, DIM_BUFFER, 0);
            memset(buffer, 0, DIM_BUFFER);
            commandEnd(game, '\0', '\0', client_socket, master);
        }
        sprintf(buffer, "******************************************************\n"
                        "\nGAME INFO - Tempo rimanente: %d "
                        "secondi, Token posseduti: %d - rimanenti: %d\n"
                        "\n******************************************************\n",
                        p, game->tokenTaked, game->room->tokenCount - game->tokenTaked);
        send(client_socket, buffer, DIM_BUFFER, 0);
    }
}

/**
 * Funzione per gestire la comunicazione tra il primo e il secondo giocatore,
 * in particolare per la gestione della risposta del secondo giocatore.
 * @param game Puntatore alla struttura 'Game' che contiene informazioni sul gioco in corso
 * @param i Identificativo del socket del giocatore che sta inviando la richiesta
 * @param action Azione richiesta dal giocatore
 * @param master Puntatore al set di descrittori dei file
 */
void PcTexting(struct Game *game, int i, char *action, fd_set *master){ 
    char buffer[DIM_BUFFER];
    memset(buffer, 0, DIM_BUFFER);
    struct Room *room = game->room;

    if(game->first->socket == i){
        /* Invia messaggio al primo client che aspetta la risposta del secondo client */
        strcpy(buffer, "*** In attesa di risposta dal client secondario ***\n");
        send(game->first->socket, buffer, DIM_BUFFER, 0); 
    }
    else{
        if(atoi(action) >= game->room->tokenCount || atoi(action) <= 0){
            /* Invia errore se la richiesta è fuori range di token disponibili */
            printf("Risposta dell'utente del dark web non valida");
            sprintf(buffer, "Richiesta errata, non puoi richiedere un valore inferiore 0 uguale a 0 o superiore a %d\n", game->room->tokenCount);
            send(game->second->socket, buffer, DIM_BUFFER, 0); 
        }
        else if(atoi(action) > game->tokenTaked){
            /* Invia errore se l'utente non ha abbastanza token per la richiesta */
            printf("Risposta dell'utente del dark web non valida");
            strcpy(buffer, "Non hai sufficenti bitcoin per pagare la richiesta dell'utente del dark web, cercane altri\n");
            send(game->first->socket, buffer, DIM_BUFFER, 0); 
            memset(buffer, 0, DIM_BUFFER);
            strcpy(buffer, "La richiesta è stata troppo elevata, attendi una nuova connessione");
            send(game->second->socket, buffer, DIM_BUFFER, 0); 
            game->texting = false;
        }
        else{
            /* Esegui l'azione valida e aggiorna i token, gestendo la vittoria se completato */
            game->tokenTaked = game->tokenTaked - atoi(action);
            room->tokenCount = room->tokenCount - atoi(action);
            
            if(room->items[5].used == false){
                /* Gestisci l'azione del cavo e verifica la vittoria */
                game->solved++;
                strcpy(buffer, "L'utente del dark web è riuscito a tagliare il cavo da remoto!!\n");
                send(game->first->socket, buffer, DIM_BUFFER, 0);
                memset(buffer, 0, DIM_BUFFER);
                strcpy(buffer, "Hai aiutato il disinnescatore tagliando il cavo della bomba!\n");
                send(game->second->socket, buffer, DIM_BUFFER, 0);
                if(game->solved == 2){
                    game->win = true;
                    strcpy(buffer, "Hai disinnescato la bomba in tempo, complimenti.\n");
                    send(game->first->socket, buffer, DIM_BUFFER, 0);
                    commandEnd(game, '\0', '\0', game->first->socket, master);
                }
                room->items[5].used = true;
            }
            else if(room->items[2].used == false || room->items[3].used == false)
            {
                /* Gestisci l'azione del codice segreto e verifica la vittoria */
                strcpy(buffer,  "L'utente del dark web ha sboccato il cdice del tastierino da remoto!!.\n");
                send(game->first->socket, buffer, DIM_BUFFER, 0);
                memset(buffer, 0, DIM_BUFFER);
                strcpy(buffer, "Hai aiutato il disinnescatore inserendo il codice segreto!\n");
                send(game->second->socket, buffer, DIM_BUFFER, 0);
                game->solved++;
                if(game->solved == 2){
                    game->win = true;
                    strcpy(buffer, "Hai disinnescato la bomba in tempo, complimenti.\n");
                    send(game->first->socket, buffer, DIM_BUFFER, 0);
                    commandEnd(game, '\0', '\0', game->first->socket, master);
                }
                room->items[2].used = true;
                room->items[3].used = true;
            }
        }
    }
    /* Termina la comunicazione */
    game->texting = false;
}
