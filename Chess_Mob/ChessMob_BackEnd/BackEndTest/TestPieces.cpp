#include "stdafx.h"
#include "CppUnitTest.h"
#include <algorithm>
#include <sstream>
#include "..\Piece.h"
#include "..\Bishop.h"
#include "..\King.h"
#include "..\Knight.h"
#include "..\Pawn.h"
#include "..\Queen.h"
#include "..\Rook.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ChessMob;

namespace Microsoft
{ 
    namespace VisualStudio
    { 
        namespace CppUnitTestFramework
        {
			template<> static std::wstring ToString<std::vector<Move>::iterator>(const std::vector<Move>::iterator& t)
			{
				std::wstringstream ss;
				ss << "(" << t->From.Row << ", " << t->From.Col << ") (" << t->To.Row << ", " << t->To.Col << ")";
				return std::wstring(ss.str());
			}
        }
    }
}

namespace BackEndTest
{		
	TEST_CLASS(TestPieces)
	{
	public:
		// for testing Pawn.IsValidMove() when the pawn is white
		TEST_METHOD(TestPawnIsValidMoveWhite)
		{
			Piece * board[8][8] = {NULL};

			board[5][6] = new King(true);
			board[0][1] = new Queen(true);
			board[2][3] = new Pawn(true);

			// should be able to move forward one space (all pieces are white)
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			std::vector<Move> generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());
			Assert::AreEqual(board[2][3]->IsValidMove(Move(Coord(2, 3), Coord(1, 3), NoPiece)), true);
			
			// should not be able to move at all (Queen to front left)
			delete board[0][1];
			board[0][1] = new Queen(false);
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());
			
			// should be able to capture pawn to the front left. also should be able to move forward (black pawn blocks black queen)
			board[1][2] = new Pawn(false);
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(2, (int)generatedMoves.size());
			Assert::AreEqual(board[2][3]->IsValidMove(Move(Coord(2, 3), Coord(1, 2), PawnPiece)), true);
			Assert::AreEqual(board[2][3]->IsValidMove(Move(Coord(2, 3), Coord(1, 3), NoPiece)), true);

