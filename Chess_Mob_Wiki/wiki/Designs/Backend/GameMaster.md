# GameMaster #
The gamemaster class is the overarching class that controls the game and game logic. It 
is incharge of setting up the game and updating the board. It also checks of players being in check and end of game.

------------------------------------------------
## Member Variables: ##

### White ###

__Access Level:__ private

__Type:__ [PlayerType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PlayerType)

__Comments:__

Keeps track of black's player type, either human or computer.

### Black ###

__Access Level:__ private

__Type:__ [PlayerType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PlayerType)

__Comments:__

Keeps track of white's player type, either human or computer.

### ComputerPlayer ###

__Access Level:__ private

__Type:__ [Computer](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Player/Computer)*

__Comments:__

Used when a player is a computer player. It is set to a null pointer
when there is no computer player.

### Board ###

__Access Level:__ private

__Type:__ [Piece](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece)* [8][8]

__Comments:__

Used to keep track of the pieces on the board. All empty spaces on the board
are set to a null pointer.

### History ###

__Access Level:__ private

__Type:__ stack<[Move](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-Move)>

__Comments:__

Keeps track of the moves in order for the player to undo moves.

### IsWhiteTurn ###

__Access Level:__ private

__Type:__ bool

__Comments:__

Indicates which player's turn it is. Is true of [White](#markdown-header-White)'s turn and false of [Black](#markdown-header-Black)'s turn.

------------------------------------------------
## Member Functions: ##

### GameMaster ###

__Access Level:__ public

__Parameters:__

* [PlayerType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PlayerType) WhitePlayerType
	* The choice of the white player's player type, either human player or computer player.
* [PieceType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PieceType) WhitePieceType
	* The white player's choice of piece type.
* [PlayerType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PlayerType) BlackPlayerType
	* The choice of the black player's player type, either human player or computer player.
* [PieceType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PieceType) BlackPieceType
	* The black player's choice of piece type.

	
__Comments:__
	
This is the GameMaster constructor. This function initializes a new game. It initializes the 
variables White and Black to the corresponding player types. It initializes [IsWhiteTurn](#markdown-header-IsWhiteTurn) to true. 
It calls [SetupBoard](#markdown-header-SetupBoard) for the actual set up of the board.
 
### GameMaster ###

__Access Level:__ public

__Parameters:__

* [PlayerType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PlayerType) WhitePlayerType
	* The white player's player type, either human player or computer player.
* [PlayerType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PlayerType) BlackPlayerType
	* The black player's player type, either human player or computer player.
* const [Piece](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece)* board[8][8]
	* Board setup for a loaded game.
* bool isWhiteTurn
	* variable stating whose turn it is.

__Comments:__

This is an overloaded constructor. This function is used to setup a saved game. 
It initializes the variables White and Black to the corresponding player types. 
It initializes IsWhiteTurn to the passed in value. It sets up the board with the
passed in board setup.

### ~GameMaster ###

__Access Level:__ public

__Parameters:__ None

__Comments:__

This is the GameMaster deconstrutor. This function cleans up and deletes the 
[ComputerPlayer](#markdown-header-ComputerPlayer) and [Board](#markdown-header-Board).

### SetupBoard ###

__Access Level:__ private

__Return Type:__ void

__Parameters:__

* [PieceType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PieceType) WhitePieceType
	* The white player's choice of piece type.
* [PieceType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PieceType) BlackPieceType
	* The black player's choice of piece type.


__Comments:__

This function is used to setup the board at the beginning of the game by using 
default piece setups determined by the passed in player types.

### UpdateBoard ###

__Access Level:__ private

__Return Type:__ void

__Parameters:__

* [Move](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-Move) move
	* The move to be made.

__Comments:__

This function updates the Board. It also adds the move to the [History](#markdown-header-History) stack.

### MakeMove ###

__Access Level:__ public

__Return Type:__ bool

__Parameters:__

* [Move](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-Move) move
	* The planned move by a player.

__Comments:__

This function is used for the human player to make a move. This function checks that there is a piece at the From location of the proposed move.
The function then checks that the piece belongs to the current player. Then the function
calls the piece's [IsValidMove](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece#markdown-header-IsValidMove) function to ensure the move is a valid move. If the move is 
valid, the function calls the [UpdateBoard](#markdown-header-UpdateBoard) function. The function then returns whether the
move was executed.

We are still deciding on how a computer player makes a move.  Possible overloading of this function.

### GetPiece ###

__Access Level:__ public

__Return Type:__ [PieceType](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-PieceType)

__Parameters:__

* [Coord](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-Coord) location
	* The coordinates on the Board

__Comments:__

This function is used for the front end to get the PieceType of at the passed location.
If the piece is a black piece, the PieceType is negated.

### UndoMove ###

__Access Level:__ public

__Return Type:__ void

__Parameters:__ None

__Comments:__

This function is used to undo the previous move. The function checks the number of human players,
if the number of human players is 2, the function undoes 1 move. Otherwise, the function
undoes 2 moves.

### IsInCheck ###

__Access Level:__ public

__Return Type:__ bool

__Parameters:__ None

__Comments:__

This function checks whether the opponent player is in check. The function loops through the board and finds the opponent's king location. It then loops through the board again and for each of the current player's pieces, it calls the piece IsValidMove on the king's location.

### IsInCheckmate ###

__Access Level:__ public

__Return Type:__ bool

__Parameters:__ None

__Comments:__

This function checks whether the opponent player is in checkmate. The function should only be called if [IsInCheck](#markdown-header-IsInCheck) is true. The exact algorithm is still being designed. The function has to check that the king has no valid moves and no move by the player's other pieces blocks the attacking piece.
