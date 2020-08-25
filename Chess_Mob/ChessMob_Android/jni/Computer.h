#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "List.h"
#include "GameState.h"
#include "Structs.h"

//#include <Windows.h>
//#include <fstream>
#include <time.h>

using namespace std;

#pragma once

//#define TEST friend class TestComputer;
#define TEST

namespace ChessMob
{
	class Computer
	{
	private:
		// For creating the move tree
		struct Node
		{
			// The move required for this move to be possible
			Node * Parent;

			// Future possible moves
			List<Node *> Children;

			// The move made to get to this node
			Move StoredMove;

			// The score used for determining the relative quality of the move
			int Score;



			// Default Constructor; sets Parent to null, 
			Node()
			{
				Parent = NULL;
				Children = List<Node *>();
				StoredMove = Move(Coord(0,0), Coord(0,0), NoPiece);
				Score = 0;
			}

			// Constructor; sets Parent, StoredMove, and Score to the given values
			Node(Node * parent, Move move)
			{
				Parent = parent;
				Children = List<Node *>();
				StoredMove = move;
				Score = INT_MIN;
			}

			// Deconstructor; deletes the Children nodes and remove its pointer from its parent's Children
			~Node()
			{
				List<Node *>::Iterator nodeIndex;
				// remove pointer from Parent's Children list
				if(Parent != NULL)
				{
					nodeIndex = Parent->Children.Begin();
					for(; nodeIndex != Parent->Children.End() && nodeIndex != NULL; ++nodeIndex)
					{
						if(*nodeIndex == this)
						{
							*nodeIndex = NULL;
							break;
						}
					}
				}

				// deletes the Children nodes
				nodeIndex = Children.Begin();
				for(; nodeIndex != Children.End() && nodeIndex != NULL; ++nodeIndex)
				{
					delete *nodeIndex;
				}
			}

			// a static comparison function for use by sorting functions
			static bool NodeCompare(Node* node1, Node* node2)
			{
				return node1->Score > node2->Score;
			}

			static bool NodeMoveCompare(Node* node1, Node* node2)
			{
				if(node1->StoredMove.Capture != NoPiece && node2->StoredMove.Capture == NoPiece)
				{
					return true;
				}
				if(node2->StoredMove.Capture != NoPiece && node1->StoredMove.Capture == NoPiece)
				{
					return false;
				}
				return true;
			}
		};

		Node * MoveTreeHead;
		int DifficultyLevel;
		int TrimThreshold;
		bool IsWhite;

		// limits time computer has to make decision
		time_t Timer;

		// amount of time computer has to make decision
		time_t CompPlayTime;

		// Inserts a move onto MoveTree as a new node branching from parent, always at the front of the parents list
		void Insert(Node * parent, Move move)
		{
			Node * newNode = new Node(parent, move);
			parent->Children.PushFront(newNode);
		}

		/*
		Changes the head node of MoveTree and deletes the old head.
		This also delets the other nodes not branching from
		the newHead.
		*/
		void ChangeHead(Node * newHead)
		{
			// removes newHead from the tree so it is not deleted with the rest of the tree
			List<Node *>::Iterator newHeadIndex = newHead->Parent->Children.Begin();
			for(; newHeadIndex != newHead->Parent->Children.End() && newHeadIndex != NULL; ++newHeadIndex)
			{
				if(*newHeadIndex == newHead)
				{
					*newHeadIndex = NULL;
					break;
				}
			}

			// sets the Parent to NULL for security
			newHead->Parent = NULL;

			// deletes the rest of the tree
			delete MoveTreeHead;

			// sets newHead as the new head of the MoveTree
			MoveTreeHead = newHead;
		}

		static bool MoveCompare(Move move1, Move move2)
		{
			if(move1.Capture != NoPiece && move2.Capture == NoPiece)
			{
				return true;
			}
			if(move2.Capture != NoPiece && move1.Capture == NoPiece)
			{
				return false;
			}
			return true;
		}

		// determines which check determining function
		bool isComputerCheck(Piece * board[8][8], PieceType oppPieceType, Coord kingLoc, std::vector<Coord> & blockCheckCoords, bool isWhiteTurn)
		{
			King king(isWhiteTurn);
			switch(oppPieceType)
			{
			case QueenPiece:
				{
					return king.IsInQueenCheck(kingLoc, board, blockCheckCoords);
					break;
				}
			case RookPiece:
				{
					return king.IsInRookCheck(kingLoc, board, blockCheckCoords);
					break;
				}
			case BishopPiece:
				{
					return king.IsInBishopCheck(kingLoc, board, blockCheckCoords);
					break;
				}
			case KnightPiece:
				{
					return king.IsInKnightCheck(kingLoc, board, blockCheckCoords);
					break;
				}
			default:
				{
					return king.IsInCheck(kingLoc, board, blockCheckCoords);
					break;
				}
			};
		}

		// scores if player piece type is queen
		int ScoreQueen(Piece * board[8][8], List<Coord> & pieces, int & numMoves, bool isInCheck, std::vector<Coord> & blockCoords)
		{
			int queenScore = 0;
			if(numMoves < 0)
			{
				queenScore = QueenScore*(pieces.GetSize()-1);
				numMoves = 0;
			}
			else
			{
				// first piece is king
				List<Coord>::Iterator pieceIterator = pieces.Begin();
				bool isWhite = board[(*pieceIterator).Row][(*pieceIterator).Col]->IsWhitePiece();
				board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
				numMoves = board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
				++pieceIterator;


				for(; pieceIterator != pieces.End(); ++pieceIterator)
				{
					//as I keep track of locations, not pieces and do not remove locations if piece is taken, there may be duplicates

					board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
					numMoves += board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
					queenScore += QueenScore;

				}
			}

			// no queen pieces so cannot win
			if(queenScore == 0)
			{
				queenScore -= CannotWinScore;
			}

			return queenScore+numMoves;
		}

		// scores if player piece type is rook
		int ScoreRook(Piece * board[8][8], List<Coord> & pieces, int & numMoves, bool isInCheck, std::vector<Coord> & blockCoords)
		{
			int rookScore = 0;
			if(numMoves < 0)
			{
				rookScore = RookScore*(pieces.GetSize()-1);
				numMoves = 0;
			}
			else
			{
				// first piece is king
				List<Coord>::Iterator pieceIterator = pieces.Begin();
				bool isWhite = board[(*pieceIterator).Row][(*pieceIterator).Col]->IsWhitePiece();
				board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
				numMoves = board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
				++pieceIterator;


				for(; pieceIterator != pieces.End(); ++pieceIterator)
				{
					board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
					numMoves += board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
					rookScore += RookScore;
				}
			}

			// no rook pieces so cannot win
			if(rookScore == 0)
			{
				rookScore -= CannotWinScore;
			}

			return rookScore+numMoves;
		}

