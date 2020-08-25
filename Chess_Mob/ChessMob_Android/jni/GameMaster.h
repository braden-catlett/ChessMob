#include <deque>
#include "Computer.h"
#include "Piece.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "GameState.h"

#pragma once

namespace ChessMob
{
	class GameMaster
	{
        public:
		// the board; note that all empty positions should be set to NULL
        Piece * Board[8][8];
        
        private:
		// the move history
		std::deque<Move> History;

		// the type of player playing white
		PlayerType White;

		// the type of player playing black
		PlayerType Black;

		// the computer player; is only non-NULL if either White or Black is set to ComputerPlayer
		Computer * ComputerPlayer;

		// whether or not it is white's turn
		bool IsWhiteTurn;

		// whether or not the current player is in check
		bool IsPlayerInCheck;

		// whether or not the current player has been checkmated
		bool IsPlayerInCheckMate;

		// whether or not the game has reached a stalemate
		bool IsGameStaleMate;

		// for tracking where pieces can move to block a(ll) piece(s) that has the king in check
		std::vector<Coord> BlockCheckCoords;

		// places queens on one side of the board (determined by isWhite)
		void PlaceQueens(bool isWhite)
		{
			Coord origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			int multiplier = isWhite ? -1 : 1;

			Board[origin.Row][origin.Col + 3 * multiplier] = new Queen(isWhite);
			Board[origin.Row][origin.Col + 5 * multiplier] = new Queen(isWhite);
			Board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Queen(isWhite);
			Board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Queen(isWhite);
			Board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Queen(isWhite);
			Board[origin.Row + 2 * multiplier][origin.Col + 4 * multiplier] = new Queen(isWhite);
		}
		
		// places rooks on one side of the board (determined by isWhite)
		void PlaceRooks(bool isWhite)
		{
			Coord origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			int multiplier = isWhite ? -1 : 1;
			
			Board[origin.Row][origin.Col + 2 * multiplier] = new Rook(isWhite);
			Board[origin.Row][origin.Col + 3 * multiplier] = new Rook(isWhite);
			Board[origin.Row][origin.Col + 5 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 2 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Rook(isWhite);
		}
		
		// places bishops on one side of the board (determined by isWhite)
		void PlaceBishops(bool isWhite)
		{
			Coord origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			int multiplier = isWhite ? -1 : 1;
			
			Board[origin.Row][origin.Col + 2 * multiplier] = new Bishop(isWhite);
			Board[origin.Row][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			Board[origin.Row][origin.Col + 5 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + multiplier][origin.Col + 2 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + 2 * multiplier][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + 2 * multiplier][origin.Col + 4 * multiplier] = new Bishop(isWhite);
		}
		
		// places knights on one side of the board (determined by isWhite)
		void PlaceKnights(bool isWhite)
		{
			Coord origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			int multiplier = isWhite ? -1 : 1;
			
			Board[origin.Row][origin.Col + 1 * multiplier] = new Knight(isWhite);
			Board[origin.Row][origin.Col + 2 * multiplier] = new Knight(isWhite);
			Board[origin.Row][origin.Col + 3 * multiplier] = new Knight(isWhite);
			Board[origin.Row][origin.Col + 5 * multiplier] = new Knight(isWhite);
			Board[origin.Row][origin.Col + 6 * multiplier] = new Knight(isWhite);
			Board[origin.Row + multiplier][origin.Col + 1 * multiplier] = new Knight(isWhite);
			Board[origin.Row + multiplier][origin.Col + 2 * multiplier] = new Knight(isWhite);
			Board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Knight(isWhite);
			Board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Knight(isWhite);
			Board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Knight(isWhite);
			Board[origin.Row + multiplier][origin.Col + 6 * multiplier] = new Knight(isWhite);
			Board[origin.Row + 2 * multiplier][origin.Col + 3 * multiplier] = new Knight(isWhite);
			Board[origin.Row + 2 * multiplier][origin.Col + 4 * multiplier] = new Knight(isWhite);
		}
		
		// places pawns on one side of the board (determined by isWhite)
		void PlacePawns(bool isWhite)
		{
			Coord origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			int multiplier = isWhite ? -1 : 1;

			for (int row = origin.Row; row != origin.Row + 4 * multiplier; row += multiplier)
			{
				for (int col = 0; col < 8; col++)
				{
					if (row != origin.Row || col != origin.Row + 4 * multiplier)
						Board[row][col] = new Pawn(isWhite, row == origin.Row + 3 * multiplier);
				}
			}
		}

		// calls the appropriate PlaceXxxxx() function for each of the given piece types
		void SetupBoard(PieceType whitePieceType, PieceType blackPieceType)
		{
			switch (whitePieceType)
			{
			case QueenPiece:
				PlaceQueens(true);
				break;
			case RookPiece:
				PlaceRooks(true);
				break;
			case BishopPiece:
				PlaceBishops(true);
				break;
			case KnightPiece:
				PlaceKnights(true);
				break;
			case PawnPiece:
				PlacePawns(true);
				break;
			}
			
			switch (blackPieceType)
			{
			case QueenPiece:
				PlaceQueens(false);
				break;
			case RookPiece:
				PlaceRooks(false);
				break;
			case BishopPiece:
				PlaceBishops(false);
				break;
			case KnightPiece:
				PlaceKnights(false);
				break;
			case PawnPiece:
				PlacePawns(false);
				break;
			}
		}

		// updates the board with the given move and adds the move to History
		void UpdateBoard(Move move)
		{
			Piece * piece = Board[move.From.Row][move.From.Col];
			Board[move.From.Row][move.From.Col] = NULL;

			if (Board[move.To.Row][move.To.Col])
			{
				move.Capture = Board[move.To.Row][move.To.Col]->GetType();
				delete Board[move.To.Row][move.To.Col];
			}

			Board[move.To.Row][move.To.Col] = piece;
			
			History.push_back(move);
		}

		void GeneratePieceMoves(bool isInCheck = false, const std::vector<Coord> blockCheckCoords = std::vector<Coord>())
		{
			// update the PossibleMoves vector for each of the pieces of the next player
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					if (Board[row][col] && Board[row][col]->IsWhitePiece() == IsWhiteTurn)
					{
						Board[row][col]->GeneratePossibleMoves(Coord(row, col), Board, isInCheck, blockCheckCoords);
					}
				}
			}
		}

