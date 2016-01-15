Program author: 

    Alex Patel and Ibrahim Helmy

Purpose: 

    Create a two-player game similar to Probe. Each player will be prompted to enter a phrase that the other player will try to guess, one letter at a time. Players each will take a turn trying to guess either one letter from the other player's word, or the entire word. The first player to guess the opponent's word wins.

List of files: 

    main.c
    serverClient.c
    game.c
    defs.h

Compilation command:

    make

Lunching command: 

    ./probe                //creates a server
    ./probe 10.0.2.15      //creates a client which connects to the server

Operating instructions:

    The game was developed and tested on ubuntu 14.04 LTS.
    To run the game you need to be using bash shell.

    -create a server by using the above lunch command (./probe)
    -create a client by using the above lunch command (./probe 10.0.2.15)
    -the game will start (client getting the first turn)

    Note: if server decides to leave after winning, the client will wait 70 seconds before attempting to create a server (so the socket can be freed by the os)
