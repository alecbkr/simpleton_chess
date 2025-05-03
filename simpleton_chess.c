/*
THIS CHESS IMPLEMENTATION WAS WRITTEN BY ALEC BAKER
alecbakr@gmail.com

ascii art sourced from: 
patorjk.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>



#define ENDOFLIST -777



//=====STRUCTS====================================
typedef struct player {
    int color;
    int kingLoc;
    char graveyard[15];
    int graveyardSize;
} player;

typedef struct piece {
    char type;
    int color;
} piece;

typedef struct tile {
    piece piece;
    int tileIdx;
    struct tile *up;
    struct tile *down;
    struct tile *left;
    struct tile *right;
    struct tile *upLeft;
    struct tile *upRight;
    struct tile *downRight;
    struct tile *downLeft;
} tile;

typedef struct game {
    tile *board;
    player p1;
    player p2;
    int turn;
} game;



//=====PROTOTYPES===================================
void printBoard(tile *, int *); //prototype

//=====MISC HELPERS=================================
//quicksort algorithm, thanks brocode
void quickSort(int *possibleMoves, int start, int end) {
    if (end <= start) return; //basecase

    int pivot = possibleMoves[end];
    int i = start - 1;

    for (int j = start; j <= end - 1; j++) {
        if (possibleMoves[j] < pivot) {
            i++;
            int tempSwitch = possibleMoves[i];
            possibleMoves[i] = possibleMoves[j];
            possibleMoves[j] = tempSwitch;
        }
    }
    i++;
    int tempSwitch = possibleMoves[i];
    possibleMoves[i] = possibleMoves[end];
    possibleMoves[end] = tempSwitch;
    pivot = i;
    quickSort(possibleMoves, start, pivot - 1);
    quickSort(possibleMoves, pivot + 1, end);
}


//====CORE GAMEPLAY================================
//prints tile plainly or with possible moves if move list is provided
void printBoard(tile *board, int *possibleMoves) {
    // tile *tile = currentGame->board;
    int checkerBuffer = 0;
    int j = 0;
    int i;
    int c = 8;

    printf("  %c", 201);
    for (i = 0; i < 17; i++) {
        printf("%c", 205);
    }
    printf("%c\n%d %c ", 187, c--, 186);
    
    for (i = 0; i < 64; i++) {
        piece currPiece = board[i].piece;

        //spacing of tile
        if (i % 8 == 0 && i != 0) {
            printf("%c \n%d %c ", 186, c--, 186);
            checkerBuffer++;
        }

        //if move list is provided to print
        if (possibleMoves != NULL) {
            if (possibleMoves[j] == i) {
                board[i].piece.type == 'e' ? printf("%c ", 248) : printf("%c ", 237);
                j++;
                continue;
            }
        }
        //Decides what to print | UPR = color 0 | lwr = color 1
        if (currPiece.type != 'e')
            currPiece.color == 0 ? printf("%c ", toupper(currPiece.type)) : printf("%c ", currPiece.type);
        else {
            (i + checkerBuffer) % 2 == 0 ? printf("%c ", 178) : printf("%c ", 176);
        }
    }
    //more spacing and set dressing
    printf("%c\n  %c", 186, 200);
    for (i = 0; i < 17; i++) {
        printf("%c", 205);
    }
    printf("%c\n", 188);
    printf("    a b c d e f g h");

    printf("\n\n");
}

//helper for findMoves() | if loopDir is set, func will loop till null ptr is reached.
//returns the number of possible moves recorded
int checkTiles(int *currListAddy, tile *captured_tileIdx, int turn, int loopDir) {
    if (captured_tileIdx == NULL) return 0;

    int i = 0;
    do {
        //Empty tile case
        if (captured_tileIdx->piece.type == 'e') {
            currListAddy[i++] = captured_tileIdx->tileIdx;
        }
        //Enemy tile case
        else if (captured_tileIdx->piece.color != turn) {
            currListAddy[i++] = captured_tileIdx->tileIdx;
            return i;
        }
        //Friendly || OOB case
        else {
            // currListAddy[i++] = 99;
            return i;
        }
        switch (loopDir) {
            case 1: captured_tileIdx = captured_tileIdx->up;        break;//UP
            case 2: captured_tileIdx = captured_tileIdx->down;      break;//DOWN
            case 3: captured_tileIdx = captured_tileIdx->left;      break;//LEFT
            case 4: captured_tileIdx = captured_tileIdx->right;     break;//RIGHT
            case 5: captured_tileIdx = captured_tileIdx->upLeft;    break;//UPLEFT
            case 6: captured_tileIdx = captured_tileIdx->upRight;   break;//UPRIGHT
            case 7: captured_tileIdx = captured_tileIdx->downLeft;  break;//DOWNLEFT
            case 8: captured_tileIdx = captured_tileIdx->downRight; break;//DOWNRIGHT
        }

    } while (captured_tileIdx != NULL && loopDir != 0);
    // if (loopDir == 0) return i - 1;
    return i;
}

//returns every possible move from selected piece and prints tile with the options
int *findMoves(game *activeGame, int tileLoc) {

    piece selectedPiece = activeGame->board[tileLoc].piece;
    if (selectedPiece.type == 'e' || selectedPiece.color != activeGame->turn) return NULL;
    

    tile *selTile = &activeGame->board[tileLoc]; //ptr to tile holding selected piece
    int currTurn = activeGame->turn; //current turn

    int *possibleMoves; //a list of potential moves that can be made
    int numMoves = 0; //counts the number of move options that have been found, serves as index
    
    switch (selectedPiece.type) {
        case 'k':
            possibleMoves = malloc(sizeof(int) * 8);

            numMoves += checkTiles(&possibleMoves[numMoves], selTile->up, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->down, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->left, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->right, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->upLeft, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->upRight, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->downLeft, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->downRight, currTurn, 0);
            break;

        case 'q':
            possibleMoves = malloc(sizeof(int) * 27);

            numMoves += checkTiles(&possibleMoves[numMoves], selTile->up, currTurn, 1);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->down, currTurn, 2);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->left, currTurn, 3);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->right, currTurn, 4);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->upLeft, currTurn, 5);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->upRight, currTurn, 6);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->downLeft, currTurn, 7);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->downRight, currTurn, 8);
            break;

        case 'r':
            possibleMoves = malloc(sizeof(int) * 14);

            numMoves += checkTiles(&possibleMoves[numMoves], selTile->up, currTurn, 1);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->down, currTurn, 2);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->left, currTurn, 3);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->right, currTurn, 4);
            break;

        case 'n':
            possibleMoves = malloc(sizeof(int) * 8);

            if (tileLoc / 8 > 1) {
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->up->upLeft, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->up->upRight, currTurn, 0);
            }

            if (tileLoc / 8 < 7) {
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->down->downLeft, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->down->downRight, currTurn, 0);
            }

            if (tileLoc % 8 > 1) {
                numMoves += checkTiles(&possibleMoves[numMoves], selTile->left->upLeft, currTurn, 0);
                numMoves += checkTiles(&possibleMoves[numMoves], selTile->left->downLeft, currTurn, 0);
            }
            
            if (tileLoc % 8 < 7) {
                numMoves += checkTiles(&possibleMoves[numMoves], selTile->right->upRight, currTurn, 0);
                numMoves += checkTiles(&possibleMoves[numMoves], selTile->right->downRight, currTurn, 0);
            }
            
            break;

        case 'b':
            possibleMoves = malloc(sizeof(int) * 13);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->upLeft, currTurn, 5);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->upRight, currTurn, 6);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->downLeft, currTurn, 7);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->downRight, currTurn, 8);
            break;

        case 'p':
            possibleMoves = malloc(sizeof(int) * 4);
            
            if (currTurn == 0) {
                if (selTile->up != NULL && selTile->up->piece.type == 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->up, currTurn, 0);

                if (selTile->upLeft != NULL && selTile->upLeft->piece.type != 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->upLeft, currTurn, 0);

                if (selTile->upRight != NULL && selTile->upRight->piece.type != 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->upRight, currTurn, 0);

                if (47 < tileLoc && tileLoc < 55 && selTile->up->up->piece.type == 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->up->up, currTurn, 0);
            }

            else {
                if (selTile->down != NULL && selTile->down->piece.type == 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->down, currTurn, 0);

                if (selTile->downLeft != NULL && selTile->downLeft->piece.type != 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->downLeft, currTurn, 0);
    
                if (selTile->downRight != NULL && selTile->downRight->piece.type != 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->upRight, currTurn, 0);
    
                if (7 < tileLoc && tileLoc < 16 && selTile->down->down->piece.type == 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->down->down, currTurn, 0);
            }

            break;
    }
    numMoves--; //removes anticipation value, stabilizes | idk why this is here tbh
    if (numMoves + 1 == 0) return NULL;

    quickSort(possibleMoves, 0, numMoves);
    possibleMoves[numMoves + 1] = ENDOFLIST;
    return possibleMoves;
}

//removes piece from current tile, places on new tile
//if a piece is taken, it is added to owners graveyard
void movePiece(game *chessgame, int piece_tileIdx, int captured_tileIdx) {
    piece movedPiece = chessgame->board[piece_tileIdx].piece;
    piece takenPiece = chessgame->board[captured_tileIdx].piece;

    //if piece is taken, add to the owners graveyard
    if (takenPiece.type != 'e') {
        
        if (takenPiece.color == 0) {
            if (takenPiece.type == 'k') chessgame->p1.kingLoc = -1;
            chessgame->p1.graveyard[(chessgame->p1.graveyardSize)++] = takenPiece.type;
        }
        else {
            if (takenPiece.type == 'k') chessgame->p2.kingLoc = -1;
            chessgame->p2.graveyard[(chessgame->p2.graveyardSize)++] = takenPiece.type;
        }
    }

    //make move
    chessgame->board[piece_tileIdx].piece.type = 'e';
    chessgame->board[piece_tileIdx].piece.color = -1;
    chessgame->board[captured_tileIdx].piece = movedPiece;

    //update king location if needed
    if (movedPiece.type == 'k') {
        if (chessgame->turn == 0) chessgame->p1.kingLoc = captured_tileIdx;
        else if (chessgame->turn == 1) chessgame->p2.kingLoc = captured_tileIdx;
    }

    //check for pawn promotion condition
    else if (movedPiece.type == 'p' && (captured_tileIdx <= 7 || 56 <= captured_tileIdx )) {
        printf("promote placeholder\n");
    }
}

int isKingThreatened(game *chessgame, player p) {

    for (int i = 0; i < 8; i++) {
        tile *checkedTile = &chessgame->board[p.kingLoc];
        int isFirstPass = 1; //for pawn threat check

        do {
            switch (i) {
                case 0: checkedTile = checkedTile->up; break;
                case 1: checkedTile = checkedTile->right; break;
                case 2: checkedTile = checkedTile->down; break;
                case 3: checkedTile = checkedTile->left; break;
                case 4: checkedTile = checkedTile->upLeft; break;
                case 5: checkedTile = checkedTile->upRight; break;
                case 6: checkedTile = checkedTile->downRight; break;
                case 7: checkedTile = checkedTile->downLeft; break;
            }
            //edge of board
            if (checkedTile == NULL) break;
            
            //piece detected
            if (checkedTile->piece.type != 'e') {
                //piece is enemy
                if (checkedTile->piece.color != p.color) {
                    //check horizontal/vertical 
                    if (0 <= i && i <= 3) {
                        
                        if (checkedTile->piece.type == 'q' ||
                            checkedTile->piece.type == 'r' ) return 1;
                    }
                    //check diagonals
                    else {
                        
                        if (checkedTile->piece.type == 'q' ||
                            checkedTile->piece.type == 'b' ) return 1;

                        else if (checkedTile->piece.type == 'p' && isFirstPass) {
                            if (p.color == 0 && (i==4 || i==5)) return 1;
                            else if (p.color == 1 && (i==6 || i==7)) return 1;
                            else break;
                        }
                    }
                    break;
                }
                else {
                    break;
                }

            }
            isFirstPass = 0;
        } while (checkedTile != NULL || checkedTile->piece.type == 'e');
    }
    return 0;
}

//notifies if king is in check 
int checkGameState(game *chessgame) {
    if (chessgame->p1.kingLoc == -1 || chessgame->p2.kingLoc == -1) {
        chessgame->turn == 0 ? printf("UPR wins!\nThanks for playing\n\n") : printf("lwr wins!\nThanks for playing\n\n");
        return 1; //king down, game over
    }


    //check if checked
    if (isKingThreatened(chessgame, chessgame->p1)) printf("\nUPR king is checked!\n");
    if (isKingThreatened(chessgame, chessgame->p2)) printf("\nlwr king is checked!\n");
}

//Creates 8x8 tile and sets player turn
game *initGame() {
    game *newGame = malloc(sizeof(game));
    newGame->p1.color = 0;
    newGame->p1.graveyardSize = 0;
    newGame->p2.color = 1;
    newGame->p2.graveyardSize = 0;
    newGame->board = malloc(sizeof(tile) * 64);


    tile *tempBoard = newGame->board;

    //Links all tile tiles together and sets each empty
    for (int i = 0; i < 64; i++) {
        tempBoard[i].piece.type = 'e';
        tempBoard[i].piece.color = -1;
        tempBoard[i].tileIdx = i;

        i + 1 < 8*((i+8)/8) ? tempBoard[i].right = &tempBoard[i + 1] : (tempBoard[i].right = NULL); //Right
        i - 1 >= 8*(i/8) ? tempBoard[i].left = &tempBoard[i - 1] : (tempBoard[i].left = NULL); //Left
        i + 8 < 64 ? tempBoard[i].down = &tempBoard[i + 8] : (tempBoard[i].down = NULL); //Down
        i - 8 >= 0 ? tempBoard[i].up = &tempBoard[i - 8] : (tempBoard[i].up = NULL); //Up
        abs(i - 7) < 8*(i/8) ? tempBoard[i].upRight = &tempBoard[i - 7] : (tempBoard[i].upRight = NULL); //upRight
        i - 9 >= 8*((i-8)/8) ? tempBoard[i].upLeft = &tempBoard[i - 9] : (tempBoard[i].upLeft = NULL); //upLeft

        //downRight
        if (i + 1 < 8*((i+8)/8) && 8*((i+8)/8) < 64) tempBoard[i].downRight = &tempBoard[i + 9];
        else tempBoard[i].downRight = NULL;

        //downLeft
        if (i  > 8*(i/8) && 8*(i/8) < 56) tempBoard[i].downLeft = &tempBoard[i + 7];
        else tempBoard[i].downLeft = NULL;
    }

    newGame->turn = 0;
    return newGame;
}

//place any piece anywhere
void placePiece(game *chessgame, int tileIdx, char type) {
    tile *selTile = &chessgame->board[tileIdx];

    switch (tolower(type)) {
        case 'k': selTile->piece.type = 'k'; break;
        case 'q': selTile->piece.type = 'q'; break;
        case 'r': selTile->piece.type = 'r'; break;
        case 'n': selTile->piece.type = 'n'; break;
        case 'b': selTile->piece.type = 'b'; break;
        case 'p': selTile->piece.type = 'p'; break;
        default: selTile->piece.type = 'e'; return;

    }

    if (isupper(type) == 1) selTile->piece.color = 0;
    else selTile->piece.color = 1;
}

//Sets all pieces in typical chess postions
void setPieces(game *activeGame) {
    
    //UPR
    placePiece(activeGame, 59, 'K'); //king
    activeGame->p1.kingLoc = 59;
    placePiece(activeGame, 60, 'Q'); //queen
    placePiece(activeGame, 56, 'R'); //rook
    placePiece(activeGame, 63, 'R'); //rook
    placePiece(activeGame, 58, 'B'); //bishop
    placePiece(activeGame, 61, 'B'); //bishop
    placePiece(activeGame, 57, 'N'); //knight
    placePiece(activeGame, 62, 'N'); //knight
    placePiece(activeGame, 48, 'P'); //pawn
    placePiece(activeGame, 49, 'P'); //pawn
    placePiece(activeGame, 50, 'P'); //pawn
    placePiece(activeGame, 51, 'P'); //pawn
    placePiece(activeGame, 52, 'P'); //pawn
    placePiece(activeGame, 53, 'P'); //pawn
    placePiece(activeGame, 54, 'P'); //pawn
    placePiece(activeGame, 55, 'P'); //pawn

    //lwr
    placePiece(activeGame, 3, 'k'); //king
    activeGame->p2.kingLoc = 3;
    placePiece(activeGame, 4, 'q'); //queen
    placePiece(activeGame, 0, 'r'); //rook
    placePiece(activeGame, 7, 'r'); //rook
    placePiece(activeGame, 2, 'b'); //bishop
    placePiece(activeGame, 5, 'b'); //bishop
    placePiece(activeGame, 1, 'n'); //knight
    placePiece(activeGame, 6, 'n'); //knight
    placePiece(activeGame, 8, 'p'); //pawn
    placePiece(activeGame, 9, 'p'); //pawn
    placePiece(activeGame, 10, 'p'); //pawn
    placePiece(activeGame, 11, 'p'); //pawn
    placePiece(activeGame, 12, 'p'); //pawn
    placePiece(activeGame, 13, 'p'); //pawn
    placePiece(activeGame, 14, 'p'); //pawn
    placePiece(activeGame, 15, 'p'); //pawn
}



//place custom pieces as specified to test scenarios
void testScenario() {
    game *chessgame = initGame();
    chessgame->p1.kingLoc = -1;
    chessgame->p2.kingLoc = -1;
    int playerInput;
    int *possibleMoves;
    // setPieces(chessgame);
    chessgame->turn = 1;
    int testLoc = 31;
    
    //pieces
    placePiece(chessgame, testLoc, 'K');
    chessgame->p1.kingLoc = testLoc;

    placePiece(chessgame, 63, 'k');
    chessgame->p2.kingLoc = 63;


    placePiece(chessgame, 24, 'r');
    // placePiece(chessgame, testLoc, 'N');
    printBoard(chessgame->board, NULL);


    possibleMoves = findMoves(chessgame, testLoc);
    printBoard(chessgame->board, possibleMoves);
    checkGameState(chessgame);

    
}

//=====INPUT READING=============================================
//Gets user input for menu options
int getMenuInput() {
    char rawUserInput[2];
    
    scanf("%s", rawUserInput);
    
    int temp = strlen(rawUserInput);
    if (temp == 1 && isdigit(*rawUserInput) ) {
        
        int userInput = *rawUserInput - '0';
        if (0 <= userInput || userInput <= 3) return userInput;
    }
    return 0;
}

//Translates user input into grid coordinates
int getUserInput() {
    char userInput[2];
    int col = -1;
    int row = -1;
    int validFlag = 1;

    //repeat while input is invalid
    do {
        printf("Input: ");
        scanf("%s", userInput);

        //Confirm input is 2 chars
        int temp = strlen(userInput);
        if (temp != 2) {
            printf("Command must be a valid tile coordinate (e.g. e4, a1, etc)\n");
            continue;
        }

        //Confirm first char is a letter [a, h]
        switch(tolower(userInput[0])) {
            case 'a': col = 0; break;
            case 'b': col = 1; break;
            case 'c': col = 2; break;
            case 'd': col = 3; break;
            case 'e': col = 4; break;
            case 'f': col = 5; break;
            case 'g': col = 6; break;
            case 'h': col = 7; break;
            default: col = -1; break;
        }
    
        //Confirm second input is a digit [1, 8]
        if (isdigit(userInput[1])) {
            int temp = userInput[1] - '0';
            if (0 < temp && temp < 9) row = 8 - temp;
        }
        
        //Release flag if col and row are valid
        if (col != -1 && row != -1) 
            validFlag = 0;
        else 
            printf("Command must be a valid tile coordinate (e.g. e4, a1, etc)\n");

    } while (validFlag);

    //Calculate tile index
    int result = (row * 8) + col;
    return result;
}

//=====GAME START================================================
//Starts new game
void newGame() {
    game *chessgame = initGame();
    setPieces(chessgame);
    int playerInput = 0;
    int *possibleMoves;
    
    //gameloop
    while (playerInput != -1) {

        //Player selects piece to move
        do {
            printBoard(chessgame->board, NULL);
            chessgame->turn == 0 ? printf("Console: UPR turn!\n") : printf("Console: lwr turn!\n");
            playerInput = getUserInput();
            possibleMoves = findMoves(chessgame, playerInput);
            if (possibleMoves == NULL) printf("Console: No valid moves found\n");

        } while (possibleMoves == NULL);
        int selPiece_tile = playerInput;


        //Player selects one of the valid moves to make
        int validMoveFlag = 0;
        do {
            //print tile and options
            printBoard(chessgame->board, possibleMoves);
            printf("Possible Moves: ");
            int i = 0;
            while (possibleMoves[i] != ENDOFLIST) {
                printf("%d ", possibleMoves[i]);
                i++;
            }
            printf("\n");

            //get and verify choice
            i = 0;
            playerInput = getUserInput();
            while (possibleMoves[i] != ENDOFLIST) {
                if (playerInput == possibleMoves[i]) {
                    movePiece(chessgame, selPiece_tile, playerInput);
                    validMoveFlag = 1;
                    break;
                }
                i++;
            }
            if (validMoveFlag == 0) printf("Console: Not a valid move\n");

        } while (validMoveFlag == 0);

        //end turn
        free(possibleMoves);
        if (checkGameState(chessgame) == 1) break; //king down
        chessgame->turn = !chessgame->turn;
    }
    
    free(chessgame);
    printf("end");
}




void menu() {

    int userInput = 0;
    do {

        printf("\n\n      _   \n");
        printf("     | | Simpleton\n");
        printf("  ___| |__   ___  ___ ___  \n");
        printf(" / __| '_ \\ / _ \\/ __/ __| \n");
        printf("| (__| | | |  __/\\__ \\__ \\ \n");
        printf(" \\___|_| |_|\\___||___/___/\n");
        printf("\n             By ALEC BAKER\n\n");

        printf("1 - Start Local Game\n");
        printf("2 - About\n");
        printf("3 - Exit\n");

        printf("Input: ");
        userInput = getMenuInput();
        if (userInput == 1) newGame();
        if (userInput == 2) printf("Hol on\n");
    } while (userInput != 3);
}


int main() {
    menu();
}
