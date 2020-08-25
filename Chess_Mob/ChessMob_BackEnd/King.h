#include "Piece.h"

#pragma once

namespace ChessMob
{
	class King : public Piece
	{
	public:
		// Constructor
		King(bool isWhite) : Piece(isWhite)
		{
			Type = KingPiece;
		}
		
		virtual ~King() {}

		// for seeing if a knight can attack the king at the given Coord
		Coord CanBeAttackedByKnight(Coord coord, Piece * board[8][8])
		{
			int row, col;

			// going forward
			row = coord.Row - 2;
			if (row >= 0 && row < 8)
			{
				// forward and left
				col = coord.Col - 1;
				if (col >= 0 && col < 8 && board[row][col] && board[row][col]->GetType() == KnightPiece
					&& board[row][col]->IsWhitePiece() != IsWhite)
				{
					return Coord(row, col);
				}
						
				// forward and right
				col = coord.Col + 1;
				if (col >= 0 && col < 8 && board[row][col] && board[row][col]->GetType() == KnightPiece
					&& board[row][col]->IsWhitePiece() != IsWhite)
				{
					return Coord(row, col);
				}
			}

			// going backward
			row = coord.Row + 2;
			if (row >= 0 && row < 8)
			{
				// backward and left
				col = coord.Col - 1;
				if (col >= 0 && col < 8 && board[row][col] && board[row][col]->GetType() == KnightPiece
					&& board[row][col]->IsWhitePiece() != IsWhite)
				{
					return Coord(row, col);
				}
						
				// backward and right
				col = coord.Col + 1;
				if (col < 8 && col >= 0 && board[row][col] && board[row][col]->GetType() == KnightPiece
					&& board[row][col]->IsWhitePiece() != IsWhite)
				{
					return Coord(row, col);
				}
			}

			// going left
			col = coord.Col - 2;
			if (col >= 0 && col < 8)
			{
				// left and forward
				row = coord.Row - 1;
				if (row >= 0 && row < 8 && board[row][col] && board[row][col]->GetType() == KnightPiece
					&& board[row][col]->IsWhitePiece() != IsWhite)
				{
					return Coord(row, col);
				}
						
				// left and backward
				row = coord.Row + 1;
				if (row >= 0 && row < 8 && board[row][col] && board[row][col]->GetType() == KnightPiece
					&& board[row][col]->IsWhitePiece() != IsWhite)
				{
					return Coord(row, col);
				}
			}

			// going right
			col = coord.Col + 2;
			if (col < 8 && col >= 0)
			{
				// right and forward
				row = coord.Row - 1;
				if (row >= 0 && row < 8 && board[row][col] && board[row][col]->GetType() == KnightPiece
					&& board[row][col]->IsWhitePiece() != IsWhite)
				{
					return Coord(row, col);
				}
						
				// right and backward
				row = coord.Row + 1;
				if (row < 8 && row >= 0 && board[row][col] && board[row][col]->GetType() == KnightPiece
					&& board[row][col]->IsWhitePiece() != IsWhite)
				{
					return Coord(row, col);
				}
			}
			
			return Coord(1337, 1337);
		}

		// for seeing if a pawn can attack the king at the given Coord
		Coord CanBeAttackedByPawn(Coord coord, Piece * board[8][8])
		{
			int forward = IsWhite ? -1 : 1;

			int row = coord.Row + forward;
			int colLeft = coord.Col + forward;
			int colRight = coord.Col - forward;

			if (row >= 0 && row < 8)
			{
				if (colLeft >= 0 && colLeft < 8)
				{
					Piece * piece = board[row][colLeft];
					if (piece && piece->IsWhitePiece() != IsWhite && piece->GetType() == PawnPiece)
						return Coord(row, colLeft);
				}
				
				if (colRight < 8 && colRight >= 0)
				{
					Piece * piece = board[row][colRight];
					if (piece && piece->IsWhitePiece() != IsWhite && piece->GetType() == PawnPiece)
						return Coord(row, colRight);
				}
			}

			return Coord(1337, 1337);
		}

