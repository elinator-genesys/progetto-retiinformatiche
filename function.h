#ifndef FUNCTION_H
#define FUNCTION_H

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "structure.h"

struct Room *initializeRoom(int id);
void commandSignup(char *target1, char *target2, int client_socket);
void commandLogin(char *target1, char *target2, int client_socket);
void commandStart(struct Game *game, char *target1, char *target2, int client_socket);
void commandEnd(struct Game *game, char *target1, char *target2, int client_socket, fd_set *master);
void commandLook(struct Game *game, char *target1, char *target2, int client_socket);
void commandTake(struct Game *game, char* target1, char *target2, int client_socket);
void commandUse(struct Game *game, char *target1, char *target2, int client_socket, fd_set *master);
void commandObjs(struct Game *game, char *target1, char *target2, int client_socket);

#endif