		// scores if player piece type is queen
		int ScoreBishop(Piece * board[8][8], List<Coord> & pieces, int & numMoves, bool isInCheck, std::vector<Coord> & blockCoords)
		{
			// king is first piece
			List<Coord>::Iterator pieceIterator = pieces.Begin();
			bool isWhite = board[(*pieceIterator).Row][(*pieceIterator).Col]->IsWhitePiece();
			board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
			numMoves = board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
			++pieceIterator;
			int numOddBishops = 0;
			int numEvenBishops = 0;
			int bishopScore = 0;

			for(; pieceIterator != pieces.End(); ++pieceIterator)
			{
				if(numMoves != -1)
				{
					board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
					numMoves += board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
				}

				if((*pieceIterator).Row+(*pieceIterator).Col & 1)
				{
					numOddBishops++;
				}
				else
				{
					numEvenBishops++;
				}
			}

			// bishops cannot force checkmate without one of each type
			if(numOddBishops < 1 || numEvenBishops < 1)
			{
				bishopScore -= CannotWinScore;
			}

			//regular score for paired bishops
			for(; numOddBishops > 0 && numEvenBishops > 0; numOddBishops--, numEvenBishops--)
			{
				bishopScore += PairedBishopScore;
			}

			//score for unpaired bishops
			if(numOddBishops > 0)
			{
				bishopScore += numOddBishops*UnpairedBishopScore;
			}
			else
			{
				bishopScore += numEvenBishops*UnpairedBishopScore;
			}

			if(numMoves == -1)
			{
				numMoves = 0;
			}

			return bishopScore+numMoves;
		}

		//scores for knight pieces
		int ScoreKnight(Piece * board[8][8], List<Coord> & pieces, int & numMoves, bool isInCheck, std::vector<Coord> & blockCoords)
		{
			int knightScore = 0;
			if(numMoves < 0)
			{
				knightScore = KnightScore*(pieces.GetSize()-1);
				numMoves = 0;
			}
			else
			{
				//first piece is king
				List<Coord>::Iterator pieceIterator = pieces.Begin();
				bool isWhite = board[(*pieceIterator).Row][(*pieceIterator).Col]->IsWhitePiece();
				board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
				numMoves = board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
				++pieceIterator;


				for(; pieceIterator != pieces.End(); ++pieceIterator)
				{
					board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
					numMoves += board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
					knightScore += KnightScore;

				}
			}

			//make sure enough knights to force checkmate
			if(knightScore < 4*KnightScore)
			{
				knightScore -= CannotWinScore;
			}

			return knightScore+numMoves;
		}

		// score for pawns
		int ScorePawn(Piece * board[8][8], List<Coord> & pieces, int & numMoves, bool isInCheck, std::vector<Coord> & blockCoords)
		{
			//kings is first piece
			List<Coord>::Iterator pieceIterator = pieces.Begin();
			bool isWhite = board[(*pieceIterator).Row][(*pieceIterator).Col]->IsWhitePiece();
			board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
			numMoves = board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
			++pieceIterator;
			int pawnScore = 0;

			for(; pieceIterator != pieces.End(); ++pieceIterator)
			{
				if(numMoves != -1)
				{
					board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCoords);
					numMoves += board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves().size();
				}

				switch(board[(*pieceIterator).Row][(*pieceIterator).Col]->GetType())
				{
				case PawnPiece:
					{
						pawnScore += PawnScore;
						break;
					}
				case QueenPiece:
					{
						pawnScore += QueenScore;
						break;
					}
				case RookPiece:
					{
						pawnScore += RookScore;
						break;
					}
				case BishopPiece:
					{
						pawnScore += UnpairedBishopScore;
						break;
					}
				case KnightScore:
					{
						pawnScore += KnightScore;
						break;
					}
				};

			}

			// if no pawns, cannot win
			if(pawnScore == 0)
			{
				pawnScore -= CannotWinScore;
			}

			if(numMoves < 0)
			{
				numMoves = 0;
			}

			return pawnScore+numMoves;
		}

		//scores player's pieces by type
		int scorePieceType(Piece * board[8][8], List<Coord> & pieces, PieceType pieceType, int & numMoves, bool isInCheck, std::vector<Coord> & blockCoords)
		{
			switch(pieceType)
			{
				// just a king
			case NoPiece:
				{
					board[(*pieces.Begin()).Row][(*pieces.Begin()).Col]->GeneratePossibleMoves(*pieces.Begin(), board, isInCheck, blockCoords);
					numMoves = board[(*pieces.Begin()).Row][(*pieces.Begin()).Col]->GetPossibleMoves().size();
					return -CannotWinScore;
					break;
				}
			case QueenPiece:
				{
					return ScoreQueen(board, pieces, numMoves, isInCheck, blockCoords);
					break;
				}
			case RookPiece:
				{
					return ScoreRook(board, pieces, numMoves, isInCheck, blockCoords);
					break;
				}
			case BishopPiece:
				{
					return ScoreBishop(board, pieces, numMoves, isInCheck, blockCoords);
					break;
				}
			case KnightPiece:
				{
					return ScoreKnight(board, pieces, numMoves, isInCheck, blockCoords);
					break;
				}
			case PawnPiece:
				{
					return ScorePawn(board, pieces, numMoves, isInCheck, blockCoords);
					break;
				}
			default:
				{
					numMoves = 0;
					return 0;
				}
			};
		}

