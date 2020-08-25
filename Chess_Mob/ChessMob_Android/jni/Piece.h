#pragma once
#include <vector>
#include "Structs.h"

namespace ChessMob
{
	class Piece
	{
	protected:
		// for easily determining the type of this piece
		PieceType Type;

		// holds a list of moves that the piece can make
		std::vector<Move> PossibleMoves;

		// indicates the color of the piece
		bool IsWhite;
		
		// looks for possibility of check along the given direction and its reverse
		// will return OtherDirection if forward is not Forward, Left, ForwardLeft, or ForwardRight
		Direction LookAlongAxis(const Coord & position, Piece * board[8][8], Direction forward, bool isKing)
		{
			Piece * pieceToFront = 0;

			int colMod = 0;
			int rowMod = 0;
			Direction backward = OtherDirection;
			PieceType otherPossibleAttacker;

			switch (forward)
			{
			case Forward:
				backward = Backward;
				otherPossibleAttacker = RookPiece;
				colMod = 0;
				rowMod = -1;
				break;
			case Left:
				backward = Right;
				otherPossibleAttacker = RookPiece;
				colMod = -1;
				rowMod = 0;
				break;
			case ForwardLeft:
				backward = BackwardRight;
				otherPossibleAttacker = BishopPiece;
				colMod = -1;
				rowMod = -1;
				break;
			case ForwardRight:
				backward = BackwardLeft;
				otherPossibleAttacker = BishopPiece;
				colMod = 1;
				rowMod = -1;
				break;
			default:
				return OtherDirection;
			}

			// look to the front
			int row = position.Row + rowMod;
			int col = position.Col + colMod;
			for (; row >= 0 && row < 8 && col >= 0 && col < 8; row += rowMod, col += colMod)
			{
				Piece * piece = board[row][col];
				if (piece)
				{
					if (piece->IsWhitePiece() == IsWhite)
					{
						if (piece->GetType() == KingPiece)
						{
							// it cannot be the king piece if I am moving the king piece
							if(isKing)
							{
								continue;
							}
							pieceToFront = piece;
						}
						// CHECK - what is the piece to front set as a king, if the current piece is the king
						else if (isKing)
						{
							pieceToFront = board[row][col];
						}
						break;
					}
					else if (piece->GetType() == QueenPiece || piece->GetType() == otherPossibleAttacker)
					{
						if (isKing)
						{
							return forward;
						}
						else
						{
							pieceToFront = piece;
						}
						break;
					}
					// CHECK - if I found a piece, any further pieces should not matter, add a break
					break;
				}
			}

			// look to the back, if necessary
			if (pieceToFront || isKing)
			{
				int row = position.Row - rowMod;
				int col = position.Col - colMod;
				for (; row >= 0 && row < 8 && col >= 0 && col < 8; row -= rowMod, col -= colMod)
				{
					Piece * piece = board[row][col];
					if (piece)
					{
						if (piece->IsWhitePiece() == IsWhite)
						{
							if (piece->GetType() == KingPiece)
							{
								// CHECK - how can the piece behind be king, if this piece is king?
								if (isKing)
								{
									continue;
								}
								return forward;
							}
						}
						else if ((isKing || pieceToFront->GetType() == KingPiece) &&
							(piece->GetType() == QueenPiece || piece->GetType() == otherPossibleAttacker))
						{
							return backward;
						}
						// CHECK - I found a piece, I should be done: add a break
						break;
					}
				}
			}

			return OtherDirection;
		}

