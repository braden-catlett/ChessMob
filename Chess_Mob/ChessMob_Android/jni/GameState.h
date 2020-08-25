#include "Piece.h"
#include "King.h"

#pragma once

namespace ChessMob
{
	// finds out if the given player is in check or not
	static bool IsInCheck(Piece * board[8][8], bool isWhiteTurn, std::vector<Coord> & blockingMoves)
	{
		Coord kingLocation = Coord(1337, 1337);
		
		// find the location of the player's king
		for (int row = 0; row < 8; row++)
		{
			for (int col = 0; col < 8; col++)
			{
				Piece * piece = board[row][col];
				if (piece && piece->IsWhitePiece() == isWhiteTurn && piece->GetType() == KingPiece)
				{
					kingLocation.Row = row;
					kingLocation.Col = col;

					//for breaking the outer for loop
					row = 8;
					break;
				}
			}
		}
		if(kingLocation.Row < 0 || kingLocation.Row > 7 ||kingLocation.Col < 0 || kingLocation.Col > 7)
			return true;

		King king(isWhiteTurn);

		bool isInCheck = king.IsInCheck(kingLocation, board, blockingMoves);
		
		return isInCheck;
	}

	// finds out if the given player can move or not
	static bool IsStaleMate(Piece * board[8][8], bool isWhiteTurn)
	{
		bool foundKing = false;
		bool foundMoves = false;
		for (int row = 0; row < 8; row++)
		{
			for (int col = 0; col < 8; col++)
			{
				Piece * piece = board[row][col];
				if (piece && piece->IsWhitePiece() == isWhiteTurn)
				{
					if(piece->GetType() == KingPiece)
						foundKing = true;
					if(piece->GetPossibleMoves().size() > 0)
						foundMoves = true;
				}
			}
		}
		return (!foundKing || !foundMoves);
	}

	static bool IsDraw(Piece * board[8][8])
	{
		char bishopColor = 'n';
		int bishopCount = 0;
		int knightCount = 0;
		for (int row = 0; row < 8; row++)
		{
			for(int col = 0; col < 8; col++)
			{
				Piece * piece = board[row][col];
				if (piece)
				{
					char color = 'n';
					switch(piece->GetType())
					{
					case KingPiece:
						// skip kings; they should always be there
						continue;
					case KnightPiece:
						// not a draw with more than one knight left
						knightCount++;
						if (knightCount > 1) { return false; }
						break;
					case BishopPiece:
						// not a draw if bishops are not all on the same color tiles
						bishopCount++;
						color = row % 2 == col % 2 ? 'b' : 'w';
						if (bishopColor == 'n')
						{
							bishopColor = color;
						}
						else if (bishopColor != color)
						{
							return false;
						}
						break;
					default:
						// not a draw if any other types of pieces are on the board
						return false;
					}
				}
			}
		}
		return true;
	}
}