		// 
		int ScoreNode(Piece * board[8][8], bool isWhiteTurn, List<Coord> & currentPieces, List<Coord> & otherPieces, PieceType currentPieceType, PieceType otherPieceType)
		{
			int tempScore = 0;
			List<Coord>::Iterator myPieceIterator = currentPieces.Begin();
			bool isInCheck = false;
			std::vector<Coord> blockCoords;
			int numPossibleMoves = 0;
			int oppNumPossibleMoves = -1;

			isInCheck = isComputerCheck(board, otherPieceType, *myPieceIterator, blockCoords, isWhiteTurn);

			if(isWhiteTurn == IsWhite)
			{
				tempScore += scorePieceType(board, currentPieces, currentPieceType, numPossibleMoves, isInCheck, blockCoords);
				tempScore -= scorePieceType(board, otherPieces, otherPieceType, oppNumPossibleMoves, false, blockCoords);

				if(numPossibleMoves == 0)
				{
					//checkmate
					if(isInCheck)
					{
						tempScore = -CheckmateScore;
					}
					//stalemate
					else
					{
						switch(currentPieceType)
						{
						case KnightPiece:
							{
								if(currentPieces.GetSize() < 4)
								{
									tempScore = CannotWinScore;
								}
								else
								{
									tempScore = -CannotWinScore;
								}
								break;
							}
						case BishopPiece:
							{
								if(currentPieces.GetSize() < 3)
								{
									tempScore = CannotWinScore;
								}
								else
								{
									tempScore =  -CannotWinScore;
								}
								break;
							}
						case NoPiece:
							{
								tempScore = CannotWinScore;
								break;
							}
						default:
							{
								if(currentPieces.GetSize() < 2)
								{
									tempScore = CannotWinScore;
								}
								else
								{
									tempScore -= CannotWinScore;
								}
							}
						};
					}
				}
				// check
				else if(isInCheck)
				{
				}
			}
			else
			{
				tempScore -= scorePieceType(board, currentPieces, currentPieceType, numPossibleMoves, isInCheck, blockCoords);
				tempScore += scorePieceType(board, otherPieces, otherPieceType, oppNumPossibleMoves, false, blockCoords);

				if(numPossibleMoves == 0)
				{
					//checkmate
					if(isInCheck)
					{
						tempScore = +CheckmateScore;
					}
					//stalemate
					else
					{
						switch(currentPieceType)
						{
						case KnightPiece:
							{
								if(currentPieces.GetSize() < 4)
								{
									tempScore = -CannotWinScore;
								}
							}
						case BishopPiece:
							{
								if(currentPieces.GetSize() < 3)
								{
									tempScore = -CannotWinScore;
								}
								break;
							}
						case NoPiece:
							{
								tempScore = -CannotWinScore;
								break;
							}
						default:
							{
								if(currentPieces.GetSize() < 2)
								{
									tempScore = -CannotWinScore;
								}
							}
						};
					}
				}
				// check
				else if(isInCheck)
				{
				}
			}

			return tempScore;
		}


		//// used for debugging, outputs the board for windows console
		//void printBoard(Piece* board[8][8])
		//{
		//	HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );


		//	SetConsoleTextAttribute(hConsole, 0x08);

		//	std::cout << "col:  ";
		//	for(int col = 0; col < 8; col++)
		//	{
		//		std::cout << " " << col;
		//	}

		//	for(int row = 0; row < 8; row++)
		//	{

		//		std::cout << std::endl;
		//		std::cout << "row " << row << ":";
		//		for(int col = 0; col < 8; col++)
		//		{
		//			int piece = NoPiece;
		//			if(board[row][col])
		//			{
		//				piece = board[row][col]->GetType();
		//			}

		//			bool isWhite = true;

		//			SetConsoleTextAttribute(hConsole, 0x07);

		//			if(piece < 0)
		//			{
		//				piece = -1*piece;
		//				isWhite = false;
		//				SetConsoleTextAttribute(hConsole, 0x0C);
		//			}

		//			switch(piece)
		//			{
		//			case NoPiece:
		//				{
		//					SetConsoleTextAttribute(hConsole, 0x08);
		//					std::cout << " 0";
		//					break;
		//				}
		//			case KingPiece:
		//				{
		//					std::cout << " K";
		//					break;
		//				}
		//			case QueenPiece:
		//				{
		//					std::cout << " Q";
		//					break;
		//				}
		//			case RookPiece:
		//				{
		//					std::cout << " R";
		//					break;
		//				}
		//			case BishopPiece:
		//				{
		//					std::cout << " B";
		//					break;
		//				}
		//			case KnightPiece:
		//				{
		//					std::cout << " N";
		//					break;
		//				}
		//			case PawnPiece:
		//				{
		//					std::cout << " P";
		//					break;
		//				}
		//			default:
		//				{
		//					std::cout << " !";
		//					break;
		//				}
		//			}
		//		}
		//		SetConsoleTextAttribute( hConsole, 0x08);
		//	}
		//}

		void UpdateComputerBoard1(Piece * board[8][8], char to_row, char to_col, char from_row, char from_col,
			PieceType &capture, bool isWhiteTurn, bool &isPawn)
		{
			//update board
			//capture piece
			if(board[to_row][to_col])
			{
				capture = board[to_row][to_col]->GetType();
				delete board[to_row][to_col];
			}
			//move piece
			board[to_row][to_col] = board[from_row][from_col];
			board[from_row][from_col] = NULL;

			//update pawn to queen
			if(board[to_row][to_col]->GetType() == PawnPiece && ((isWhiteTurn && to_row == 0 )
				|| (!isWhiteTurn && to_row == 7)))
			{
				delete board[to_row][to_col];
				board[to_row][to_col] = new Queen(isWhiteTurn);
				isPawn = true;
			}
		}

		void UpdateComputerBoard2(Piece * board[8][8], char to_row, char to_col, char from_row, char from_col, PieceType capture,
			bool isWhiteTurn, bool &isPawn)
		{
			Piece * capturePiece = NULL;

			switch(capture)
			{
			case NoPiece:
				{
					capturePiece = NULL;
					break;
				}
			case PawnPiece:
				{
					capturePiece = new Pawn(!isWhiteTurn, false);
					break;
				}
			case KnightPiece:
				{
					capturePiece = new Knight(!isWhiteTurn);
					break;
				}
			case BishopPiece:
				{
					capturePiece = new Bishop(!isWhiteTurn);
					break;
				}
			case RookPiece:
				{
					capturePiece = new Rook(!isWhiteTurn);
					break;
				}
			case QueenPiece:
				{
					capturePiece = new Queen(!isWhiteTurn);
					break;
				}
			default:
				{
					capturePiece = NULL;
					//std::cout << "\n**trouble**\n";
				}
			};

			//a pawn was converted, convert back
			if(isPawn)
			{
				//return taken piece/remove current player's piece
				delete board[to_row][to_col];
				board[to_row][to_col] = new Pawn(isWhiteTurn);
				isPawn = false;
			}

			//update board
			board[from_row][from_col] = board[to_row][to_col];
			board[to_row][to_col] = capturePiece;
			capturePiece = NULL;
		}

		////generates move tree and scores, based off alpha-beta min-max trimming
		//int ABTree(Piece * board[8][8], bool isWhiteTurn, /*Node * branchHead,*/ int currentDepth, List<Coord> & currentPieces, List<Coord> & otherPieces,
		//	PieceType currentPieceType, PieceType otherPieceType, int &alpha, int &beta)
		//{


