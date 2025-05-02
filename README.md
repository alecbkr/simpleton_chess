# simpleton_chess
Small chess program created from the ground up in C that runs on the Windows command line. 

# about
A typical chess implementation on an 8x8 board and printed in ASCII characters to the command line. Only supports local 2 player games. Each piece is represented on the board by the first letter of its name (excluding **knight** which is represented by '**n**'). All pieces are capable of their usual moves, excluding complex maneuvers such as castling. No win condition is currently present.

This project was programmed soley by me, Alec Baker, and my knowledge of C. No tutorials, AI, or other external help was used in its making. 

# how to play
Player 1 is **"UPR"**, whose pieces are represented by uppercase letters and will always go first. Player 2 is **"lwr"**, whose pieces are represented by lowercase letters. To move a piece, use the grid coordinates seen on the left and below the board to locate the piece to move, then type the coordinate and press enter. When a valid piece is chosen, the board will show your available moves marked by a small 'o'. Type the coordinate of a valid move you wish to make and press enter to finalize your turn.

# features
- **Reactive board**: the board will change to show available moves of selected pieces
- **Valid moves only**: when a move is attempted, a check is performed to ensure the move is within that piece's rules

# in the future...
This program is not complete. Some very basic, but essential features of chess are not yet present. I plan to work on this in my free time and get it fully functional. Here is a list of what's missing, plus some things I want to implement (maybe).

- Win condition
- Threatened king notification (check, checkmate)
- Pawn promotions
- Special moves (castling, en passant)
- multiplatform support
- Saved games
- Game replays
- graphical interface
- Simple AI opponent
- LAN multiplayer
- Online multiplayer

# credits
- Entire program written by **Alec Baker**
- ASCII "chess" art provided by **patorjk.com**
