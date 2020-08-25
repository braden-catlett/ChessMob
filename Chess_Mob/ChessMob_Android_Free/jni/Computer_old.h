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

#pragma once

//#define TEST friend class TestComputer;
#define TEST

const int BishopScore = 22568;

namespace ChessMob
{
	class Computer_Old
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
			float Score;

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
				Score = -1*FLT_MAX;
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
		};

		Node * MoveTreeHead;
		int DifficultyLevel;
		int TrimThreshold;
		bool IsWhite;

		// determines the score for the piece passed in and the associated board
		// isWhiteTurn
		// TODO: Add scores associated with check and checkmate
		float MoveScore(PieceType piece, Piece * board[8][8], bool isWhiteTurn)
		{
			float tempScore = 0;

			// updates score on piece taken
			switch(piece)
			{
			case NoPiece:
				{
					break;
				}
			case QueenPiece:
				{
					tempScore += QueenScore;
					break;
				}
			case RookPiece:
				{
					tempScore += RookScore;
					break;
				}
			case BishopPiece:
				{
					tempScore += BishopScore;
					break;
				}
			case KnightPiece:
				{
					tempScore += KnightScore;
					break;
				}
			case PawnPiece:
				{
					tempScore += PawnScore;
					break;
				}
			};

			std::vector<Coord> blockMoves;
			bool isInCheck = false;
			isInCheck = IsInCheck(board, !isWhiteTurn, blockMoves);
			// updates score on opponent being in check or checkmate
			if(isInCheck)
			{				
				// opponent is in checkmate
				isInCheck = IsStaleMate(board, !isWhiteTurn);
				if(isInCheck)
				{
					tempScore += CheckmateScore;
				}

				// opponent is in check
				else
				{
					tempScore += CheckScore;
				}
			}

			return tempScore;
		}

		// Inserts a move onto MoveTree as a new node branching from parent
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

		// function that actually adds the children of a branch
		void AddChildren(Node * parent, Piece * board[8][8], bool isWhiteTurn)
		{
			int row,col,moveIndex;
			float moveScore;
			Piece * capturePiece;
			std::vector<Move> possibleMoves;
			List<Node *>::Iterator newNode;

			bool isInCheck = false;
			std::vector<Coord> blockCheckCoords;
			
			isInCheck = ChessMob::IsInCheck(board, isWhiteTurn, blockCheckCoords);

			// search every board location for pieces
			for(row = 0; row < 8; row++)
			{
				for(col = 0; col < 8; col++)
				{
					// checks piece of certain player's turn
					if(board[row][col] != NULL && board[row][col]->IsWhitePiece() == isWhiteTurn)
					{						
						board[row][col]->GeneratePossibleMoves( Coord(row,col), board, isInCheck, blockCheckCoords);
						possibleMoves = board[row][col]->GetPossibleMoves();
						// adds every possible move to MoveTree
						for(moveIndex = 0; moveIndex < (int)possibleMoves.size(); moveIndex++)
						{
							Insert(parent, possibleMoves[moveIndex]);

							delete board[possibleMoves[moveIndex].To.Row][possibleMoves[moveIndex].To.Col];
							board[possibleMoves[moveIndex].To.Row][possibleMoves[moveIndex].To.Col] = board[row][col];
							board[row][col] = NULL;

							moveScore = MoveScore(possibleMoves[moveIndex].Capture, board, isWhiteTurn);
							newNode = parent->Children.Begin();
							if(isWhiteTurn == IsWhite)
							{								
								(*newNode)->Score = parent->Score + moveScore;
							}
							else
							{
								(*newNode)->Score = parent->Score - moveScore;
							}

							switch(possibleMoves[moveIndex].Capture)
							{
					
							// tries NoPiece first as highest probability
							case NoPiece:
								{
									capturePiece = NULL;
									break;
								}
							case PawnPiece:
								{
									capturePiece = new Pawn(!isWhiteTurn);
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
							case KingPiece:
								{
									capturePiece = new King(!isWhiteTurn);
									break;
								}
							};

							board[row][col] = board[possibleMoves[moveIndex].To.Row][possibleMoves[moveIndex].To.Col];
							board[possibleMoves[moveIndex].To.Row][possibleMoves[moveIndex].To.Col] = capturePiece;
						}
					}
				}
			}
		}

		// Adds one layer to MoveTree finding the bottom layer through a recursive depth first approach
		void UpdateMoveTreeRecursiveDepth(Node * branchHead, Piece * board[8][8], bool isWhiteTurn)
		{
			// used to replace pieces in board when transversing back up the tree
			Piece * capturePiece = NULL;
			
			List<Node *>::Iterator branchIndex;

			// if bottom of the tree, add children. 
			// **NOTE: this approach is inefficient for checkmate cases
			if(branchHead->Children.Empty())
			{
				AddChildren(branchHead, board, isWhiteTurn);
			}

			// Recursively call all branches UpdateMoveTree to find bottom branch
			else
			{
				// Go through all Children
				for(branchIndex = branchHead->Children.Begin(); branchIndex != branchHead->Children.End() && branchIndex != NULL; ++branchIndex)
				{
					// Update board for the new branch
					delete board[(*branchIndex)->StoredMove.To.Row][(*branchIndex)->StoredMove.To.Col];
					board[(*branchIndex)->StoredMove.To.Row][(*branchIndex)->StoredMove.To.Col] = board[(*branchIndex)->StoredMove.From.Row][(*branchIndex)->StoredMove.From.Col];
					board[(*branchIndex)->StoredMove.From.Row][(*branchIndex)->StoredMove.From.Col] = NULL;

					// Recursive call
					UpdateMoveTreeRecursiveDepth(*branchIndex, board, !isWhiteTurn);

					// Restore board to current branch setting
					// finds piece to restore for captured pieces
					switch((*branchIndex)->StoredMove.Capture)
					{					
					// tries NoPiece first as highest probability
					case NoPiece:
						{
							capturePiece = NULL;
							break;
						}
					case PawnPiece:
						{
							capturePiece = new Pawn(!isWhiteTurn);
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
					case KingPiece:
						{
							capturePiece = new King(!isWhiteTurn);
							break;
						}
					};

					// Restores board
					delete board[(*branchIndex)->StoredMove.From.Row][(*branchIndex)->StoredMove.From.Col];
					board[(*branchIndex)->StoredMove.From.Row][(*branchIndex)->StoredMove.From.Col] = board[(*branchIndex)->StoredMove.To.Row][(*branchIndex)->StoredMove.To.Col];
					board[(*branchIndex)->StoredMove.To.Row][(*branchIndex)->StoredMove.To.Col] = capturePiece;
				}
			}
		}

		// head of update move tree recursive depth
		void UpdateMoveTree(Piece * board[8][8])
		{
			UpdateMoveTreeRecursiveDepth(MoveTreeHead, board, IsWhite);
		}

		// rescores parent nodes as the minimum of their children
		// isWhiteTurn is the the player who makes the move to get to the children nodes
		// Upon exiting all scores are scored as the minimum of their Children's score.
		float EvaluateMoveTreeRecursiveDepth(Node * branchHead, Piece * board[8][8], bool isWhiteTurn)
		{
			// if there are Children nodes, go through them and evaluates their scores.
			if(!branchHead->Children.Empty())
			{
				// used to find the minimum score of children and then set that score as the branchHead's score
				float minScore = FLT_MAX;
				
				List<Node *>::Iterator branchIndex;
				for(branchIndex = branchHead->Children.Begin(); branchIndex != branchHead->Children.End() && branchIndex != NULL; ++branchIndex)
				{
					// recursive call of EvaulateMoveTreeRecursiveDepth, which returns the score of it.
					float tempScore = EvaluateMoveTreeRecursiveDepth(*branchIndex, board, !isWhiteTurn);

					if(tempScore < minScore)
					{
						minScore = tempScore;
					}
				}
				branchHead->Score = minScore;
			}
			return branchHead->Score;
		}

		// head of evaluate move tree recursive depth transversal
		void EvaluateMoveTree(Piece * board[8][8])
		{
			EvaluateMoveTreeRecursiveDepth(MoveTreeHead, board, IsWhite);
		}
		
		// removes branches from levels of opponents turn by breadth first transversal.
		// meant to be called after EvaulateMoveTree
		void TrimMoveTree()
		{
			// queue for breadth first transversal
			std::queue<Node *> nodeQueue;

			// queue that correspondes to nodeQueue that keeps track whether the node's children should be trimmed.
			std::queue<bool> thisMoveQueue = std::queue<bool>();

			// initialize the transversal starting at the head of the tree, whose children should not be trimmed
			nodeQueue.push(MoveTreeHead);
			thisMoveQueue.push(true);

			// variables for each step of the beadth transversal
			Node * branchHead = NULL;
			bool thisMove;

			while(!nodeQueue.empty())
			{
				// get current branch head
				branchHead = nodeQueue.front();
				nodeQueue.pop();

				// get corresponding bool for whether to trim its children
				thisMove = thisMoveQueue.front();
				thisMoveQueue.pop();

				// remove all NULL children as they are pointless
				branchHead->Children.Remove(NULL);

				// sorts the children, should have greatest scores at the front.
				branchHead->Children.Sort( &Node::NodeCompare );

				// remove nodes of opponent's move, by removing node of least harm to this player
				if(!thisMove)
				{
					// removes children until at the designated size
					while(branchHead->Children.GetSize() > TrimThreshold)
					{
						delete *(branchHead->Children.Begin());
						branchHead->Children.PopFront();
					}
				}

				/*
					trim computer player's moves, but by the middle, so they still have best moves 
					and are keeping track of opponents best moves
				*/
				else if((int)branchHead->Children.GetSize() > TrimThreshold)
				{
					List<Node *>::Iterator locOfRemoval = branchHead->Children.Begin();

					// move locOfRemoval to the correct position
					for(int loc_counter = 0; loc_counter < TrimThreshold/2; loc_counter++)
					{
						++locOfRemoval;
					}

					// remove child until designated size
					while((int)branchHead->Children.GetSize() > TrimThreshold)
					{
						delete *locOfRemoval;
						locOfRemoval = branchHead->Children.Erase(locOfRemoval);
					}

					
				}


				// adds the remaining children to the breadth first tansversal to be trimmed
				List<Node *>::Iterator branchIndex = branchHead->Children.Begin();
				for(; branchIndex != branchHead->Children.End() && branchIndex != NULL; ++branchIndex)
				{
					nodeQueue.push(*branchIndex);

					// only every other level is trimmed
					thisMoveQueue.push(!thisMove);
				}
			}
		}


		// creates a new Move Tree based off the current board
		void GenerateMoveTree(Piece * board[8][8])
		{
			// create new head of Move Tree
			if(MoveTreeHead != NULL)
				delete MoveTreeHead;
			MoveTreeHead = new Node();

			// adds two layers to Move Tree for each difficulty level
			for(int i = 0; i < DifficultyLevel; i++)
			{
				// computer move
				UpdateMoveTree(board);
				// opponent move
				UpdateMoveTree(board);
				EvaluateMoveTree(board);
				TrimMoveTree();
			}
		}

	public:

		/* 
			Constructer; 
				sets IsWhite to the passed values.
				sets MoveTreeHead to a default Node
				sets DifficultyLevel to a value between 1 and 3, by picking the value closest the passed in value.
		*/
		Computer_Old(int difficultyLevel, bool isWhite)
		{
			if(difficultyLevel < 1)
				difficultyLevel = 1;
			else if(difficultyLevel > 4)
				difficultyLevel = 4;
			DifficultyLevel = difficultyLevel;
			IsWhite = isWhite;
			MoveTreeHead = new Node();
			TrimThreshold = 5;
			srand((unsigned int)time(NULL));
		}

		~Computer_Old()
		{
			delete MoveTreeHead;
		}

		// decides on the move the computer player makes
		Move MakeMove(Piece * board[8][8], Move lastMove)
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
								break;
							}
						case KnightPiece:
							{
								computerBoard[row][col] = new Knight(board[row][col]->IsWhitePiece());
								break;
							}
						case BishopPiece:
							{
								computerBoard[row][col] = new Bishop(board[row][col]->IsWhitePiece());
								break;
							}
						case RookPiece:
							{
								computerBoard[row][col] = new Rook(board[row][col]->IsWhitePiece());
								break;
							}
						case QueenPiece:
							{
								computerBoard[row][col] = new Queen(board[row][col]->IsWhitePiece());
								break;
							}
						case KingPiece:
							{
								computerBoard[row][col] = new King(board[row][col]->IsWhitePiece());
								break;
							}
						}
					}
				}
			}

			// check whether a Move Tree needs to be generated
			bool isMoveInTree = false;
			// search for opponent's move in tree
			List<Node *>::Iterator moveTreeIndex = MoveTreeHead->Children.Begin();
			for(; moveTreeIndex != MoveTreeHead->Children.End(); ++moveTreeIndex)
			{
				if((*moveTreeIndex) != NULL)
				{
					// found opponent's move, updates MoveTree to that node
					//Move move = (*moveTreeIndex)->StoredMove;
					if((*moveTreeIndex)->StoredMove == lastMove)
					{
						ChangeHead(*moveTreeIndex);
						isMoveInTree = true;
						break;
					}
				}
			}

			// generates a new Move Tree as the old one did not have the move made
			if(!isMoveInTree)
			{
				GenerateMoveTree(computerBoard);
			}
			// the tree is two layers short, so add two layers
			else
			{
				// another layer of this player's moves
				UpdateMoveTree(computerBoard);

				// another layer of opponent's moves
				UpdateMoveTree(computerBoard);

				// set scores and remove extra limbs
				EvaluateMoveTree(computerBoard);
				TrimMoveTree();
			}
			// sorts the first layer of the tree to find the best moves for this turn
			MoveTreeHead->Children.Sort( &Node::NodeCompare );

			// stores all moves of equal top score
			std::vector<List<Node *>::Iterator> topMoves;

			// top score for current turn
			float maxScore;

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
						 topMoves.push_back(moveTreeIndex);
					 }
					 else
					 {
						 continue;
					 }
				 }
			 }
			 //If there are moves to select from then return one
			 if(topMoves.size() > 0)
			 {
				//randomly choose move
				myMoveIndex = rand() % topMoves.size();

				 //reflect move on move tree
				Node * topmove = *topMoves[myMoveIndex];
				ChangeHead(topmove);

				// return the chosen move
				Move StoredMove = MoveTreeHead->StoredMove;
				return StoredMove;
			 }
			 //Else return a blank move because checkmate, stalemate or a bug has occurred (There should always be moves)
			 else 
			 {
				 Move move = Move();
				 return move;
			 }
		}

		PieceType ConvertPawn()
		{
			return QueenPiece;
		}
	};
}
