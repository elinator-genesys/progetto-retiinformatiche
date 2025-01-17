#ifndef UTILITY_H
#define UTILITY_H

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

#include "structure.h"
#include "function.h"

void instructionLogin(char *buffer);
void instructionCommand(char *buffer);
void login(char *username, char *password, int client_socket);
void registerUser(char *username, char *password, int client_socket);
void logout(struct User *u);
void createGame(int client_socket);
struct Game *getGame(int i);
struct Game *freeGame(int room);
void deleteGame(int game);
struct User *findUserByUsername(char *username);
struct User *findUserBySocket(int client_socket);
void deleteUsers();
void sendEnigma(struct Item *item, int client_socket);
int gameOn();
int hashString(struct Game *game, const char *str);
void gameInfos(struct Game *game, char *target1, char *target2, int client_socket, fd_set *master);
void PcTexting(struct Game *game, int i, char *action, fd_set *master);

#endif