#include "stdafx.h"
#include "CppUnitTest.h"
#include <deque>
#include "..\GameMaster.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ChessMob;

namespace BackEndTest
{
	TEST_CLASS(TestGameState)
	{
	public:

		TEST_METHOD(TestGameStatesQueenVQueen)
		{
			GameMaster gm1(HumanPlayer, QueenPiece, HumanPlayer, QueenPiece);

			Assert::AreEqual(true, gm1.MakeMove(Move(Coord(5, 3), Coord(1, 3), QueenPiece)));

			Assert::AreEqual(true, gm1.IsInCheck());
			Assert::AreEqual(false, gm1.IsStaleMate());
			Assert::AreEqual(false, gm1.IsInCheckMate());

			GameMaster gm2(HumanPlayer, QueenPiece, HumanPlayer, QueenPiece);

			Assert::AreEqual(true, gm2.MakeMove(Move(Coord(5, 3), Coord(5, 4), NoPiece)));

			Assert::AreEqual(false, gm2.IsInCheck());
			Assert::AreEqual(false, gm2.IsStaleMate());
			Assert::AreEqual(false, gm2.IsInCheckMate());

			Assert::AreEqual(true, gm2.MakeMove(Move(Coord(1, 3), Coord(6, 3), NoPiece)));

			Assert::AreEqual(true, gm2.IsInCheck());
			Assert::AreEqual(false, gm2.IsStaleMate());
			Assert::AreEqual(false, gm2.IsInCheckMate());

			Assert::AreEqual(true, gm2.MakeMove(Move(Coord(5, 4), Coord(6, 3), NoPiece)));

			Assert::AreEqual(false, gm2.IsInCheck());
			Assert::AreEqual(false, gm2.IsStaleMate());
			Assert::AreEqual(false, gm2.IsInCheckMate());
		}

		TEST_METHOD(TestGameStatesQueenVQueenComputer)
		{
			GameMaster gm1(HumanPlayer, QueenPiece, ComputerPlayer, QueenPiece);

			Assert::AreEqual(true, gm1.MakeMove(Move(Coord(5, 3), Coord(5, 4), NoPiece)));

			Assert::AreEqual(false, gm1.IsInCheck());
			Assert::AreEqual(false, gm1.IsStaleMate());
			Assert::AreEqual(false, gm1.IsInCheckMate());

			gm1.MakeComputerMove();

			std::deque<Move> history = gm1.GetHistory();

			Move computerMove = *(--history.end());

			if (computerMove.To == Coord(6, 3))
			{
				Assert::AreEqual(true, gm1.IsInCheck());
			}
			else
			{
				Assert::AreEqual(false, gm1.IsInCheck());
			}
			Assert::AreEqual(false, gm1.IsStaleMate());
			Assert::AreEqual(false, gm1.IsInCheckMate());

			if (computerMove.To == Coord(6, 3))
			{
				Assert::AreEqual(true, gm1.MakeMove(Move(Coord(5, 4), Coord(6, 3), NoPiece)));

				Assert::AreEqual(false, gm1.IsInCheck());
				Assert::AreEqual(false, gm1.IsStaleMate());
				Assert::AreEqual(false, gm1.IsInCheckMate());

				gm1.MakeComputerMove();

				history = gm1.GetHistory();

				computerMove = *(--history.end());

				if (computerMove.To == Coord(6, 3) || computerMove.To == Coord(6, 4))
				{
					Assert::AreEqual(true, gm1.IsInCheck());
				}
				else
				{
					Assert::AreEqual(false, gm1.IsInCheck());
				}
				Assert::AreEqual(false, gm1.IsStaleMate());
				Assert::AreEqual(false, gm1.IsInCheckMate());
			}
		}

		TEST_METHOD(TestGameKingMoveBishop)
		{
			Piece* board[8][8] = {NULL};
			bool isInCheck = false;
			std::vector<Coord> blockCheckCoords;
			std::vector<Move> possibleMoves;
			
			board[0][3] = new Bishop(true);
			board[0][5] = new Bishop(true);
			board[2][1] = new King(false);
			board[3][3] = new Bishop(true);
			board[3][7] = new Bishop(true);
			board[4][7] = new Bishop(true);
			board[5][3] = new Bishop(true);
			board[6][3] = new King(true);

			isInCheck = ChessMob::IsInCheck(board, false, blockCheckCoords);

			board[2][1]->GeneratePossibleMoves(Coord(2,1), board, isInCheck, blockCheckCoords);
			possibleMoves = board[2][1]->GetPossibleMoves();

			Assert::AreEqual(1, (int)possibleMoves.size());

			Assert::AreEqual(true, possibleMoves[0].To.Row == 1 && possibleMoves[0].To.Col == 0);
		}
		
		TEST_METHOD(TestGameStateCustomSetups)
		{
			Piece * board[8][8] = {NULL};

			board[7][4] = new King(true);
			board[0][0] = new King(false);
			board[4][5] = new Queen(false);
			board[5][5] = new Queen(false);

			GameMaster gm(HumanPlayer, HumanPlayer, board, std::deque<Move>(), false);
			Assert::AreEqual(false, gm.IsStaleMate());
			Assert::AreEqual(false, gm.IsInCheck());
			Assert::AreEqual(false, gm.IsInCheckMate());

			gm.MakeMove(Move(Coord(4, 5), Coord(5, 4)));
			Assert::AreEqual(true, gm.IsStaleMate());
			Assert::AreEqual(true, gm.IsInCheck());
			Assert::AreEqual(true, gm.IsInCheckMate());
		}

		TEST_METHOD(TestGameStateIsDraw)
		{
			Piece * board[8][8] = {NULL};

			board[3][3] = new King(true);
			board[4][1] = new King(false);
			board[7][6] = new Knight(true);

			// king vs king & knight
			Assert::AreEqual(true, ChessMob::IsDraw(board));

			// king & pawn vs king & knight
			board[4][2] = new Pawn(false);
			Assert::AreEqual(false, ChessMob::IsDraw(board));

			// king & queen vs king & knight
			delete board[4][2];
			board[4][2] = new Queen(false);
			Assert::AreEqual(false, ChessMob::IsDraw(board));

			// king & knight vs king & knight
			delete board[4][2];
			board[4][2] = new Knight(true);
			Assert::AreEqual(false, ChessMob::IsDraw(board));

			// king & bishop (on black tile) vs king & bishop (on white tile)
			delete board[4][2];
			delete board[7][6];
			board[4][2] = new Bishop(true);
			board[7][6] = new Bishop(false);
			Assert::AreEqual(false, ChessMob::IsDraw(board));

			// king vs king & bishop
			delete board[7][6];
			board[7][6] = NULL;
			Assert::AreEqual(true, ChessMob::IsDraw(board));

			// king vs king & multiple bishops (on black tiles only)
			board[6][2] = new Bishop(true);
			board[3][3] = new Bishop(true);
			Assert::AreEqual(true, ChessMob::IsDraw(board));

			// king & bishop (on white tile) vs king & multiple bishops (on black tiles only)
			board[5][2] = new Bishop(false);
			Assert::AreEqual(false, ChessMob::IsDraw(board));
		}
	};
}