		// for seeing if a pawn can attack the king at the given Coord
		Coord CanBeAttackedByKing(Coord coord, Piece * board[8][8])
		{
			for (int row = coord.Row - 1; row <= coord.Row + 1 && row < 8; row++)
			{
				if (row >= 0)
				{
					for (int col = coord.Col - 1; col <= coord.Col + 1 && col < 8; col++)
					{
						if (col >= 0)
						{
							Piece * piece = board[row][col];
							if (piece && piece->IsWhitePiece() != IsWhite && piece->GetType() == KingPiece)
								return Coord(row, col);
						}
					}
				}
			}

			return Coord(1337, 1337);
		}

		// generates a list of possible moves that this piece can make from the given position and stores it in PossibleMoves
		// only generates the list if IsPossibleMovesCurrent is false
		void GeneratePossibleMoves(const Coord & position, Piece * board[8][8], bool isInCheck, const std::vector<Coord> & blockCheckCoords)
		{
			PossibleMoves.clear();

			// go around the king, checking each adjacent position on the board to see if it is a possible move
			for (int row = position.Row - 1; row <= position.Row + 1 && row < 8; row++)
			{
				if (row >= 0)
				{
					for (int col = position.Col - 1; col <= position.Col + 1 && col < 8; col++)
					{
						if (col >= 0)
						{
							// a position adjacent to the king that needs to be looked at as a possible move
							Coord to(row, col);
							// whether or not the 'to' position is already occupied by a friendly piece
							bool isFriendly = IsFriendlyPiece(to, board);
							// find out if an enemy pawn can attack the 'to' position
							Coord pawnLocation = CanBeAttackedByPawn(to, board);
							bool canBeAttackedByPawn = pawnLocation.Row >= 0 && pawnLocation.Row < 8;
							// find out if an enemy knight can attack the 'to' position
							Coord knightLocation = CanBeAttackedByKnight(to, board);
							bool canBeAttackedByKnight = knightLocation.Row >= 0 && knightLocation.Row < 8;
							// find out if an enemy king can attack the 'to' position
							Coord kingLocation = CanBeAttackedByKing(to, board);
							bool canBeAttackedByKing = kingLocation.Row >= 0 && kingLocation.Row < 8;
							// find out if any other enemy pieces can attack the 'to' position
							Direction directionForCheck = FindDirectionForCheck(to, board, true);

							/*
								if the 'to' position is not occupied by a friedly piece ('!isFriendly'),
								not attackable by any enemy pieces ('directionForCheck == OtherDirection &&
								!canBeAttackedByPawn && !canBeAttackedByKnight' && !canBeAttackedByKing),
								and is not where the king already is ('!(col == position.Col && row == position.Row)'),
								add it to PossibleMoves
							*/
							if (!isFriendly && directionForCheck == OtherDirection &&
								!canBeAttackedByPawn && !canBeAttackedByKnight && !canBeAttackedByKing &&
								!(col == position.Col && row == position.Row))
							{
								Move move = Move(position, to, board[row][col] ? board[row][col]->GetType() : NoPiece);
								PossibleMoves.push_back(move);
							}
						}
					}
				}
			}
		}

		// can be used by any th
		bool IsInInternalCheck(Coord position, Piece * board[8][8], std::vector<Coord> & blockingMoves, Direction directionForCheck, bool canBeAttackedByKnight = false, bool canBeAttackedByPawn = false)
		{
			// if the king is not being attacked from multiple directions, add all coords along the given direction to blockingMoves
			if (directionForCheck != MultipleDirections && !canBeAttackedByKnight && !canBeAttackedByPawn)
			{
				int rowMod, colMod;
				switch (directionForCheck)
				{
				case Forward:
					rowMod = -1;
					colMod = 0;
					break;
				case Backward:
					rowMod = 1;
					colMod = 0;
					break;
				case Left:
					rowMod = 0;
					colMod = -1;
					break;
				case Right:
					rowMod = 0;
					colMod = 1;
					break;
				case ForwardLeft:
					rowMod = -1;
					colMod = -1;
					break;
				case ForwardRight:
					rowMod = -1;
					colMod = 1;
					break;
				case BackwardLeft:
					rowMod = 1;
					colMod = -1;
					break;
				case BackwardRight:
					rowMod = 1;
					colMod = 1;
					break;
				}
		
				int row = position.Row + rowMod;
				int col = position.Col + colMod;
				for (; row >= 0 && row < 8 && col >= 0 && col < 8; row += rowMod, col += colMod)
				{
					if (board[row][col] && board[row][col]->IsWhitePiece() == IsWhite)
						return false;
					Coord c(row, col);
					blockingMoves.push_back(c);
					if (board[row][col])
						break;
				}
			}
			return true;
		}

