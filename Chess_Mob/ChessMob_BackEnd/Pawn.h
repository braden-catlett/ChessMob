#include "Piece.h"

#pragma once

namespace ChessMob
{
	class Pawn : public Piece
	{
	private:
		// to indicate that the pawn started on the front line and has not yet moved
		bool IsDoubleMoveAvailable;

	public:
		// Constructor
		Pawn(bool isWhite, bool isDoubleMoveAvailable = false) : Piece(isWhite)
		{
			Type = PawnPiece;
			IsDoubleMoveAvailable = isDoubleMoveAvailable;
		}

		virtual ~Pawn() {}

		// generates a list of possible moves that this piece can make from the given position and stores it in PossibleMoves
		// only generates the list if IsPossibleMovesCurrent is false
		void GeneratePossibleMoves(const Coord & position, Piece * board[8][8], bool isInCheck, const std::vector<Coord> & blockCheckCoords)
		{
			PossibleMoves.clear();

			int forward = 1;
			Direction forwardLeft = BackwardLeft;
			Direction forwardRight = BackwardRight;

			if (IsWhite)
			{
				forward = -1;
				forwardLeft = ForwardLeft;
				forwardRight = ForwardRight;
			}

			Direction directionForCheck = FindDirectionForCheck(position, board);
			
			// see if the space in front of the pawn is available
			if (!board[position.Row + forward][position.Col] && (directionForCheck == OtherDirection ||
				directionForCheck == Forward || directionForCheck == Backward))
			{
				AddPossibleMovesAlongAxis(position, board, isInCheck, blockCheckCoords, forward, 0, 1);

				// no need to check that position.Row + 2 * forward is between 0 and 7 because they should not have a double move available
				if (IsDoubleMoveAvailable && !board[position.Row + 2 * forward][position.Col])
					AddPossibleMovesAlongAxis(position, board, isInCheck, blockCheckCoords, 2 * forward, 0, 1);
			}

			// see if the pawn can attack forward and to the left
			if (((IsWhite && position.Col > 0) || (!IsWhite && position.Col < 7)) &&
				board[position.Row + forward][position.Col + forward] &&
				(directionForCheck == OtherDirection || directionForCheck == ForwardLeft || directionForCheck == BackwardRight))
			{
				AddPossibleMovesAlongAxis(position, board, isInCheck, blockCheckCoords, forward, forward, 1);
			}

			// see if the pawn can attack forward and to the right
			if (((IsWhite && position.Col < 7) || (!IsWhite && position.Col > 0)) &&
				board[position.Row + forward][position.Col - forward] &&
				(directionForCheck == OtherDirection || directionForCheck == ForwardRight || directionForCheck == BackwardLeft))
			{
				AddPossibleMovesAlongAxis(position, board, isInCheck, blockCheckCoords, forward, -forward, 1);
			}
		}
	};
}