			// should not be able to move (attacked by rook to side)
			delete board[5][6];
			board[5][6] = 0;
			board[2][6] = new King(true);
			board[2][0] = new Rook(false);
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());

			// should not be able to move (all places are blocked by friendly pieces)
			delete board[2][6];
			board[2][6] = NULL;
			delete board[2][0];
			board[2][0] = NULL;
			delete board[0][1];
			board[0][1] = NULL;
			delete board[3][4];
			board[3][4] = NULL;
			board[1][2] = new Knight(true);
			board[1][3] = new King(true);
			board[1][4] = new Pawn(true);
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());

			// edge cases, literary :: no end edge, as in the future that should be covered by piece replacement
			// left edge
			delete board[2][3];
			board[2][3] = NULL;
			board[3][0] = new Pawn(true);
			board[3][0]->GeneratePossibleMoves(Coord(3, 0), board, false, std::vector<Coord>());
			generatedMoves = board[3][0]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());

			// right edge
			delete board[3][0];
			board[3][0] = NULL;
			board[3][7] = new Pawn(true);
			board[3][7]->GeneratePossibleMoves(Coord(3, 7), board, false, std::vector<Coord>());
			generatedMoves = board[3][7]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());
		}
		
		// for testing Pawn.IsValidMove() when the pawn is black
		TEST_METHOD(TestPawnIsValidMoveBlack)
		{
			Piece * board[8][8] = {NULL};

			board[5][6] = new King(false);
			board[0][1] = new Queen(false);
			board[2][3] = new Pawn(false);

			// should be able to move forward one space (all pieces are black)
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			std::vector<Move> generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());
			Assert::AreEqual(board[2][3]->IsValidMove(Move(Coord(2, 3), Coord(3, 3), NoPiece)), true);
			
			// should not be able to move at all (queen would be able to attack king)
			delete board[0][1];
			board[0][1] = new Queen(true);
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());
			
			// should be able to capture pawn to the right and move forward as opponent pawn blocks queen.
			board[3][4] = new Pawn(true);
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(2, (int)generatedMoves.size());
			Assert::AreEqual(board[2][3]->IsValidMove(Move(Coord(2, 3), Coord(3, 4), PawnPiece)), true);
			Assert::AreEqual(board[2][3]->IsValidMove(Move(Coord(2, 3), Coord(3, 3), NoPiece)), true);

			// should not be able to move (attaked by rook to side)
			delete board[5][6];
			board[5][6] = 0;
			board[2][6] = new King(false);
			board[2][0] = new Rook(true);
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());

			// should not be able to move (all places are blocked by friendly pieces)
			delete board[2][6];
			board[2][6] = NULL;
			delete board[2][0];
			board[2][0] = NULL;
			delete board[0][1];
			board[0][1] = NULL;
			delete board[3][4];
			board[3][4] = NULL;
			board[3][2] = new Knight(false);
			board[3][3] = new King(false);
			board[3][4] = new Pawn(false);
			board[2][3]->GeneratePossibleMoves(Coord(2, 3), board, false, std::vector<Coord>());
			generatedMoves = board[2][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());

			// edge cases, literary :: no end edge, as in the future that should be covered by piece replacement
			// left edge
			delete board[2][3];
			board[2][3] = NULL;
			board[3][0] = new Pawn(false);
			board[3][0]->GeneratePossibleMoves(Coord(3, 0), board, false, std::vector<Coord>());
			generatedMoves = board[3][0]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());

			// right edge
			delete board[3][0];
			board[3][0] = NULL;
			board[3][7] = new Pawn(false);
			board[3][7]->GeneratePossibleMoves(Coord(3, 7), board, false, std::vector<Coord>());
			generatedMoves = board[3][7]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());
		}
		
		// for testing King.IsValidMove()
		TEST_METHOD(TestKingIsValidMove)
		{
			Piece * board[8][8] = {NULL};
			
			board[5][6] = new King(true);

			// can move any direction (no other pieces on board)
			board[5][6]->GeneratePossibleMoves(Coord(5, 6), board, false, std::vector<Coord>());
			std::vector<Move> generatedMoves = board[5][6]->GetPossibleMoves();
			Assert::AreEqual(8, (int)generatedMoves.size());

			// cannot move
			board[7][5] = new Rook(false);
			board[0][7] = new Queen(false);
			board[2][4] = new Bishop(false);
			board[7][4] = new Knight(false);
			board[5][6]->GeneratePossibleMoves(Coord(5, 6), board, false, std::vector<Coord>());
			generatedMoves = board[5][6]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());

			// can move to one spot
			delete board[7][4];
			board[7][4] = NULL;
			board[5][6]->GeneratePossibleMoves(Coord(5, 6), board, false, std::vector<Coord>());
			generatedMoves = board[5][6]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());
			Assert::AreEqual(board[5][6]->IsValidMove(Move(Coord(5, 6), Coord(6, 6), NoPiece)), true);

			// reset the board
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					delete board[row][col];
					board[row][col] = NULL;
				}
			}

			// king in corner
			board[7][7] = new King(true);
			board[7][7]->GeneratePossibleMoves(Coord(7, 7), board, false, std::vector<Coord>());
			generatedMoves = board[7][7]->GetPossibleMoves();
			Assert::AreEqual(3, (int)generatedMoves.size());

			// king in corner with opponent pawn
			board[5][5] = new Pawn(false);
			board[7][7]->GeneratePossibleMoves(Coord(7, 7), board, false, std::vector<Coord>());
			generatedMoves = board[7][7]->GetPossibleMoves();
			Assert::AreEqual(2, (int)generatedMoves.size());

			// king trapped by white pieces
			delete board[5][5];
			board[5][5] = NULL;
			board[6][6] = new Pawn(true);
			board[7][6] = new Rook(true);
			board[6][7] = new Queen(true);
			board[7][7]->GeneratePossibleMoves(Coord(7, 7), board, false, std::vector<Coord>());
			generatedMoves = board[7][7]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());
		}

		TEST_METHOD(TestQueenIsValidMove)
		{
			Piece * board[8][8] = {NULL};

			board[3][3] = new Queen(true);

			// should move in all directions with 27 positions, only piece on the board and in ideal location
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			std::vector<Move> generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(27, (int)generatedMoves.size());

			// should move to 24 positions, same team piece in the way
			// checks cannot attack same team or jump over them
			board[3][2] = new King(true);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(24, (int)generatedMoves.size());

			// should only move one position.
			// test not allowing self checking, and no jumping opponent pieces
			board[3][4] = new Rook(false);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());
			Assert::AreEqual(board[3][3]->IsValidMove(Move(Coord(3, 3), Coord(3, 4), RookPiece)), true);

			// put white queen in corner with nothing else on board; should have 21 possible moves
			delete board[3][2];
			board[3][2] = NULL;
			delete board[3][3];
			board[3][3] = NULL;
			delete board[3][4];
			board[3][4] = NULL;
			board[7][7] = new Queen(true);
			board[7][7]->GeneratePossibleMoves(Coord(7, 7), board, false, std::vector<Coord>());
			Assert::AreEqual(21, (int) board[7][7]->GetPossibleMoves().size());
			Assert::AreEqual(true, board[7][7]->IsValidMove(Move(Coord(7, 7), Coord(0, 0))));

			// add a few black pawns
			board[3][3] = new Pawn(false);
			board[7][6] = new Pawn(false);
			board[5][7] = new Pawn(false);
			board[7][7]->GeneratePossibleMoves(Coord(7, 7), board, false, std::vector<Coord>());
			Assert::AreEqual(7, (int) board[7][7]->GetPossibleMoves().size());
			Assert::AreEqual(true, board[7][7]->IsValidMove(Move(Coord(7, 7), Coord(3, 3))));
		}

		TEST_METHOD(TestRookIsValidMove)
		{
			Piece * board[8][8] = {NULL};

			board[3][3] = new Rook(true);

			// should move both horizontal and vertical. 14 open positions
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			std::vector<Move> generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(14, (int)generatedMoves.size());

			// should move in three directions, 11 positions
			// checks does not attack self and does not jump self
			board[3][2] = new King(true);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(11, (int)generatedMoves.size());

			// should move only one location, location of opponent queen
			// checks does not jump opponents or put self in check
			board[3][4] = new Queen(false);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());
			Assert::AreEqual(board[3][3]->IsValidMove(Move(Coord(3, 3), Coord(3, 4), RookPiece)), true);

			// should not be able to move, it blocks Bishops attack
			delete board[3][4];
			board[3][4] = NULL;
			delete board[3][2];
			board[3][2] = NULL;
			board[4][2] = new King(true);
			board[2][4] = new Bishop(false);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());
		}

		TEST_METHOD(TestBishopIsValidMove)
		{
			Piece * board[8][8] = {NULL};

			board[3][3] = new Bishop(true);

			// should be able move in any diagonal, 13 positions
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			std::vector<Move> generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(13, (int)generatedMoves.size());

			// should be able to move in three directions, 10 positions
			// checks for not attacking same pieces and not jumping same pieces
			board[2][2] = new King(true);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(10, (int)generatedMoves.size());

			// should only be able to move to 1 location, where opponent is
			// checks does not put itself in check and no jumping opponent pieces
			board[4][4] = new Queen(false);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(1, (int)generatedMoves.size());
			Assert::AreEqual(board[3][3]->IsValidMove(Move(Coord(3, 3), Coord(4, 4), QueenPiece)), true);

			// should not be able move, it blocks Rook attack
			delete board[4][4];
			board[4][4] = NULL;
			delete board[2][2];
			board[2][2] = NULL;
			board[3][2] = new King(true);
			board[3][5] = new Rook(false);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());
		}

		TEST_METHOD(TestKnightIsValidMove)
		{
			Piece * board[8][8] = {NULL};

			board[3][3] = new Knight(true);

			// should have eight possible moves
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			std::vector<Move> generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(8, (int)generatedMoves.size());

			// should be able to move to all eight spaces, testing jump on white and black
			board[2][2] = new Knight(true);
			board[2][3] = new Knight(true);
			board[2][4] = new Knight(true);
			board[3][4] = new Knight(true);
			board[4][4] = new Bishop(false);
			board[4][3] = new Bishop(false);
			board[4][2] = new Bishop(false);
			board[3][2] = new Bishop(false);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(8, (int)generatedMoves.size());

			// should be able to move to 4 spaces
			// tests not attacking same pieces, but opponent pieces

			// white pieces
			board[5][2] = board[2][2];
			board[2][2] = NULL;
			board[4][1] = board[2][3];
			board[2][3] = NULL;
			board[2][1] = board[2][4];
			board[2][4] = NULL;
			board[1][2] = board[3][4];
			board[3][4] = NULL;

			// black pieces
			board[1][4] = board[4][4];
			board[4][4] = NULL;
			board[2][5] = board[4][3];
			board[4][3] = NULL;
			board[4][5] = board[4][2];
			board[4][2] = NULL;
			board[5][4] = board[3][2];
			board[3][2] = NULL;

			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(4, (int)generatedMoves.size());
			Assert::AreEqual(board[3][3]->IsValidMove(Move(Coord(3, 3), Coord(1, 4), BishopPiece)), true);
			Assert::AreEqual(board[3][3]->IsValidMove(Move(Coord(3, 3), Coord(2, 5), BishopPiece)), true);
			Assert::AreEqual(board[3][3]->IsValidMove(Move(Coord(3, 3), Coord(4, 5), BishopPiece)), true);
			Assert::AreEqual(board[3][3]->IsValidMove(Move(Coord(3, 3), Coord(5, 4), BishopPiece)), true);

			// should not be able to move, testing not self check
			delete board[5][2];
			board[5][2] = NULL;
			delete board[4][1];
			board[4][1] = NULL;
			delete board[2][1];
			board[2][1] = NULL;
			delete board[1][2];
			board[2][1] = NULL;
			delete board[1][4];
			board[1][4] = NULL;
			delete board[2][5];
			board[2][5] = NULL;
			delete board[4][5];
			board[4][5] = NULL;
			delete board[5][4];
			board[5][4] = NULL;

			board[3][1] = new King(true);
			board[3][6] = new Rook(false);
			board[3][3]->GeneratePossibleMoves(Coord(3, 3), board, false, std::vector<Coord>());
			generatedMoves = board[3][3]->GetPossibleMoves();
			Assert::AreEqual(0, (int)generatedMoves.size());
		}
	};
}