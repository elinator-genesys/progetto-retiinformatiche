#ifndef STRUCTURE_H
#define STRUCTURE_H

#define DIM_BUFFER 1024
#define MAX_TOKENS 8
#define MAX_ITEMS 20
#define MAX_LOCATION 5
#define MAX_ITEMS_TAKED 6
#define MAX_TIME 300

typedef enum {false, true} bool;

struct Enigma{
    char question[DIM_BUFFER];
    char solution[DIM_BUFFER];
};

struct Item{
    char name[15];
    char description[DIM_BUFFER];
    char take[DIM_BUFFER];

    bool isLocked; /*Se l'oggetto è chiuso*/
    bool answering;
    bool taked;
    bool token;
    bool hidden;
    bool used;

    struct Enigma enigma;
};

struct Location{
    char name[15];
    char description[DIM_BUFFER];
};

struct Room{
    int id;
    char description[DIM_BUFFER];
    struct Item items[MAX_ITEMS]; /*Oggetti nella stanza*/
    struct Location locations[MAX_LOCATION];
    int itemCount;
    int tokenCount;
};

struct User{
    char username[20];
    char password[20];
    int socket;
    bool logged;

    struct User *next;
};

struct Game{
    int id;
    struct User *first;
    struct User *second;
    struct Room *room;

    int itemTaked;
    struct Item items[MAX_ITEMS_TAKED];

    int solved;
    bool win;

    int tokenTaked;

    bool texting;

    time_t startTime;

    struct Game *next;
};

#endif /* STRUCTURE_H */