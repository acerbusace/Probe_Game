#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_STR 30

void handleSigInt(int);
void createServerClient(int, char*);
int game(int, int);
void setGame(int, int *, int *, int *, char *, int *, int *, int, char *);
void set(int, int *);
int checkWin(char *word, int *wordHidden);
void displayWord(char *, int *);
int findLetter(char *, char *, int *);