		// for regular check, checks every possible, not limited by pieces as possible
		bool IsInCheck(Coord position, Piece * board[8][8], std::vector<Coord> & blockingMoves)
		{
			blockingMoves.clear();

			Direction directionForCheck = FindDirectionForCheck(position, board, true, true);
			Coord pawnLocation = CanBeAttackedByPawn(position, board);
			Coord knightLocation = CanBeAttackedByKnight(position, board);
			bool canBeAttackedByPawn = pawnLocation.Row >= 0 && pawnLocation.Row < 8;
			bool canBeAttackedByKnight = knightLocation.Row >= 0 && knightLocation.Row < 8;

			if(directionForCheck != OtherDirection)
			{
				return IsInInternalCheck(position, board, blockingMoves, directionForCheck, canBeAttackedByKnight, canBeAttackedByPawn);
			}
			
			// if not being attacked along a major direction, but is being attacked by a knight
			if (canBeAttackedByKnight)
			{
				blockingMoves.push_back(knightLocation);
				return true;
			}

			// if not being attacked along a major direction, but is being attacked by a pawn
			if (canBeAttackedByPawn)
			{
				blockingMoves.push_back(pawnLocation);
				return true;
			}
			return false;
		}

		// if opponent's piece type is queen, this tells if in check
		bool IsInQueenCheck(Coord position, Piece * board[8][8], std::vector<Coord> & blockingMoves)
		{
			blockingMoves.clear();

			Direction directionForCheck = FindDirectionForCheck(position, board, true);

			if(directionForCheck != OtherDirection)
			{
				return IsInInternalCheck(position, board, blockingMoves, directionForCheck);
			}

			return false;
		}

		// if opponent's piece type is rook, this tells if in check
		bool IsInRookCheck(Coord position, Piece * board[8][8], std::vector<Coord> & blockingMoves)
		{
			blockingMoves.clear();

			// looks along horizontals and verticals
			Direction direction = LookAlongAxis(position, board, Left, true);
			if (direction != OtherDirection)
				return IsInInternalCheck(position, board, blockingMoves, direction);

			direction = LookAlongAxis(position, board, Forward, true);
			if (direction != OtherDirection)
				return IsInInternalCheck(position, board, blockingMoves, direction);	

			return false;
		}

		// if opponent's piece type is bishop, this tells if in check
		bool IsInBishopCheck(Coord position, Piece * board[8][8], std::vector<Coord> & blockingMoves)
		{
			blockingMoves.clear();

			// looks along diagonals.
			Direction direction = LookAlongAxis(position, board, ForwardLeft, true);
			if (direction != OtherDirection)
				return IsInInternalCheck(position, board, blockingMoves, direction);

			direction = LookAlongAxis(position, board, ForwardRight, true);
			if (direction != OtherDirection)
				return IsInInternalCheck(position, board, blockingMoves, direction);			

			return false;
		}

		// if opponent's piece type is knight, this tells if in check
		bool IsInKnightCheck(Coord position, Piece * board[8][8], std::vector<Coord> & blockingMoves)
		{
			blockingMoves.clear();

			Coord knightLocation = CanBeAttackedByKnight(position, board);
			bool canBeAttackedByKnight = knightLocation.Row >= 0 && knightLocation.Row < 8;

			// if not being attacked along a major direction, but is being attacked by a knight
			if (canBeAttackedByKnight)
			{
				blockingMoves.push_back(knightLocation);
				return true;
			}

			return false;
		}
	};
}