# Structs & Enums #
The structs.h file contains all of the small structs and enums that are used by the back end code. See below for details on what things are for.

---------------------------
## Enum PieceType ##
The PieceType enum makes transferring information about what pieces are where easier and less memory intensive for those cases when a full [Piece](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece) is not needed.

The possible values of the PieceType enum are, in order: None, King, Queen, Rook, Bishop, Knight, and Pawn.

---------------------------
## Enum PlayerType ##
The PlayerType enum distinguishes human players from computer players as all human player features are handled through the front end.

The possible values of the PlayerType enum are, in order: Human and Computer.

---------------------------
## Struct Coord ##
The Coord struct allows for a simpler method of transferring the positions of pieces and actions on the board.

#### Member Variables: ####

Name				 | Description
-------------------- | -------------
int Row		| The row being referenced.
int Col		| the column being referenced.

#### Member Functions: ####

Name				 | Description
-------------------- | -------------
Coord(int row, int col)	| Constructor. Sets Col and Row with the given values.

---------------------------
## Struct Move ##
The move struct is for tracking moves made in a game. This allows for the GameManager to contain a move history and perform "undo move" operations.

#### Member Variables: ####

Name				 | Description
-------------------- | -------------
Coord From			 | The starting coordinate of the piece that was moved.
Coord To			 | The coordinate of the space the piece was moved to.
PieceType Capture	 | The type of piece that was captured.

#### Member Functions: ####

Name				 | Description
-------------------- | -------------
Move(Coord from, Coord to, PieceType capture = PieceType::None)	| Constructor. Sets From, To, and Capture to the given values. Note that capture can be omitted to give Capture a default value of None.