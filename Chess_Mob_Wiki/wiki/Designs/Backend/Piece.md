# Piece #
The piece class is an abstract class that provides an interface for working with the different Chess pieces.

------------------------------------------------
## Member Variables: ##

### PossibleMoves ###

__Access Level:__ private

__Type:__ vector<[Move](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-Move)>

__Comments:__

Used to store the moves available to the piece.

### IsWhite ###

__Access Level:__ private

__Type:__ bool

__Comments:__

Indicates which player the piece belongs to.

------------------------------------------------
## Member Functions: ##

### IsValidMove ###

__Access Level:__ public

__Return Type:__ bool

__Parameters:__

* [Move](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-Move) move
	* The move being validated
* const Piece* board[8][8]
	* The current state of the board

__Comments:__

This is function that returns whether or not the given move is valid. This should include whether or not making the move causes the player to put themselves in check. The check for check incrementally looks along the diagonals and straights of the board from the current location. Upon coming upon the current player's king, look in the opposite direction. Upon coming upon another current player's piece, move on to the next path. Upon coming upon an opponent's piece, check whether the piece can move in that direction. If the piece can, check the opposite direction for a king, otherwise move on to the next path.

### GeneratePossibleMoves ###

__Access Level:__ public

__Return Type:__ void

__Parameters:__

* [Coord](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums#markdown-header-Coord) position
	* The piece's current position
* const Piece* board[8][8]
	* The current state of the board

__Comments:__

This is a virtual function that finds all the moves that the piece can make. It stores the results in [PossibleMoves](#markdown-header-possiblemoves) which can be accessed internally or externally through [GetPossibleMoves](#markdown-header-GetPossibleMoves).