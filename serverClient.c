#include <stdio.h>

#include "defs.h"

#define SERVER_PORT 60003
/*
 * Purpose: creates a server or client process depending on server variable
 * in: specifies whether to create a server or client process
 * in: ip address the server/client process uses
 */
void createServerClient(int server, char *SERVER_IP){
    int mySocket, myListenSocket;
    struct sockaddr_in addr, clientAddr;
    int i, addrSize;

 	//create socket
    if (server){
        //server
        myListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (myListenSocket < 0){
            printf("couldn't open socket\n");
            exit(-1);
        }
    } else {
        //client
	    mySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (mySocket < 0){
            printf("couldn't open socket\n");
            exit(-1);
        }
    }

    //setup my server address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if (server){
        //server
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        //client
        addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    }        
	addr.sin_port = htons((unsigned short) SERVER_PORT);

	if (server){
        //server
        //bind my listen socket
        i = bind(myListenSocket, (struct sockaddr *) &addr, sizeof(addr));

	    if (i < 0){
            printf("couldn't bind socket\n");
            exit(-1);
        }

	    //listen
        i = listen(myListenSocket, 1);
        if (i < 0){
            printf("couldn't listen\n");
            exit(-1);
        }

        do{
            //wait for connection request
            printf("Waiting for another game processing to connect...\n");
            addrSize = sizeof(clientAddr);

            mySocket = accept(myListenSocket, (struct sockaddr *) &clientAddr, &addrSize);
            if (mySocket < 0){
                printf("couldn't accept the connection\n");
                exit(-1);
            }
        } 
        //starts up the game, once a client has connected 
        while(game(0, mySocket));
	} else {
        //client
	    //connect to server
        i = connect(mySocket, (struct sockaddr *) &addr, sizeof(addr));
        if (i < 0){
            printf("client could not connect!\n");
            exit(-1);
        }
        //starts up the game, once connected to a server
        if (game(1, mySocket)){
            //if the if statement triggers, converts the current client process into a server process
            close(mySocket);
            //waits for the socket to be useable again
            sleep(70);
            createServerClient(1, SERVER_IP);
        }
	}
}