		void UpdateGameState()
		{
			// find out if the next player is in check
			IsPlayerInCheck = ChessMob::IsInCheck(Board, IsWhiteTurn, BlockCheckCoords);

			// update the PossibleMoves vector for each of the pieces of the next player
			GeneratePieceMoves(IsPlayerInCheck, BlockCheckCoords);

			// find out if the player has any available moves
			IsGameStaleMate = ChessMob::IsStaleMate(Board, IsWhiteTurn) || ChessMob::IsDraw(Board);

			// find out if the player has lost
			IsPlayerInCheckMate = IsPlayerInCheck && IsGameStaleMate;
		}

	public:
		// Constructor; starts a new game
		GameMaster(PlayerType whitePlayerType, PieceType whitePieceType, PlayerType blackPlayerType, PieceType blackPieceType,
			int computerDifficulty = 1)
		{
			BlockCheckCoords = std::vector<Coord>();
			IsWhiteTurn = true;
			History = std::deque<Move>();

			// set up the players
			White = whitePlayerType;
			Black = blackPlayerType;
			ComputerPlayer = whitePlayerType == ChessMob::ComputerPlayer || blackPlayerType == ChessMob::ComputerPlayer ?
				new Computer(computerDifficulty, whitePlayerType == ChessMob::ComputerPlayer ? true : false) : NULL;
			
			// set up the Board
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					Board[row][col] = NULL;
				}
			}
			Board[7][3] = new King(true);
			Board[0][4] = new King(false);
			SetupBoard(whitePieceType, blackPieceType);
			GeneratePieceMoves();
			UpdateGameState();
		}

		// Constructor; resumes a game
		GameMaster(PlayerType whitePlayerType, PlayerType blackPlayerType, Piece * board[8][8], std::deque<Move> history, bool isWhiteTurn,
			int computerDifficulty = 1)
		{
			BlockCheckCoords = std::vector<Coord>();
			IsWhiteTurn = isWhiteTurn;
			History = history;

			// set up the players
			White = whitePlayerType;
			Black = blackPlayerType;
			ComputerPlayer = whitePlayerType == ChessMob::ComputerPlayer || blackPlayerType == ChessMob::ComputerPlayer ?
				new Computer(computerDifficulty, whitePlayerType == ChessMob::ComputerPlayer ? true : false) : NULL;
			
			// set up the Board
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					Board[row][col] = board[row][col] ? board[row][col] : NULL;
				}
			}

			UpdateGameState();
		}

		// release the memory held by GameMaster
		~GameMaster()
		{
			// delete the board
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					if (Board[row][col])
						delete Board[row][col];
				}
			}

			// delete the history
			History.~deque();

			// delete the computer player if necessary
			if (ComputerPlayer)
				delete ComputerPlayer;
		}

		// converts a pawn that has made it to the other side of the given board into the given piece type
		bool ConvertPawn(Coord position, PieceType replacementType)
		{
			Piece * pawn = Board[position.Row][position.Col];
			if (pawn && pawn->GetType() == PawnPiece)
			{
				bool isWhite = pawn->IsWhitePiece();
				if ((isWhite && position.Row == 0) || (!isWhite && position.Row == 7))
				{
					Piece * replacement;
					switch (replacementType)
					{
					case QueenPiece:
						replacement = new Queen(isWhite);
						break;
					case RookPiece:
						replacement = new Rook(isWhite);
						break;
					case BishopPiece:
						replacement = new Bishop(isWhite);
						break;
					case KnightPiece:
						replacement = new Knight(isWhite);
						break;
					default:
						return false;
					}

					Board[position.Row][position.Col] = replacement;
					delete pawn;
					History.back().ChangedType = true;
					// Make sure that the correct game state is calculated if the human player just converted a pawn
					// Not necessary for computer player since it calls UpdateGameState after it converts a pawn
					if ((isWhite && White == HumanPlayer) || (!isWhite && Black == HumanPlayer))
						UpdateGameState();
					return true;
				}
			}

			return false;
		}

		// checks if the given move is valid and executes it if so
		bool MakeMove(Move move)
		{
			Piece * piece = Board[move.From.Row][move.From.Col];
			
			// check that the piece exists, is of the correct color and can actually make the given move
			if (!piece || piece->IsWhitePiece() != IsWhiteTurn || !piece->IsValidMove(move))
				return false;

			// update the board with the move
			UpdateBoard(move);

			// change to the other player's turn
			IsWhiteTurn = !IsWhiteTurn;

			UpdateGameState();

			return true;
		}

		// makes the computer make a move
		void MakeComputerMove()
		{
			// get and make the computer player's move
			Move lastMove = History.size() > 0 ? History.back() : Move();
			Move computerMove;
			do
			{
				computerMove = ComputerPlayer->MakeMove(Board);
			} while(!Board[computerMove.From.Row][computerMove.From.Col] && Board[computerMove.From.Row][computerMove.From.Col]->IsWhitePiece() != ComputerPlayer->IsCWhite());
			UpdateBoard(computerMove);

			// check if the computer player moved a pawn to the opposite side of the board and convert it if so
			int endRow = IsWhiteTurn ? 0 : 7;
			if (computerMove.To.Row == endRow && Board[computerMove.To.Row][computerMove.To.Col]->GetType() == PawnPiece)
				ConvertPawn(computerMove.To, ComputerPlayer->ConvertPawn());

			// update who's turn it is
			IsWhiteTurn = !IsWhiteTurn;

			UpdateGameState();
		}

		// undoes the previous moves until a human player's turn is reached
		void UndoMove()
		{
			unsigned int numMovesToUndo = ComputerPlayer ? 2 : 1;

			if (History.size() < numMovesToUndo)
				return;

			for (unsigned int moveNum = 0; moveNum < numMovesToUndo; moveNum++)
			{
				// move the moved piece back
				Move move = History.back();
				Piece * piece = Board[move.To.Row][move.To.Col];
				if(move.ChangedType)
				{
					Board[move.From.Row][move.From.Col] = new Pawn(piece->IsWhitePiece());
				}
				else
				{
					Board[move.From.Row][move.From.Col] = piece;
				}
				// restore the captured piece if necessary
				switch(move.Capture)
				{
				case QueenPiece:
					Board[move.To.Row][move.To.Col] = new Queen(IsWhiteTurn);
					break;
				case RookPiece:
					Board[move.To.Row][move.To.Col] = new Rook(IsWhiteTurn);
					break;
				case BishopPiece:
					Board[move.To.Row][move.To.Col] = new Bishop(IsWhiteTurn);
					break;
				case KnightPiece:
					Board[move.To.Row][move.To.Col] = new Knight(IsWhiteTurn);
					break;
				case PawnPiece:
					Board[move.To.Row][move.To.Col] = new Pawn(IsWhiteTurn);
					break;
				default:
					Board[move.To.Row][move.To.Col] = NULL;
					break;
				}

				// toggle whose turn it is
				IsWhiteTurn = !IsWhiteTurn;

				UpdateGameState();

				// remove the move from the history
				History.pop_back();
			}
		}

		// returns the type of the piece that is at the location
		int GetPiece(Coord location)
		{
			if(Board[location.Row][location.Col] == NULL)
				return 0;
			return Board[location.Row][location.Col]->IsWhitePiece() ? Board[location.Row][location.Col]->GetType() :
				-Board[location.Row][location.Col]->GetType();
		}

		// returns the current move history deque
		const std::deque<Move> GetHistory()
		{
			return History;
		}

		// returns if the current player is doing a poor job protecting their king and let themselves be put in check
		bool IsInCheck()
		{
			return IsPlayerInCheck;
		}
		
		// returns if the current player has been put in checkmate and therefore lost like the loser they are because they did not go with the totally OP pawns
		bool IsInCheckMate()
		{
			return IsPlayerInCheckMate;
		}

		// returns if the current player somehow managed to get into a situation where they can not move; how embarassing, how embarassing
		bool IsStaleMate()
		{
			return IsGameStaleMate;
		}
	};
}