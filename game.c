#include <stdio.h>

#include "defs.h"

/*
 * Purpose: starts up the game (GUI; handles user input, output and sending and receiving data from the other process)
 * in: specifies whether the current process gets the first turn (game)
 * in: specifies the socket used for sending and receiving data 
 * return: server: opponent choose to leave the game (waiting for another game process to connect)
 *         client: opponent choose to leave the game (converting client to server)
 */
int game(int turn, int mySocket){
    int bytesRcv;
    char receivingBuffer[MAX_STR];
    char sendingBuffer[MAX_STR];

    //game variables
	int myTurn;
	int option;
	char intBuffer[MAX_STR];
	char guessWord[MAX_STR];

	int first;
    int win;

	char myWord[MAX_STR];
    int myWordHidden[MAX_STR];
    char opponentWord[MAX_STR];
    int opponentWordHidden[MAX_STR];

    //sets up the game variables
    setGame(turn, &myTurn, &win, &first, myWord, myWordHidden, opponentWordHidden, mySocket, sendingBuffer);

    while(1){
        //gets data from the other process
        bytesRcv = recv(mySocket, receivingBuffer, sizeof(receivingBuffer), 0);
        receivingBuffer[bytesRcv] = '\0';

        //if actual data is received from the other process
        if (bytesRcv > 0){
            //if the other process sent the guessing word
            if (first){
                strcpy(opponentWord, receivingBuffer);
                first = 0;
            } else {
                myTurn = 0;
                //if the other process wants to replay
                if (strcmp(receivingBuffer, "--replay--") == 0){
                    printf("Opponent has choosen to replay\n");

                    //resets the game
                    setGame(0, &myTurn, &win, &first, myWord, myWordHidden, opponentWordHidden, mySocket, sendingBuffer);
                } 
                //if the other process if leaving 
                else if (strcmp(receivingBuffer, "--leave--") == 0){
                    printf("Opponent has choosen to leave\n");
                    if (turn){
                        printf("Converting game process to a server...\n");
                    }
                    return 1;
                } 
                //if a singal byte(letter) is received 
                else if (bytesRcv == 1){
                    //if the word does not contain the letter
                    if (!findLetter(receivingBuffer, myWord, myWordHidden)){
                        printf("Opponent guessed the wrong letter\n");
                        myTurn = 1;
                    } 
                    //if the word contained the letter
                    else {
                        //checks if the opponent won
                        if (checkWin(myWord, myWordHidden)){
                            //sets win to lossing state
                            win = -1;
                        } else {
                            myTurn = 1;
                        }
                    }
                } else {
                    //if the opponent guessed the correct word
                    if (strcmp(myWord, receivingBuffer) == 0){
                        //displays the entire word
                        set(1, myWordHidden);
                        //sets win to lossing state
                        win = -1;
                    } 
                    //if the opponent guessed the wrong word
                    else {
                        printf("Opponent guessed the wrong word\n");
                        myTurn = 1;
                    }
                }
            }
        }

        //prints out the current conditions of the words
        printf("-----------------------------------------------------\n");
        printf("My Word: ");
        displayWord(myWord, myWordHidden);

        printf("Opponent's Word: ");
        displayWord(opponentWord, opponentWordHidden); 
        printf("-----------------------------------------------------\n");

        //if its the users turn
        while(myTurn){
            printf("Would you like to (1) guess a letter (2) guess the word? ");
            fgets(intBuffer, sizeof(intBuffer), stdin);
            sscanf(intBuffer, "%d", &option);

            switch(option){
                //if the user choose to guess a letter
                case 1:
                    printf("Enter the letter you would like to guess: ");
                    fgets(guessWord, sizeof(guessWord), stdin);
                    guessWord[strlen(guessWord) - 1] = '\0';

                    //sends the letter to the other process
                    strcpy(sendingBuffer, guessWord);
                    send(mySocket, sendingBuffer, strlen(sendingBuffer), 0);

 
                    //if the user guessed the wrong letter
                    if (!findLetter(guessWord, opponentWord, opponentWordHidden)){
                        printf("You guessed the wrong letter\n");
                    } 
                    //if the user guessed the correct letter
                    else {
                        //checks if the user won
                        if (checkWin(opponentWord, opponentWordHidden)){
                            //sets win to winning state
                            win = 1;
                        }
                    }

                    //ends the users turn
                    myTurn = 0;
                    break;
                //if the user choose to guess the entire word
                case 2:
                    printf("Enter the word you would like to guess: ");
                    fgets(guessWord, sizeof(guessWord), stdin);
                    guessWord[strlen(guessWord) - 1] = '\0';

                    //sends the word to the other process
                    strcpy(sendingBuffer, guessWord);
                    send(mySocket, sendingBuffer, strlen(sendingBuffer), 0);

                    //if the user guessed the correct word
                    if (strcmp(guessWord, opponentWord) == 0){
                        //displays the entire word
                        set(1, opponentWordHidden);
                        //sets win to winning state
                        win = 1;
                    } 
                    //if the user guessed the wrong word
                    else {
                        printf("You guessed the wrong word\n");
                    }

                    //ends the users turn
                    myTurn = 0;
                    break;
                //if an invalid option is entered
                default:
                    printf("Invalid option, please choose again\n");
                    break;
            }
        }

        //prints out the current conditions of the words
        printf("-----------------------------------------------------\n");
        printf("My Word: ");
        displayWord(myWord, myWordHidden);

        printf("Opponent's Word: ");
        displayWord(opponentWord, opponentWordHidden); 
        printf("-----------------------------------------------------\n");
        
        //if the user won
        if (win == 1){
            printf("You have won the game\n");
            do{
                printf("Would you like to (1) start a new game (2) leave the game? ");
                fgets(intBuffer, sizeof(intBuffer), stdin);
                sscanf(intBuffer, "%d", &option);

                switch(option){
                    //if the user wants to replay
                    case 1:
                        //tells the other process the user wants to replay
                        strcpy(sendingBuffer, "--replay--");
                        send(mySocket, sendingBuffer, strlen(sendingBuffer), 0);
                        
                        //resets the game
                        setGame(1, &myTurn, &win, &first, myWord, myWordHidden, opponentWordHidden, mySocket, sendingBuffer);

                        break;
                    //if the user is leaving
                    case 2:
                        //tells the other process the user is leaving
                        strcpy(sendingBuffer, "--leave--");
                        send(mySocket, sendingBuffer, strlen(sendingBuffer), 0);
                        printf("Thankyou for playing the game, enjoy your win!\n");
                        return 0;
                        break;
                    default:
                        printf("Invalid option, please choose again\n");
                        break;
                }
            } while(!(option == 1 || option == 2));
             
        } 
        //if the user lost 
        else if (win == -1){
            printf("Opponent has won the game!\n");
            printf("Waiting for opponent's decision...\n");
        }
    }
}

