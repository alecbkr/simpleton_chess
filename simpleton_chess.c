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
typedef struct piece {
    char type;
    int color;
} piece;

typedef struct player {
    int color; //1 = caps(white), 0 = lowercase(black)
} player;

typedef struct board {
    piece pieceOnTile;
    int tileNum;
    struct board *up;
    struct board *down;
    struct board *left;
    struct board *right;
    struct board *upLeft;
    struct board *upRight;
    struct board *downRight;
    struct board *downLeft;
} board;

typedef struct game {
    board *chessBoard;
    player *p0;
    player *p1;
    int turn;
} game;

//=====PROTOTYPES===================================
void printBoard(board *, int *); //prototype

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
//prints board plainly or with possible moves if move list is provided
void printBoard(board *chessBoard, int *possibleMoves) {
    // board *board = currentGame->chessBoard;
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
        piece currPiece = chessBoard[i].pieceOnTile;

        //spacing of board
        if (i % 8 == 0 && i != 0) {
            printf("%c \n%d %c ", 186, c--, 186);
            checkerBuffer++;
        }

        //if move list is provided to print
        if (possibleMoves != NULL) {
            if (possibleMoves[j] == i) {
                chessBoard[i].pieceOnTile.type == 'e' ? printf("%c ", 248) : printf("%c ", 237);
                j++;
                continue;
            }
        }
        //Decides what to print | CAPS = color 1 | lower = color 0
        if (currPiece.type != 'e')
            currPiece.color == 1 ? printf("%c ", toupper(currPiece.type)) : printf("%c ", currPiece.type);
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
int checkTiles(int *currListAddy, board *newTile, int turn, int loopDir) {
    if (newTile == NULL) return 0;

    int i = 0;
    do {
        //Empty tile case
        if (newTile->pieceOnTile.type == 'e') {
            currListAddy[i++] = newTile->tileNum;
        }
        //Enemy tile case
        else if (newTile->pieceOnTile.color != turn) {
            currListAddy[i++] = newTile->tileNum;
            return i;
        }
        //Friendly || OOB case
        else {
            // currListAddy[i++] = 99;
            return i;
        }
        switch (loopDir) {
            case 1: newTile = newTile->up;        break;//UP
            case 2: newTile = newTile->down;      break;//DOWN
            case 3: newTile = newTile->left;      break;//LEFT
            case 4: newTile = newTile->right;     break;//RIGHT
            case 5: newTile = newTile->upLeft;    break;//UPLEFT
            case 6: newTile = newTile->upRight;   break;//UPRIGHT
            case 7: newTile = newTile->downLeft;  break;//DOWNLEFT
            case 8: newTile = newTile->downRight; break;//DOWNRIGHT
        }

    } while (newTile != NULL && loopDir != 0);
    // if (loopDir == 0) return i - 1;
    return i;
}

//returns every possible move from selected piece and prints board with the options
int *findMoves(game *activeGame, int tileIdx) {

    piece selectedPiece = activeGame->chessBoard[tileIdx].pieceOnTile;
    if (selectedPiece.type == 'e' || selectedPiece.color != activeGame->turn) return NULL;
    

    board *selTile = &activeGame->chessBoard[tileIdx]; //ptr to tile holding selected piece
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

            if (tileIdx / 8 > 1) {
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->up->upLeft, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->up->upRight, currTurn, 0);
            }

            if (tileIdx / 8 < 7) {
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->down->downLeft, currTurn, 0);
            numMoves += checkTiles(&possibleMoves[numMoves], selTile->down->downRight, currTurn, 0);
            }

            if (tileIdx % 8 > 1) {
                numMoves += checkTiles(&possibleMoves[numMoves], selTile->left->upLeft, currTurn, 0);
                numMoves += checkTiles(&possibleMoves[numMoves], selTile->left->downLeft, currTurn, 0);
            }
            
            if (tileIdx % 8 < 7) {
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
            
            if (currTurn == 1) {
                if (selTile->up != NULL && selTile->up->pieceOnTile.type == 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->up, currTurn, 0);

                if (selTile->upLeft != NULL && selTile->upLeft->pieceOnTile.type != 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->upLeft, currTurn, 0);

                if (selTile->upRight != NULL && selTile->upRight->pieceOnTile.type != 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->upRight, currTurn, 0);

                if (47 < tileIdx && tileIdx < 55 && selTile->up->up->pieceOnTile.type == 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->up->up, currTurn, 0);
            }

            else {
                if (selTile->down != NULL && selTile->down->pieceOnTile.type == 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->down, currTurn, 0);

                if (selTile->downLeft != NULL && selTile->downLeft->pieceOnTile.type != 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->downLeft, currTurn, 0);
    
                if (selTile->downRight != NULL && selTile->downRight->pieceOnTile.type != 'e')
                    numMoves += checkTiles(&possibleMoves[numMoves], selTile->upRight, currTurn, 0);
    
                if (7 < tileIdx && tileIdx < 16 && selTile->down->down->pieceOnTile.type == 'e')
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
void movePiece(game *active, int selPiece_tile, int newTile) {
    piece selPiece = active->chessBoard[selPiece_tile].pieceOnTile;
    active->chessBoard[selPiece_tile].pieceOnTile.type = 'e';
    selPiece_tile = newTile;
    active->chessBoard[newTile].pieceOnTile = selPiece;
}

//Creates 8x8 board and sets player turn
game *initGame() {
    game *newGame = malloc(sizeof(game));
    newGame->p0 = malloc(sizeof(player));
    newGame->p1 = malloc(sizeof(player));
    newGame->chessBoard = malloc(sizeof(board) * 64);

    board *tempBoard = newGame->chessBoard;

    //Links all board tiles together and sets each empty
    for (int i = 0; i < 64; i++) {
        tempBoard[i].pieceOnTile.type = 'e';
        tempBoard[i].tileNum = i;

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

    newGame->turn = 1;
    return newGame;
}

//Sets all pieces in typical chess postions
void setPieces(game *activeGame) {
    
    for (int i = 0; i < 2; i++) {
        //KING
        piece king = {
            .type = 'k', 
            .color = i,
        };
        activeGame->chessBoard[3 + (i * 56)].pieceOnTile = king;

        //QUEEN
        piece queen = {
            .type = 'q',
            .color = i,
        };
        activeGame->chessBoard[4 + (i * 56)].pieceOnTile = queen;

        //ROOKS
        piece rook = {
            .color = i,
            .type = 'r',
        };
        activeGame->chessBoard[0 + (i * 56)].pieceOnTile = rook;
        activeGame->chessBoard[7 + (i * 56)].pieceOnTile = rook;

        //BISHOPS
        piece bishop = {
            .color = i,
            .type = 'b'
        };
        activeGame->chessBoard[2 + (i * 56)].pieceOnTile = bishop;
        activeGame->chessBoard[5 + (i * 56)].pieceOnTile = bishop;

        //KNIGHTS
        piece knight = {
            .color = i,
            .type = 'n'
        };
        activeGame->chessBoard[1 + (i * 56)].pieceOnTile = knight;
        activeGame->chessBoard[6 + (i * 56)].pieceOnTile = knight;

        // PAWNS
        piece pawn = {
            .color = i,
            .type = 'p'
        };
        activeGame->chessBoard[8 + (i * 40)].pieceOnTile = pawn;
        activeGame->chessBoard[9 + (i * 40)].pieceOnTile = pawn;
        activeGame->chessBoard[10 + (i * 40)].pieceOnTile = pawn;
        activeGame->chessBoard[11 + (i * 40)].pieceOnTile = pawn;
        activeGame->chessBoard[12 + (i * 40)].pieceOnTile = pawn;
        activeGame->chessBoard[13 + (i * 40)].pieceOnTile = pawn;
        activeGame->chessBoard[14 + (i * 40)].pieceOnTile = pawn;
        activeGame->chessBoard[15 + (i * 40)].pieceOnTile = pawn;
    }
}

//Translates board coordinates into tile index
//Will loop until valid board coordinate is inputed
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
            printf("Command must be a valid board coordinate (e.g. e4, a1, etc)\n");
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
            printf("Command must be a valid board coordinate (e.g. e4, a1, etc)\n");

    } while (validFlag);

    //Calculate board index
    int result = (row * 8) + col;
    return result;
}

//======DEBUG====================================================
//Debug function: prints surrounding neighbors of given tile
void checkNeighbors(board *chessboard, int tileNum) {
    board *tile = &chessboard[tileNum];
    if (tile == NULL) return;
    
    tile->upLeft != NULL ? printf("%2d ", tile->upLeft->tileNum) : printf("N  ");
    tile->up != NULL ? printf("%2d ", tile->up->tileNum) : printf("N  ");
    tile->upRight != NULL ? printf("%2d \n", tile->upRight->tileNum) : printf("N  \n");
    tile->left != NULL ? printf("%2d ", tile->left->tileNum) : printf("N  ");
    printf("%2d ", tile->tileNum);
    tile->right != NULL ? printf("%2d \n", tile->right->tileNum) : printf("N  \n");
    tile->downLeft != NULL ? printf("%2d ", tile->downLeft->tileNum) : printf("N  ");
    tile->down != NULL ? printf("%2d ", tile->down->tileNum) : printf("N  ");
    tile->downRight != NULL ? printf("%2d \n", tile->downRight->tileNum) : printf("N  \n");
}

//place any piece anywhere
void placePiece(game *active, int tileNum, char type) {
    board *selTile = &active->chessBoard[tileNum];

    if (isupper(type) == 0) selTile->pieceOnTile.color = 0;
    else selTile->pieceOnTile.color = 1;

    switch (tolower(type)) {
        case 'k': selTile->pieceOnTile.type = 'k'; break;
        case 'q': selTile->pieceOnTile.type = 'q'; break;
        case 'r': selTile->pieceOnTile.type = 'r'; break;
        case 'n': selTile->pieceOnTile.type = 'n'; break;
        case 'b': selTile->pieceOnTile.type = 'b'; break;
        default: selTile->pieceOnTile.type = 'p'; break;
    }
}

//place custom pieces as specified to test scenarios
void testScenario() {
    game *active = initGame();
    int playerInput;
    int *possibleMoves;
    setPieces(active);
    int testLoc = 56;
    
    // placePiece(active, testLoc, 'Q');
    // placePiece(active, 54, 'P');
    // placePiece(active, testLoc, 'N');

    possibleMoves = findMoves(active, testLoc);
    printBoard(active->chessBoard, possibleMoves);
    
}


//=====GAME START================================================
//Starts new game
void newGame() {
    game *active = initGame();
    setPieces(active);
    int playerInput = 0;
    int *possibleMoves;
    
    //gameloop
    while (playerInput != -1) {

        //Player selects piece to move
        do {
            printBoard(active->chessBoard, NULL);
            active->turn == 1 ? printf("Console: UPR turn!\n") : printf("Console: lwr turn!\n");
            playerInput = getUserInput();
            possibleMoves = findMoves(active, playerInput);
            if (possibleMoves == NULL) printf("Console: No valid moves found\n");

        } while (possibleMoves == NULL);
        int selPiece_tile = playerInput;


        //Player selects one of the valid moves to make
        int validMoveFlag = 0;
        do {
            //print board and options
            printBoard(active->chessBoard, possibleMoves);
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
                    movePiece(active, selPiece_tile, playerInput);
                    validMoveFlag = 1;
                    break;
                }
                i++;
            }
            if (validMoveFlag == 0) printf("Console: Not a valid move\n");

        } while (validMoveFlag == 0);

        //end turn
        free(possibleMoves);
        active->turn = !active->turn;
    }
    
    free(active);
    printf("end");
}


void menu() {
    printf("\n\n      _   \n");
    printf("     | | Simpleton\n");
    printf("  ___| |__   ___  ___ ___  \n");
    printf(" / __| '_ \\ / _ \\/ __/ __| \n");
    printf("| (__| | | |  __/\\__ \\__ \\ \n");
    printf(" \\___|_| |_|\\___||___/___/\n");
    printf("\n             By ALEC BAKER\n\n");

    printf("1 - Start Game\n");
    printf("2 - About\n");
    printf("3 - Exit\n");

    int userInput = 0;
    do {
        printf("Input: ");
        scanf("%d", &userInput);
        if (userInput > 3 || 1 > userInput) userInput = 0;
        if (userInput == 1) newGame();
        if (userInput == 2) printf("Hol on\n");
    } while (userInput != 3);
    

}



int main() {
    menu();
}