		//	// looked as many moves ahead as willing to look, score node and return
		//	if(currentDepth == DifficultyLevel)
		//	{
		//		/*branchHead->Score =*/
		//		return ScoreNode(board, isWhiteTurn, currentPieces, otherPieces, currentPieceType, otherPieceType);
		//		//printBoard(board);
		//		//std::cout << branchHead->Score << std::endl;
		//		//return branchHead->Score;
		//	}

		//	bool isPawn = false;
		//	int moveIndex;
		//	int childScore;
		//	Piece* capturePiece = NULL;
		//	std::vector<Move> possibleMoves;

		//	List<Coord>::Iterator pieceIterator;
		//	int numberOfMoves = 0;

		//	bool isInCheck = false;
		//	std::vector<Coord> blockCheckCoords;

		//	PieceType capture = NoPiece;

		//	pieceIterator = currentPieces.Begin();


		//	isInCheck = isComputerCheck(board, otherPieceType, (*pieceIterator), blockCheckCoords, isWhiteTurn);

		//	int score = isWhiteTurn == IsWhite ? INT_MIN : INT_MAX;

		//	//go through all the pieces possible moves as they are all branches of this one node
		//	for(pieceIterator = currentPieces.Begin(); pieceIterator != currentPieces.End(); ++pieceIterator)
		//	{
		//		//as I keep track of locations, not pieces and do not remove locations if piece is taken, there may be duplicates
		//		bool repeat = false;
		//		for(List<Coord>::Iterator repeatIterator = currentPieces.Begin(); repeatIterator != pieceIterator; ++repeatIterator)
		//		{
		//			if(*repeatIterator == *pieceIterator)
		//			{
		//				repeat = true;
		//				break;
		//			}
		//		}

		//		if(repeat)
		//		{
		//			continue;
		//		}


		//		//checks piece still exist
		//		if(board[(*pieceIterator).Row][(*pieceIterator).Col] && board[(*pieceIterator).Row][(*pieceIterator).Col]->IsWhitePiece() == isWhiteTurn)
		//		{
		//			board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCheckCoords);
		//			possibleMoves = board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves();

		//			numberOfMoves += possibleMoves.size();

		//			//each possible move is the next level for the move tree
		//			for(moveIndex = 0; moveIndex < (int)possibleMoves.size(); moveIndex++)
		//			{
		//				//update board
		//				UpdateComputerBoard1(board, possibleMoves[moveIndex].To.Row, possibleMoves[moveIndex].To.Col,
		//					(*pieceIterator).Row, (*pieceIterator).Col, capture, isWhiteTurn, isPawn);

		//				//update my piece location
		//				(*pieceIterator).Row = possibleMoves[moveIndex].To.Row;
		//				(*pieceIterator).Col = possibleMoves[moveIndex].To.Col;

		//				//go a level deeper
		//				childScore = ABTree(board, !isWhiteTurn, /**branchHead->Children.Begin(),*/ currentDepth+1, otherPieces, currentPieces,
		//					otherPieceType, currentPieceType, alpha, beta);

		//				//update current player's piece list
		//				(*pieceIterator).Row = possibleMoves[moveIndex].From.Row;
		//				(*pieceIterator).Col = possibleMoves[moveIndex].From.Col;

		//				UpdateComputerBoard2(board, possibleMoves[moveIndex].To.Row, possibleMoves[moveIndex].To.Col,
		//					(*pieceIterator).Row, (*pieceIterator).Col, capture,  isWhiteTurn, isPawn);

		//				capture = NoPiece;

		//				//it is my turn/move
		//				if(isWhiteTurn == IsWhite)
		//				{
		//					// if I found a better score I would choose it
		//					if(childScore > alpha)
		//					{
		//						alpha = childScore;
		//						score = childScore;

		//						//the only way I can stop looking is if my score has changed
		//						//if I can make a choice better here than in another configuration, my opponent will choose the other configuration
		//						if(beta <= alpha)
		//						{
		//							//to break double for loop, increase values to break the for loop statement
		//							pieceIterator = currentPieces.End();
		//							break;
		//						}
		//					}
		//				}
		//				else
		//				{
		//					//opponents move, will choose move that is least beneficial to me
		//					if(childScore < beta)
		//					{
		//						beta = childScore;
		//						score = childScore;

		//						if(beta <= alpha)
		//						{
		//							//to break triple for loop, increase values to break the for loop statement
		//							pieceIterator = currentPieces.End();
		//							break;
		//						}						
		//					}
		//				}
		//				if(time(NULL) > Timer)
		//				{
		//					// uncomment this and it will only work for the assigned time limit
		//					//return score;//branchHead->Score;
		//				}
		//			}
		//		}
		//	}

		//	//if player is in checkmate, the branch is done and there is no score yet assigned.
		//	if(numberOfMoves == 0)
		//	{
		//		if(isInCheck)
		//		{
		//			//if I am trying to make a move and the state is in checkmate = bad
		//			return isWhiteTurn == IsWhite ? -CheckmateScore : CheckmateScore;
		//		}
		//		//stalemate
		//		else
		//		{
		//			int multiplier = isWhiteTurn == IsWhite ? 1 : -1;
		//			switch(currentPieceType)
		//			{
		//			case KnightPiece:
		//				{
		//					// need 3 knights to force checkmate plus king
		//					if(currentPieces.GetSize() < 4)
		//						/*branchHead->Score =*/ return multiplier*CannotWinScore;
		//					break;
		//				}
		//			case BishopPiece:
		//				{
		//					// need 2 bishops to force checkmate plus king
		//					if(currentPieces.GetSize() < 3)
		//						/*branchHead->Score =*/ return multiplier*CannotWinScore;
		//					break;
		//				}
		//			case NoPiece:
		//				{
		//					/*branchHead->Score =*/ return multiplier*CannotWinScore;
		//					break;
		//				}
		//			default :
		//				{
		//					// need 1 of everything else to force checkmate plus king
		//					if(currentPieces.GetSize() < 2)
		//						/*branchHead->Score =*/ return multiplier*CannotWinScore;
		//					break;
		//				}
		//			};
		//		}
		//	}
		//	return score;//branchHead->Score;
		//}