/*
 * Purpose: setup the game variables
 * in: specifies the turn
 * out: stores the turn of the user
 * out: stores the win state of the user
 * out: stores if the first byte after the game started was sent (for guessing word) 
 * out: stores the user's word
 * out: stores the "revealed" state for the letters of the user's word
 * out: stores the "revealed" state for the letters of the opponent's word
 * in: socket used for communication between server and client
 * in-out: stores the data thats being sent to the other process
 */
void setGame(int turn, int *myTurn, int *win, int *first, char *myWord, int *myWordHidden, int *opponentWordHidden, int mySocket, char *sendingBuffer){
    *myTurn = turn;
    *win = 0;
    *first = 1;

    //sets the variables to 0
    set(0, myWordHidden);
    set(0, opponentWordHidden);

    //gets the user's guessing word
    char word[MAX_STR];
    printf("Enter your word: ");
    fgets(word, sizeof(word), stdin);
    word[strlen(word) - 1] = '\0';
    printf("length of word: %d\n", strlen(word));

    strcpy(myWord, word);
    strcpy(sendingBuffer, word);
    //sends the guessing word to the other process
    send(mySocket, sendingBuffer, strlen(sendingBuffer), 0);
    
}

/*
 * Purpose: sets the arrays values to specified value
 * in: specified value the values of the array are set to
 * out: array being modified
 */
void set(int option, int *wordHidden){
    int i;
    for (i = 0; i < MAX_STR; i++){
        wordHidden[i] = option;
    }
}

/*
 * Purpose: checks if entire word has be revealed
 * in: word being checked
 * in: array that stores the "revealed" state of the word
 * return: if the word is fully revealed
 */
int checkWin(char *word, int *wordHidden){
    int returning = 1;
    int i; 
    for (i = 0; i < strlen(word); i++){
        if (!wordHidden[i]){
            returning = 0;
        }
    }
    return returning;
}

/*
 * Purpose: prints out the given word, based on the "reveal" state of the int array
 * in: word being printed
 * in: array that stores the "revealed" state of the word
 */
void displayWord(char *word, int *wordHidden){
    int i;
    for (i = 0; i < strlen(word); i++){
        if (wordHidden[i]){ 
            printf("%c", word[i]);
        } else{ 
            printf("*");
        }
    } 
    printf("\n");
}

/*
 * Purpose: finds if the letter is within the word
 * in: letter being found
 * in: word where the letter is being found
 * out: array that stores the "revealed" state of the word
 * return: if the letter was found
 */
int findLetter(char *letter, char *word, int *wordHidden){
    int returning = 0;
    int i; 
    for (i = 0; i < strlen(word); i++){
        if (letter[0] == word[i]){
            wordHidden[i] = 1;
            returning = 1;
        }
    }
    return returning;
}