		// finds the direction from which an attacker can cause check if the piece at position is moved
		// set isKing to true when the king is the piece that is (supposedly) at the given position
		Direction FindDirectionForCheck(const Coord & position, Piece * board[8][8], bool isKing = false, bool checkMultiDirection = false)
		{
			if (checkMultiDirection)
			{
				Direction directions[4];
				directions[0] = LookAlongAxis(position, board, Left, isKing);
				directions[1] = LookAlongAxis(position, board, ForwardLeft, isKing);
				directions[2] = LookAlongAxis(position, board, Forward, isKing);
				directions[3] = LookAlongAxis(position, board, ForwardRight, isKing);

				int directionCount = 0;
				Direction onlyDirection = OtherDirection;
				for (int axisNum = 0; axisNum < 4; axisNum++)
				{
					if (directions[axisNum] != OtherDirection)
					{
						onlyDirection = directions[axisNum];
						directionCount++;
					}
				}
				
				return directionCount > 1 ? MultipleDirections : onlyDirection;
			}
			else
			{
				Direction direction = LookAlongAxis(position, board, Left, isKing);
				if (direction != OtherDirection)
					return direction;
			
				direction = LookAlongAxis(position, board, ForwardLeft, isKing);
				if (direction != OtherDirection)
					return direction;
			
				direction = LookAlongAxis(position, board, Forward, isKing);
				if (direction != OtherDirection)
					return direction;
			
				direction = LookAlongAxis(position, board, ForwardRight, isKing);

				return direction;
			}
		}

		// check if the given position on the given board is occupied by a friendly piece
		bool IsFriendlyPiece(const Coord & coord, Piece * board[8][8])
		{
			return board[coord.Row][coord.Col] && board[coord.Row][coord.Col]->IsWhitePiece() == IsWhite;
		}

		// goes out from the given coordinates using the row and column modifiers given adding to PossibleMoves
		void AddPossibleMovesAlongAxis(const Coord & position, Piece * board[8][8], bool isInCheck,
			const std::vector<Coord> & blockCheckCoords, int rowMod, int colMod, int maxSteps = 8)
		{
			if(rowMod == 1337)
				rowMod = -1;
			if(colMod == 1337)
				colMod = -1;
			int row = position.Row + rowMod;
			int col = position.Col + colMod;
			for (int step = 0; row >= 0 && col >= 0 && row < 8 && col < 8 && step < maxSteps;
				row += rowMod, col += colMod, step++)
			{
				Coord coord = Coord(row, col);
				bool isFriendlyPiece = IsFriendlyPiece(coord, board);
				bool foundBlockCoord = false;
				for(int i = 0; i < (int)blockCheckCoords.size(); i++)
				{
					if(blockCheckCoords[i] == coord)
					{
						foundBlockCoord = true;
						break;
					}
				}
				if (isFriendlyPiece)
					break;
				if(!isInCheck || (board[row][col] && board[row][col]->Type == KingPiece && board[row][col]->IsWhitePiece() == IsWhite) || foundBlockCoord)
				{
					Piece * piece = board[row][col];
					if (!piece)
					{
						Move move = Move(position, coord, NoPiece);
						PossibleMoves.push_back(move);
					}
					else
					{
						Move move = Move(position, coord, piece->GetType());
						PossibleMoves.push_back(move);
						break;
					}
				}
			}
		}

	public:

		// Constructor; sets IsWhite to the given value and initializes IsPossibleMovesCurrent to false and PossibleMoves to an empty vector
		Piece(bool isWhite)
		{
			IsWhite = isWhite;
			PossibleMoves = std::vector<Move>();
		}

		Piece(bool isWhite, std::vector<Move> moves)
		{
			IsWhite = isWhite;
			PossibleMoves = moves;
		}
		
		virtual ~Piece() {}

		// generates the moves that this piece can make from the given position on the given board
		virtual void GeneratePossibleMoves(const Coord & position, Piece * board[8][8], bool isInCheck,
			const std::vector<Coord> & blockCheckCoords) = 0;
		
		// checks if the given move can be made by seeing if it is in PossibleMoves
		bool IsValidMove(Move move)
		{
			for(int i = 0; i < (int)PossibleMoves.size(); i++)
			{
				if(PossibleMoves[i] == move)
					return true;
			}
			return false;
			//return std::find(PossibleMoves.begin(), PossibleMoves.end(), move) != PossibleMoves.end();
		}

		// returns whether or not the piece is white
		bool IsWhitePiece()
		{
			return IsWhite;
		}

		// returns the PossibleMoves vector
		std::vector<Move> GetPossibleMoves()
		{
			return PossibleMoves;
		}

		// returns the PieceType of this piece
		virtual const PieceType GetType()
		{
			return Type;
		}
	};
}