		//generates move tree and scores, based off alpha-beta min-max trimming
		int ABTree(Piece * board[8][8], bool isWhiteTurn, /*Node * branchHead,*/ int currentDepth, List<Coord> & currentPieces, List<Coord> & otherPieces,
			PieceType currentPieceType, PieceType otherPieceType, int alpha, int beta)
		{
			// looked as many moves ahead as willing to look, score node and return
			if(currentDepth == DifficultyLevel)
			{
				/*branchHead->Score =*/
				return ScoreNode(board, isWhiteTurn, currentPieces, otherPieces, currentPieceType, otherPieceType);
				//printBoard(board);
				//std::cout << branchHead->Score << std::endl;
				//return branchHead->Score;
			}

			bool isPawn = false;
			int moveIndex;
			int childScore;
			Piece* capturePiece = NULL;
			std::vector<Move> possibleMoves;
			List<Move> allPossibleMoves;

			List<Coord>::Iterator pieceIterator;
			int numberOfMoves = 0;

			bool isInCheck = false;
			std::vector<Coord> blockCheckCoords;

			PieceType capture = NoPiece;

			pieceIterator = currentPieces.Begin();




			isInCheck = isComputerCheck(board, otherPieceType, (*pieceIterator), blockCheckCoords, isWhiteTurn);

			//int score = isWhiteTurn == IsWhite ? INT_MIN : INT_MAX;

			//go through all the pieces possible moves as they are all branches of this one node
			for(pieceIterator = currentPieces.Begin(); pieceIterator != currentPieces.End(); ++pieceIterator)
			{
				board[(*pieceIterator).Row][(*pieceIterator).Col]->GeneratePossibleMoves(*pieceIterator, board, isInCheck, blockCheckCoords);
				possibleMoves = board[(*pieceIterator).Row][(*pieceIterator).Col]->GetPossibleMoves();

				//each possible move is the next level for the move tree
				for(moveIndex = 0; moveIndex < (int)possibleMoves.size(); moveIndex++)
				{
					allPossibleMoves.PushFront(possibleMoves[moveIndex]);
				}
			}

			//if player is in checkmate, the branch is done and there is no score yet assigned.
			if(allPossibleMoves.GetSize() == 0)
			{
				if(isInCheck)
				{
					//if I am trying to make a move and the state is in checkmate = bad
					return isWhiteTurn == IsWhite ? -CheckmateScore : CheckmateScore;
				}
				//stalemate
				else
				{
					int multiplier = isWhiteTurn == IsWhite ? 1 : -1;
					switch(currentPieceType)
					{
					case KnightPiece:
						{
							// need 3 knights to force checkmate plus king
							if(currentPieces.GetSize() < 4)
								/*branchHead->Score =*/ return multiplier*CannotWinScore;
							break;
						}
					case BishopPiece:
						{
							// need 2 bishops to force checkmate plus king
							if(currentPieces.GetSize() < 3)
								/*branchHead->Score =*/ return multiplier*CannotWinScore;
							break;
						}
					case NoPiece:
						{
							/*branchHead->Score =*/ return multiplier*CannotWinScore;
							break;
						}
					default :
						{
							// need 1 of everything else to force checkmate plus king
							if(currentPieces.GetSize() < 2)
								/*branchHead->Score =*/ return multiplier*CannotWinScore;
							break;
						}
					};
					return -CannotWinScore;
				}
			}

			allPossibleMoves.Sort( &MoveCompare );

			List<Move>::Iterator moveIterator = allPossibleMoves.Begin();

			if(isWhiteTurn == IsWhite)
			{

				for(; moveIterator != allPossibleMoves.End(); ++moveIterator)
				{

					//update board
					UpdateComputerBoard1(board, (*moveIterator).To.Row, (*moveIterator).To.Col,
						(*moveIterator).From.Row, (*moveIterator).From.Col, capture, isWhiteTurn, isPawn);


					for(pieceIterator = currentPieces.Begin(); pieceIterator != currentPieces.End(); ++pieceIterator)
					{
						if(*pieceIterator == (*moveIterator).From)
						{
							(*pieceIterator).Row = (*moveIterator).To.Row;
							(*pieceIterator).Col = (*moveIterator).To.Col;
							break;
						}
					}

					if((*moveIterator).Capture != NoPiece)
					{
						otherPieces.Remove((*moveIterator).To);
					}

					//std::cout << "debug:" << (int)(*moveIterator).From.Row << " " << (int)(*moveIterator).From.Col << " " << (int)(*moveIterator).To.Row << " " << (int)(*moveIterator).To.Col << std::endl;

					//go a level deeper
					childScore = ABTree(board, !isWhiteTurn, /**branchHead->Children.Begin(),*/ currentDepth+1, otherPieces, currentPieces,
						otherPieceType, currentPieceType, alpha, beta);


					for(pieceIterator = currentPieces.Begin(); pieceIterator != currentPieces.End(); ++pieceIterator)
					{
						if(*pieceIterator == (*moveIterator).To)
						{
							(*pieceIterator).Row = (*moveIterator).From.Row;
							(*pieceIterator).Col = (*moveIterator).From.Col;
							break;
						}
					}

					if((*moveIterator).Capture != NoPiece)
					{
						Coord kingLoc = *otherPieces.Begin();
						otherPieces.PopFront();
						otherPieces.PushFront((*moveIterator).To);
						otherPieces.PushFront(kingLoc);
					}



					UpdateComputerBoard2(board, (*moveIterator).To.Row, (*moveIterator).To.Col,
						(*moveIterator).From.Row, (*moveIterator).From.Col, capture,  isWhiteTurn, isPawn);

					capture = NoPiece;

					if(childScore > alpha)
					{
						alpha = childScore;
						if(beta <= alpha)
						{
							break;
						}
					}
				}
				return alpha;

			}
			else
			{
				for(; moveIterator != allPossibleMoves.End(); ++moveIterator)
				{

					//update board
					UpdateComputerBoard1(board, (*moveIterator).To.Row, (*moveIterator).To.Col,
						(*moveIterator).From.Row, (*moveIterator).From.Col, capture, isWhiteTurn, isPawn);


					for(pieceIterator = currentPieces.Begin(); pieceIterator != currentPieces.End(); ++pieceIterator)
					{
						if(*pieceIterator == (*moveIterator).From)
						{
							(*pieceIterator).Row = (*moveIterator).To.Row;
							(*pieceIterator).Col = (*moveIterator).To.Col;
							break;
						}
					}

					if((*moveIterator).Capture != NoPiece)
					{
						otherPieces.Remove((*moveIterator).To);
					}

					//std::cout << "debug:" << (int)(*moveIterator).From.Row << " " << (int)(*moveIterator).From.Col << " " << (int)(*moveIterator).To.Row << " " << (int)(*moveIterator).To.Col << std::endl;

					//go a level deeper
					childScore = ABTree(board, !isWhiteTurn, /**branchHead->Children.Begin(),*/ currentDepth+1, otherPieces, currentPieces,
						otherPieceType, currentPieceType, alpha, beta);


					for(pieceIterator = currentPieces.Begin(); pieceIterator != currentPieces.End(); ++pieceIterator)
					{
						if(*pieceIterator == (*moveIterator).To)
						{
							(*pieceIterator).Row = (*moveIterator).From.Row;
							(*pieceIterator).Col = (*moveIterator).From.Col;
							break;
						}
					}

					if((*moveIterator).Capture != NoPiece)
					{
						Coord kingLoc = *otherPieces.Begin();
						otherPieces.PopFront();
						otherPieces.PushFront((*moveIterator).To);
						otherPieces.PushFront(kingLoc);
					}



					UpdateComputerBoard2(board, (*moveIterator).To.Row, (*moveIterator).To.Col,
						(*moveIterator).From.Row, (*moveIterator).From.Col, capture,  isWhiteTurn, isPawn);

					capture = NoPiece;

					if(childScore < beta)
					{
						beta = childScore;
						if(beta <= alpha)
						{
							break;
						}
					}

				}
				return beta;
			}

			////it is my turn/move
			//if(isWhiteTurn == IsWhite)
			//{
			//	// if I found a better score I would choose it
			//	if(childScore >= alpha)
			//	{
			//		alpha = childScore;
			//		score = childScore;

			//		//the only way I can stop looking is if my score has changed
			//		//if I can make a choice better here than in another configuration, my opponent will choose the other configuration
			//		if(beta <= alpha)
			//		{
			//			return alpha
			//			//to break double for loop, increase values to break the for loop statement
			//			//break;
			//		}
			//	}
			//}
			//else
			//{
			//	//opponents move, will choose move that is least beneficial to me
			//	if(childScore <= beta)
			//	{
			//		beta = childScore;
			//		score = childScore;

			//		if(beta <= alpha)
			//		{
			//			return beta;
			//			//to break triple for loop, increase values to break the for loop statement
			//			//break;
			//		}						
			//	}
			//}
			//if(time(NULL) > Timer)
			//{
			//	// uncomment this and it will only work for the assigned time limit
			//	//return score;//branchHead->Score;
			//}
		//}



		//return score;//branchHead->Score;
	}

