#pragma once

namespace ChessMob
{	
	//least common multiple = 50778
	//using lcm*4 (not sure why)

	const int QueenScore = 33852;
	const int RookScore = 29016;
	const int PairedBishopScore = 45136;
	const int UnpairedBishopScore = 11284;
	const int KnightScore = 15624;
	const int PawnScore = 6652;
	const int CheckScore = 101556;
	const int CheckmateScore = 20311200;
	const int CannotWinScore = 10155600;

	/*
	// c++ is not letting me use fractions for initializing these constants, so done in decimal form
	const float QueenScore = 0.16666666666; // 1/6
	const float RookScore = 0.14285714285;  // 1/7
	const float BishopScore = 0.11111111111;  // 1/9
	const float KnightScore = 0.07692307692;  // 1/13
	const float PawnScore = 0.03225806451;  // 1/31
	const float CheckScore = 0.5;
	const float CheckmateScore = 10.0;
	*/

	// For indicating a type of piece without having to pass a full Piece object
	enum PieceType {NoPiece, KingPiece, QueenPiece, RookPiece, BishopPiece, KnightPiece, PawnPiece};
	
	// For indicating a type of player
	enum PlayerType {HumanPlayer, ComputerPlayer};
	
	// For indicating a type of piece without having to pass a full Piece object
	enum Direction {OtherDirection, Forward, Backward, Left, Right, ForwardLeft, ForwardRight, BackwardLeft, BackwardRight, MultipleDirections};

	// For indicating a position on the board
	struct Coord
	{
		// The row position
		char Row;

		// The column position
		char Col;

		// Default constructor; sets Row and Col to 0
		Coord()
		{
			Row = 0;
			Col = 0;
		}

		// Constructor; sets Row and Col to the given values
		Coord(int row, int col)
		{
			Row = row;
			Col = col;
		}

		// override for equality operator
		bool operator == (const Coord &other) const
		{
			//if(!Row || !Col)
			//	return false;
			return Row == other.Row && Col == other.Col;
		}

		// override for not equal operator
		bool operator != (const Coord &other) const
		{
			return Row != other.Row || Col != other.Col;
		}
	};

	// For holding information about a move made
	struct Move
	{
		// The starting position of the move
		Coord From;

		// The ending position of the move
		Coord To;

		// The type of piece that was captured
		PieceType Capture;

		// Flag for if the move resulted in a converted piece
		bool ChangedType;

		Move()
		{
			From = Coord();
			To = Coord();
			Capture = NoPiece;
			ChangedType = false;
		}

		// Constructor; sets From, To, and Capture to the given values with a default value for Capture
		Move(Coord from, Coord to, PieceType capture = NoPiece, bool changedType = false)
		{
			From = from;
			To = to;
			Capture = capture;
			ChangedType = changedType;
		}

		// Copy Constructor
		Move(const Move &other)
		{
			From = other.From;
			To = other.To;
			Capture = other.Capture;
			ChangedType = other.ChangedType;
		}

		// override for assignment operator
		void operator = (const Move &other)
		{
			From = other.From;
			To = other.To;
			Capture = other.Capture;
			ChangedType = other.ChangedType;
		}

		// override for equality operator
		bool operator == (const Move &other) const
		{
			return From == other.From && To == other.To;
		}

		// override for not equal operator
		bool operator != (const Move &other) const
		{
			return From != other.From || To != other.To;
		}
	};
}
