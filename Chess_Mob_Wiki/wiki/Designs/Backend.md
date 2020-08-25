# Backend Design Documents #
The backend consists of a [GameMaster](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/GameMaster) class that will track the state of the game and be the main conduit for communications between this module and the UI modules.

The GameMaster will contain two [Player](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Player) objects that can be of either the [Human](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Player/Human) or [Computer](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Player/Computer) sub-classes. These classes will handle the appropriate actions for making and accepting moves with the Human class working with the UI for input while the Computer class calculates and returns a move on its own.

The GameMaster will also contain a 2D array of [Piece](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece) pointers. Piece is an abstract class with a sub-class for each type of Chess piece: [Pawn](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece/Pawn), [Knight](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece/Knight), [Bishop](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece/Bishop), [Rook](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece/Rook), [Queen](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece/Queen), and [King](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/Piece/King). The GameMaster will receive moves from the UI and the Computer player as needed, verify the move, add the move to a "history" stack, apply the move to the board, and then see if the move causes check or checkmate.

The back end also used several [structs and enums](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend/StructsAndEnums) to make passing information between objects easier.