	//// start of alpha beta check
	//void CreateTree(Piece * board[8][8], PieceType myPieceType, List<Coord> & myPieces, PieceType oppPieceType, List<Coord> & oppPieces)
	//{
	//	List<Coord>::Iterator myPieceIterator = myPieces.Begin();
	//	std::vector<Move> possibleMoves;
	//	std::vector<Coord> blockCheckCoords;

	//	bool isInCheck = isComputerCheck(board, oppPieceType, *myPieceIterator, blockCheckCoords, IsWhite);
	//	bool isPawn = false;
	//	PieceType capture = NoPiece;

	//	int alpha = INT_MIN;
	//	int beta = INT_MAX;

	//	// create first layer of move tree
	//	for(++myPieceIterator; myPieceIterator != myPieces.End(); ++myPieceIterator)
	//	{
	//		board[(*myPieceIterator).Row][(*myPieceIterator).Col]->GeneratePossibleMoves(*myPieceIterator, board, isInCheck, blockCheckCoords);
	//		possibleMoves = board[(*myPieceIterator).Row][(*myPieceIterator).Col]->GetPossibleMoves();

	//		for(int moveIterator = 0; moveIterator < (int)possibleMoves.size(); moveIterator++)
	//		{
	//			Insert(MoveTreeHead, possibleMoves[moveIterator]);

	//			UpdateComputerBoard1(board, possibleMoves[moveIterator].To.Row, possibleMoves[moveIterator].To.Col,
	//				possibleMoves[moveIterator].From.Row, possibleMoves[moveIterator].From.Col, capture, IsWhite, isPawn);

	//			(*myPieceIterator).Row = possibleMoves[moveIterator].To.Row;
	//			(*myPieceIterator).Col = possibleMoves[moveIterator].To.Col;

	//			(*(*MoveTreeHead->Children.Begin())).Score = ABTree(board, !IsWhite, 2, oppPieces, myPieces, oppPieceType,
	//				myPieceType, alpha, beta);

	//			(*myPieceIterator).Row = possibleMoves[moveIterator].From.Row;
	//			(*myPieceIterator).Col = possibleMoves[moveIterator].From.Col;

	//			UpdateComputerBoard2(board, possibleMoves[moveIterator].To.Row, possibleMoves[moveIterator].To.Col,
	//				possibleMoves[moveIterator].From.Row, possibleMoves[moveIterator].From.Col, capture, IsWhite, isPawn);
	//			capture = NoPiece;					

	//			if(time(NULL) > Timer)
	//			{
	//				// uncommit this and it will only work for the assigned time limit
	//				return;//branchHead->Score;
	//			}
	//		}
	//	}

	//	myPieceIterator = myPieces.Begin();

	//	board[(*myPieceIterator).Row][(*myPieceIterator).Col]->GeneratePossibleMoves(*myPieceIterator, board, isInCheck, blockCheckCoords);
	//	possibleMoves = board[(*myPieceIterator).Row][(*myPieceIterator).Col]->GetPossibleMoves();

	//	for(int moveIterator = 0; moveIterator < (int)possibleMoves.size(); moveIterator++)
	//	{
	//		Insert(MoveTreeHead, possibleMoves[moveIterator]);

	//		UpdateComputerBoard1(board, possibleMoves[moveIterator].To.Row, possibleMoves[moveIterator].To.Col,
	//			possibleMoves[moveIterator].From.Row, possibleMoves[moveIterator].From.Col, capture, IsWhite, isPawn);

	//		(*myPieceIterator).Row = possibleMoves[moveIterator].To.Row;
	//		(*myPieceIterator).Col = possibleMoves[moveIterator].To.Col;

	//		(*(*MoveTreeHead->Children.Begin())).Score = ABTree(board, !IsWhite, 2, oppPieces, myPieces, oppPieceType,
	//			myPieceType, alpha, beta);

	//		(*myPieceIterator).Row = possibleMoves[moveIterator].From.Row;
	//		(*myPieceIterator).Col = possibleMoves[moveIterator].From.Col;

