#include "stdafx.h"
#include "CppUnitTest.h"
#include <algorithm>
#include <sstream>
#include "..\GameMaster.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ChessMob;

namespace BackEndTest
{		
	TEST_CLASS(TestGameMaster)
	{
	public:
		// for testing that GameMaster can be constructed correctly
		TEST_METHOD(TestConstructor)
		{
			GameMaster gm1(HumanPlayer, PawnPiece, HumanPlayer, PawnPiece);

			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					if ((row == 0 && col == 4) || (row == 7 && col == 3))
					{
						Assert::AreEqual((int) KingPiece, abs(gm1.GetPiece(Coord(row, col))));
					}
					else
					{
						Assert::AreEqual(row < 4, gm1.GetPiece(Coord(row, col)) == - PawnPiece);

						Assert::AreEqual(row >= 4, gm1.GetPiece(Coord(row, col)) == PawnPiece);
					}
				}
			}

			Piece * board[8][8] = {
				{ new Queen(false), NULL, new King(false), NULL, new Queen(false), NULL, NULL, NULL },
				{ NULL, NULL, new Queen(false), NULL, NULL, NULL, NULL, NULL },
				{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
				{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
				{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, new Rook(true) },
				{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
				{ NULL, NULL, NULL, NULL, NULL, NULL, new Rook(true), NULL },
				{ new Rook(true), NULL, NULL, NULL, NULL, NULL, new Rook(true), new King(true) }
			};

			// test the constructor to be used when loading a game
			GameMaster gm2(HumanPlayer, HumanPlayer, board, std::deque<Move>(), true);

			int pieceCount = 0;
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					if ((row == 0 && col == 2) || (row == 7 && col == 7))
					{
						Assert::AreEqual((int) KingPiece, abs(gm2.GetPiece(Coord(row, col))));
						pieceCount++;
					}
					else
					{
						if (gm2.GetPiece(Coord(row, col)))
						{
							Assert::AreEqual(row < 4, gm2.GetPiece(Coord(row, col)) == - QueenPiece);

							Assert::AreEqual(row >= 4, gm2.GetPiece(Coord(row, col)) == RookPiece);
							pieceCount++;
						}
					}
				}
			}
			Assert::AreEqual(9, pieceCount);
		}

		// for testing GameMaster.MakeMove
		TEST_METHOD(TestMakeMove)
		{
			GameMaster gm1(HumanPlayer, PawnPiece, HumanPlayer, PawnPiece);
			Assert::AreEqual(true, gm1.MakeMove(Move(Coord(4,3), Coord(3,4), NoPiece)));
			Assert::AreEqual(0, gm1.GetPiece(Coord(4,3)));
			Assert::AreEqual(6, gm1.GetPiece(Coord(3,4)));
			Assert::AreEqual(true, gm1.MakeMove(Move(Coord(2,3), Coord(3,4), NoPiece)));
			Assert::AreEqual(0, gm1.GetPiece(Coord(2,3)));
			Assert::AreEqual(-6, gm1.GetPiece(Coord(3,4)));
			Assert::AreEqual(2, (int) gm1.GetHistory().size());

			GameMaster gm2(HumanPlayer, QueenPiece, HumanPlayer, RookPiece);
			Assert::AreEqual(false, gm2.MakeMove(Move(Coord(4,3), Coord(3,4), NoPiece)));
			Assert::AreEqual(true, gm2.MakeMove(Move(Coord(5,3), Coord(1,3), RookPiece)));
			Assert::AreEqual(false, gm2.MakeMove(Move(Coord(1,4), Coord(1,3), NoPiece)));
			Assert::AreEqual(true, gm2.MakeMove(Move(Coord(1,2), Coord(1,3), QueenPiece)));
			Assert::AreEqual(-3, gm2.GetPiece(Coord(1,3)));

			// test a known failure in Android app caused by piece ghosting (has been fixed)
			Piece * board[8][8] = {NULL};

			board[0][1] = new Rook(false);
			board[0][4] = new King(false);
			board[0][5] = new Rook(false);
			board[1][2] = new Rook(true);
			board[1][3] = new Rook(true);
			board[1][4] = new Rook(false);
			board[1][5] = new Rook(true);
			board[6][4] = new Rook(false);
			board[7][2] = new Rook(true);
			board[7][3] = new King(true);
			board[7][4] = new Rook(true);
			board[7][7] = new Rook(true);

			GameMaster * gm3 = new GameMaster(HumanPlayer, HumanPlayer, board, std::deque<Move>(), true);

			Assert::AreEqual(true, gm3->MakeMove(Move(Coord(7, 4), Coord(6, 4), NoPiece)));
		}

		TEST_METHOD(TestComputerVsComputer)
		{
			GameMaster gm1(ComputerPlayer, QueenPiece, ComputerPlayer, RookPiece);

			gm1.MakeComputerMove();
			gm1.MakeComputerMove();
			gm1.MakeComputerMove();
			gm1.MakeComputerMove();
		}
	};
}