#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include "../Computer.h"
#include "../Piece.h"
#include "../King.h"
#include "../Queen.h"
#include "../Rook.h"
#include "../Bishop.h"
#include "../Knight.h"
#include "../Pawn.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ChessMob;

namespace BackEndTest
{	
	TEST_CLASS(TestComputer)
	{
	public:

		TEST_METHOD(TestBasicMakeMove)
		{
			Piece * board[8][8];

			for(int i = 0; i < 8; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					board[i][j] = NULL;
				}
			};

			Move lastMove;

			board[0][0] = new King(true);
			board[7][7] = new King(false);

			Computer aiplayer(3,false);

			Move aimove = aiplayer.MakeMove(board);

			//move is from king's location
			Assert::AreEqual((char)7, aimove.From.Row);
			Assert::AreEqual((char)7, aimove.From.Col);

			Assert::AreEqual(true, (aimove.To.Row == 6 && (aimove.To.Col == 7 || aimove.To.Col == 6)) || (aimove.To.Row == 7 && aimove.To.Col == 6));
		}

		TEST_METHOD(TestMoveTreeQueenLimit)
		{
			Piece * board[8][8] = {NULL};
			bool isWhite = true;

			board[7][3] = new King(true);
			board[0][4] = new King(false);

			Coord origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			int multiplier = isWhite ? -1 : 1;

			board[origin.Row][origin.Col + 3 * multiplier] = new Queen(isWhite);
			board[origin.Row][origin.Col + 5 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Queen(isWhite);
			board[origin.Row + 2 * multiplier][origin.Col + 4 * multiplier] = new Queen(isWhite);

			isWhite = false;

			origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			multiplier = isWhite ? -1 : 1;

			board[origin.Row][origin.Col + 3 * multiplier] = new Queen(isWhite);
			board[origin.Row][origin.Col + 5 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Queen(isWhite);
			board[origin.Row + 2 * multiplier][origin.Col + 4 * multiplier] = new Queen(isWhite);

			Move lastMove;

			Computer * aiPlayer = new Computer(3, true);

			Move aiMove = aiPlayer->MakeMove(board);

			Assert::AreEqual(aiMove.From.Col==aiMove.To.Col && aiMove.From.Row == aiMove.To.Row, false);

			delete aiPlayer;
		}

		TEST_METHOD(TestMoveTreeRookLimit)
		{
			Piece * Board[8][8] = {NULL};
			bool isWhite = true;

			Board[7][3] = new King(true);
			Board[0][4] = new King(false);

			Coord origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			int multiplier = isWhite ? -1 : 1;
			
			Board[origin.Row][origin.Col + 2 * multiplier] = new Rook(isWhite);
			Board[origin.Row][origin.Col + 3 * multiplier] = new Rook(isWhite);
			Board[origin.Row][origin.Col + 5 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 2 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Rook(isWhite);

			isWhite = false;

			origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			multiplier = isWhite ? -1 : 1;
			
			Board[origin.Row][origin.Col + 2 * multiplier] = new Rook(isWhite);
			Board[origin.Row][origin.Col + 3 * multiplier] = new Rook(isWhite);
			Board[origin.Row][origin.Col + 5 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 2 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Rook(isWhite);
			Board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Rook(isWhite);

			Move lastMove;

			Computer aiPlayer(3,true);

			Move aiMove = aiPlayer.MakeMove(Board);
		}

		TEST_METHOD(TestMoveTreeBishopLimit)
		{
			Piece * Board[8][8] = {NULL};
			bool isWhite = true;

			Board[7][3] = new King(true);
			Board[0][4] = new King(false);

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

			isWhite = false;

			origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			multiplier = isWhite ? -1 : 1;
			
			Board[origin.Row][origin.Col + 2 * multiplier] = new Bishop(isWhite);
			Board[origin.Row][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			Board[origin.Row][origin.Col + 5 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + multiplier][origin.Col + 2 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + 2 * multiplier][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			Board[origin.Row + 2 * multiplier][origin.Col + 4 * multiplier] = new Bishop(isWhite);

			Move lastMove;

			Computer *aiPlayer = new Computer(3,true);

			Move aiMove = aiPlayer->MakeMove(Board);

			delete aiPlayer;
		}

		TEST_METHOD(TestMoveTreeKnightLimit)
		{
			Piece * Board[8][8] = {NULL};
			bool isWhite = true;

			Board[7][3] = new King(true);
			Board[0][4] = new King(false);

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

			isWhite = false;

			origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			multiplier = isWhite ? -1 : 1;
			
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

			Move lastMove;

			Computer aiPlayer(2,true);

			Move aiMove = aiPlayer.MakeMove(Board);
		}

		TEST_METHOD(TestMoveTreePawnLimit)
		{
			Piece * Board[8][8] = {NULL};
			bool isWhite = true;

			Board[7][3] = new King(true);
			Board[0][4] = new King(false);

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

			isWhite = false;

			origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			multiplier = isWhite ? -1 : 1;
			
			for (int row = origin.Row; row != origin.Row + 4 * multiplier; row += multiplier)
			{
				for (int col = 0; col < 8; col++)
				{
					if (row != origin.Row || col != origin.Row + 4 * multiplier)
						Board[row][col] = new Pawn(isWhite, row == origin.Row + 3 * multiplier);
				}
			}

			Move lastMove;

			Computer aiPlayer(3,true);

			Move aiMove = aiPlayer.MakeMove(Board);
			int zero = 0;
		}

		TEST_METHOD(TestBishopWeakness)
		{
			Piece* board[8][8] = {NULL};
            Computer whitePlayer(2, true);

			board[0][3] = new King(false);
			board[0][5] = new Bishop(false);
			board[0][7] = new Bishop(false);
			board[2][1] = new Bishop(false);
			board[3][2] = new Bishop(false);
			board[4][4] = new Bishop(false);
			board[6][3] = new Bishop(false);
			board[2][4] = new Queen(true);
			board[7][3] = new King(true);

			Move computerMove = whitePlayer.MakeMove(board);

			Assert::AreEqual(true, computerMove.To.Row == 4 && computerMove.To.Col == 4);
		}

		TEST_METHOD(TestLegalMoves)
		{
			Piece* board[8][8] = {NULL};
			Computer whitePlayer(2, true);

			board[0][2] = new Bishop(false);
			board[0][3] = new Bishop(false);
			board[0][4] = new King(false);
			board[1][2] = new Bishop(false);
			board[1][6] = new Bishop(false);
			board[2][3] = new Bishop(false);
			board[2][4] = new Bishop(false);
			board[3][7] = new Bishop(false);
			board[4][7] = new Bishop(false);
			board[5][1] = new Bishop(false);
			
			board[6][3] = new Queen(true);
			board[6][4] = new Queen(true);
			board[7][2] = new Queen(true);
			board[7][3] = new King(true);

			whitePlayer.MakeMove(board);

			for(int row = 0; row < 8; row++)
			{
				for(int col = 0; col < 8; col++)
				{
					if(board[row][col])
					{
						delete board[row][col];
						board[row][col] = NULL;
					}
				}
			}

			bool isWhite = true;

			board[7][3] = new King(true);
			board[0][4] = new King(false);

			Coord origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			int multiplier = isWhite ? -1 : 1;

			board[origin.Row][origin.Col + 3 * multiplier] = new Queen(isWhite);
			board[origin.Row][origin.Col + 5 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Queen(isWhite);
			board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Queen(isWhite);
			board[origin.Row + 2 * multiplier][origin.Col + 4 * multiplier] = new Queen(isWhite);

			isWhite = false;

			origin = isWhite ? Coord(7, 7) : Coord(0, 0);
			multiplier = isWhite ? -1 : 1;
			
			board[origin.Row][origin.Col + 2 * multiplier] = new Bishop(isWhite);
			board[origin.Row][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			board[origin.Row][origin.Col + 5 * multiplier] = new Bishop(isWhite);
			board[origin.Row + multiplier][origin.Col + 2 * multiplier] = new Bishop(isWhite);
			board[origin.Row + multiplier][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			board[origin.Row + multiplier][origin.Col + 4 * multiplier] = new Bishop(isWhite);
			board[origin.Row + multiplier][origin.Col + 5 * multiplier] = new Bishop(isWhite);
			board[origin.Row + 2 * multiplier][origin.Col + 3 * multiplier] = new Bishop(isWhite);
			board[origin.Row + 2 * multiplier][origin.Col + 4 * multiplier] = new Bishop(isWhite);

			board[3][7] = board[1][5];
			board[1][5] = NULL;
			board[5][1] = board[6][2];
			board[6][2] = NULL;

			whitePlayer.MakeMove(board);
		}
	};
}