	//		UpdateComputerBoard2(board, possibleMoves[moveIterator].To.Row, possibleMoves[moveIterator].To.Col,
	//			possibleMoves[moveIterator].From.Row, possibleMoves[moveIterator].From.Col, capture, IsWhite, isPawn);
	//		capture = NoPiece;					

	//		if(time(NULL) > Timer)
	//		{
	//			// uncommit this and it will only work for the assigned time limit
	//			return;//branchHead->Score;
	//		}
	//	}
	//}

	// start of alpha beta check
	void CreateTree(Piece * board[8][8], PieceType myPieceType, List<Coord> & myPieces, PieceType oppPieceType, List<Coord> & oppPieces)
	{
		List<Coord>::Iterator myPieceIterator = myPieces.Begin();
		std::vector<Move> possibleMoves;
		std::vector<Coord> blockCheckCoords;

		bool isInCheck = isComputerCheck(board, oppPieceType, *myPieceIterator, blockCheckCoords, IsWhite);
		bool isPawn = false;
		PieceType capture = NoPiece;

		int alpha = INT_MIN+1;
		int beta = INT_MAX;

		// create first layer of move tree
		for(; myPieceIterator != myPieces.End(); ++myPieceIterator)
		{
			board[(*myPieceIterator).Row][(*myPieceIterator).Col]->GeneratePossibleMoves(*myPieceIterator, board, isInCheck, blockCheckCoords);
			possibleMoves = board[(*myPieceIterator).Row][(*myPieceIterator).Col]->GetPossibleMoves();

			for(int moveIterator = 0; moveIterator < (int)possibleMoves.size(); moveIterator++)
			{
				Insert(MoveTreeHead, possibleMoves[moveIterator]);
			}
		}

		MoveTreeHead->Children.Sort( &Node::NodeMoveCompare );

		List<Node*>::Iterator myMoveIterator = MoveTreeHead->Children.Begin();
		for(; myMoveIterator != MoveTreeHead->Children.End(); ++myMoveIterator)
		{
			UpdateComputerBoard1(board, (*myMoveIterator)->StoredMove.To.Row, (*myMoveIterator)->StoredMove.To.Col,
				(*myMoveIterator)->StoredMove.From.Row, (*myMoveIterator)->StoredMove.From.Col, capture, IsWhite, isPawn);

			for(myPieceIterator = myPieces.Begin(); myPieceIterator != myPieces.End(); ++myPieceIterator)
			{
				if(*myPieceIterator == (*myMoveIterator)->StoredMove.From)
				{
					(*myPieceIterator).Row = (*myMoveIterator)->StoredMove.To.Row;
					(*myPieceIterator).Col = (*myMoveIterator)->StoredMove.To.Col;
					break;
				}
			}

			if((*myMoveIterator)->StoredMove.Capture != NoPiece)
			{
				oppPieces.Remove((*myMoveIterator)->StoredMove.To);
			}

			List<Coord>::Iterator pieceIterator;

			alpha = ABTree(board, !IsWhite, 1, oppPieces, myPieces, oppPieceType,
				myPieceType, alpha-1, beta);

			(*myMoveIterator)->Score = alpha;

			UpdateComputerBoard2(board, (*myMoveIterator)->StoredMove.To.Row, (*myMoveIterator)->StoredMove.To.Col,
				(*myMoveIterator)->StoredMove.From.Row, (*myMoveIterator)->StoredMove.From.Col, capture, IsWhite, isPawn);
			capture = NoPiece;

			for(myPieceIterator = myPieces.Begin(); myPieceIterator != myPieces.End(); ++myPieceIterator)
			{
				if(*myPieceIterator == (*myMoveIterator)->StoredMove.To)
				{
					(*myPieceIterator).Row = (*myMoveIterator)->StoredMove.From.Row;
					(*myPieceIterator).Col = (*myMoveIterator)->StoredMove.From.Col;
					break;
				}
			}

			if((*myMoveIterator)->StoredMove.Capture != NoPiece)
			{
				Coord kingLoc = *oppPieces.Begin();
				oppPieces.PopFront();
				oppPieces.PushFront((*myMoveIterator)->StoredMove.To);
				oppPieces.PushFront(kingLoc);
			}

			if(time(NULL) > Timer)
			{
				// uncommit this and it will only work for the assigned time limit
				return;//branchHead->Score;
			}
		}
	}

	//// for debugging, not sure it works correctly, but it prints the scores supposedly in a tree structure
	//void printTree(Node * BranchHead, std::ofstream& myfile)
	//{
	//	if(BranchHead == NULL)
	//	{
	//		std::cout << std::endl;
	//		myfile << std::endl;
	//		return;
	//	}
	//	else
	//	{
	//		std::cout << BranchHead->Score << " ";
	//		myfile << BranchHead->Score << "\t";
	//		List<Node*>::Iterator branchIterator = BranchHead->Children.Begin();
	//		for(; branchIterator != BranchHead->Children.End(); ++branchIterator)
	//		{
	//			printTree(*branchIterator, myfile);
	//		}
	//	}
	//}

public:

	/* 
	Constructer; 
	sets IsWhite to the passed values.
	sets MoveTreeHead to a default Node
	sets DifficultyLevel to a value between 1 and 3, by picking the value closest the passed in value.
	*/
	Computer(int difficultyLevel, bool isWhite)
	{
		if(difficultyLevel < 1)
			difficultyLevel = 1;
		else if(difficultyLevel > 5)
			difficultyLevel = 5;
		DifficultyLevel = 2*difficultyLevel;
		IsWhite = isWhite;
		MoveTreeHead = new Node();
		TrimThreshold = 5;
		srand((unsigned int)time(NULL));

		//Computer has 10 seconds to make decisions
		CompPlayTime = 30;
	}

	~Computer()
	{
		delete MoveTreeHead;
	}

