#include <stdio.h>

#include "defs.h"

int main(int argc, char* argv[]){
    //custom signal handler for SIGINT(Ctrl-C)
    //stops the signal from terminating the process
    //signal(SIGINT, handleSigInt);

    //creates the server if no additional command line arguments are entered
    if (argc == 1){
        createServerClient(1, "10.0.2.15");
    } 
    //creates a client process if additional command line arguments are entered (server ip address) 
    else {
        createServerClient(0, argv[1]);
    }
}

/*
 * Purpose: Stops the user from terminating the process using SIGINT(Ctrl-C) 
 * in: not used
 */
void handleSigInt(int signum){
    printf("Not aloud to leave durring a game\n");
}