	// decides on the move the computer player makes
	Move MakeMove(Piece * board[8][8])
	{
		// board for the computer player
		Piece * computerBoard[8][8];

		for(int i = 0; i < 8; i++)
		{
			for(int k = 0; k < 8; k++)
			{
				computerBoard[i][k] = NULL;
			}
		}

		// list the coordinates of all the pieces to speed up time by reducing searches
		List<Coord> myPieces;
		List<Coord> oppPieces;
		Coord myKing;
		Coord oppKing;
		PieceType myPieceType = NoPiece, oppPieceType = NoPiece;

		int row, col;

		// transverses through the board to create the board for the computer player so the that board can be changed
		for(row = 0; row < 8; row++)
		{
			for(col = 0; col < 8; col++)
			{
				if(board[row][col] != NULL)
				{
					switch(board[row][col]->GetType())
					{
					case PawnPiece:
						{
							computerBoard[row][col] = new Pawn(board[row][col]->IsWhitePiece());
							if(board[row][col]->IsWhitePiece() == IsWhite)
							{
								myPieces.PushFront(Coord(row,col));
								myPieceType = PawnPiece;
							}
							else
							{
								oppPieces.PushFront(Coord(row,col));
								oppPieceType = PawnPiece;
							}
							break;
						}
					case KnightPiece:
						{
							computerBoard[row][col] = new Knight(board[row][col]->IsWhitePiece());
							if(board[row][col]->IsWhitePiece() == IsWhite)
							{
								myPieces.PushFront(Coord(row,col));
								if(myPieceType == NoPiece)
								{
									myPieceType = KnightPiece;
								}
								else if(myPieceType != KnightPiece)
								{
									myPieceType = PawnPiece;
								}
							}
							else
							{
								oppPieces.PushFront(Coord(row,col));
								if(oppPieceType == NoPiece)
								{
									oppPieceType = KnightPiece;
								}
								else if(oppPieceType != KnightPiece)
								{
									oppPieceType = PawnPiece;
								}
							}
							break;
						}
					case BishopPiece:
						{
							computerBoard[row][col] = new Bishop(board[row][col]->IsWhitePiece());
							if(board[row][col]->IsWhitePiece() == IsWhite)
							{
								myPieces.PushFront(Coord(row,col));
								if(myPieceType == NoPiece)
								{
									myPieceType = BishopPiece;
								}
								else if(myPieceType != BishopPiece)
								{
									myPieceType = PawnPiece;
								}
							}
							else
							{
								oppPieces.PushFront(Coord(row,col));
								if(oppPieceType == NoPiece)
								{
									oppPieceType = BishopPiece;
								}
								else if(oppPieceType != BishopPiece)
								{
									oppPieceType = PawnPiece;
								}
							}
							break;
						}
					case RookPiece:
						{
							computerBoard[row][col] = new Rook(board[row][col]->IsWhitePiece());
							if(board[row][col]->IsWhitePiece() == IsWhite)
							{
								myPieces.PushFront(Coord(row,col));
								if(myPieceType == NoPiece)
								{
									myPieceType = RookPiece;
								}
								else if(myPieceType != RookPiece)
								{
									myPieceType = PawnPiece;
								}
							}
							else
							{
								oppPieces.PushFront(Coord(row,col));
								if(oppPieceType == NoPiece)
								{
									oppPieceType = RookPiece;
								}
								else if(oppPieceType != RookPiece)
								{
									oppPieceType = PawnPiece;
								}
							}
							break;
						}
					case QueenPiece:
						{
							computerBoard[row][col] = new Queen(board[row][col]->IsWhitePiece());
							if(board[row][col]->IsWhitePiece() == IsWhite)
							{
								myPieces.PushFront(Coord(row,col));
								if(myPieceType == NoPiece)
								{
									myPieceType = QueenPiece;
								}
								else if(myPieceType != QueenPiece)
								{
									myPieceType = PawnPiece;
								}
							}
							else
							{
								oppPieces.PushFront(Coord(row,col));
								if(oppPieceType == NoPiece)
								{
									oppPieceType = QueenPiece;
								}
								else if(oppPieceType != QueenPiece)
								{
									oppPieceType = PawnPiece;
								}
							}
							break;
						}
					case KingPiece:
						{
							computerBoard[row][col] = new King(board[row][col]->IsWhitePiece());
							if(board[row][col]->IsWhitePiece() == IsWhite)
							{
								myKing.Row = row;
								myKing.Col = col;
							}
							else
							{
								oppKing.Row = row;
								oppKing.Col = col;
							}
							break;
						}
					}
				}
			}
		}

		// puts kings as first piece in list of pieces (great for finding king, trouble for generating moves with time limit
		myPieces.PushFront(myKing);
		oppPieces.PushFront(oppKing);

		List<Node *>::Iterator moveTreeIndex = MoveTreeHead->Children.Begin();

		//int alpha = INT_MIN;
		//int beta = INT_MAX;

		//used for time limitt
		Timer = time(NULL) + CompPlayTime;

		//ABTree(computerBoard, IsWhite, MoveTreeHead, 0, myPieces, oppPieces, myPieceType, oppPieceType);
		CreateTree(board, myPieceType, myPieces, oppPieceType, oppPieces);

		// sorts the first layer of the tree to find the best moves for this turn
		MoveTreeHead->Children.Sort( &Node::NodeCompare );

		// stores all moves of equal top score
		std::vector<Move> topMoves;

		// top score for current turn
		int maxScore;

		// index of move that is chosen
		int myMoveIndex;

		// goes through the first layer and pulls out the ones until a score is not the top score
		moveTreeIndex = MoveTreeHead->Children.Begin();
		maxScore = moveTreeIndex == NULL ? 0 : (*moveTreeIndex)->Score;
		for(; moveTreeIndex != MoveTreeHead->Children.End(); ++moveTreeIndex)
		{
			if((*moveTreeIndex) != NULL)
			{
				if( (*moveTreeIndex)->Score >= maxScore )
				{
					topMoves.push_back((*moveTreeIndex)->StoredMove);
				}
				else
				{
					break;
				}
			}
		}
		//If there are moves to select from then return one
		if(topMoves.size() > 0)
		{
			/*//debug stuff
			std::ofstream myfile;
			myfile.open("MoveTree.txt");
			printTree(MoveTreeHead, myfile);
			myfile.close();*/

			//randomly choose move
			myMoveIndex = rand() % topMoves.size();

			//reflect move on move tree
			Move StoredMove = topMoves[myMoveIndex];
			//ChangeHead(topmove);

			// return the chosen move
			/*if(StoredMove.From.Row == StoredMove.To.Row && StoredMove.From.Col == StoredMove.To.Col)
			{
				std::cout << "trouble" << std::endl;
			}*/

			delete MoveTreeHead;
			MoveTreeHead = new Node();

			//debugging using windows version, prints time for decision making
			//std::cout << "\n" << time(NULL)-(Timer-CompPlayTime);

			return StoredMove;
		}
		//Else return a blank move because checkmate, stalemate or a bug has occurred (There should always be moves)
		else 
		{
			delete MoveTreeHead;
			MoveTreeHead = new Node();
			Move move = Move();
			return move;
		}
	}

	PieceType ConvertPawn()
	{
		return QueenPiece;
	}

	bool IsCWhite()
	{
		return IsWhite;
	}
};
}
