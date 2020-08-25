#include <string.h>
#include <jni.h>
#include <iostream>
#include <sstream>
#include "GameMaster.h"
namespace ChessMob
{
	extern "C"
	{
		GameMaster* master = NULL;
		JNIEXPORT void JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_InitGame(JNIEnv* env, jobject obj, jint whitePlayerType, jint whitePieceType, jint blackPlayerType, jint blackPieceType, jint Difficulty)
		{
			master = new GameMaster((PlayerType)whitePlayerType, (PieceType)whitePieceType, (PlayerType)blackPlayerType, (PieceType)blackPieceType, Difficulty);
		}

		JNIEXPORT void JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_ResumeGame(JNIEnv* env, jobject obj, jint whitePlayerType, jint blackPlayerType, jint Difficulty, jintArray pieces, jintArray moves, jboolean isWhiteTurn)
		{
			std::deque<Move> history;
			Piece* board[8][8];
			PlayerType whitePlayer = (whitePlayerType == 0) ? HumanPlayer : ComputerPlayer;
			PlayerType blackPlayer = (blackPlayerType == 0) ? HumanPlayer : ComputerPlayer;

			jsize len1 = env->GetArrayLength(moves);
			jboolean IsCopy;
			jint* value = env->GetIntArrayElements(moves, &IsCopy);
			for(int i = 0; i < len1; i += 6)
			{
				Move move(Coord(value[i], value[i + 1]), Coord(value[i + 2], value[i + 3]), (PieceType)value[i + 4], value[i + 5] == 1);
				history.push_back(move);
			}

			value = env->GetIntArrayElements(pieces, &IsCopy);
			int count = 0;
			for(int i = 0; i < 8; i++)
			{
				for(int k = 0; k < 8; k++)
				{
					jint piece = value[count++];
					switch (abs(piece))
					{
					case 0: //None
						board[i][k] = NULL;
						break;
					case 1: //King
						board[i][k] = new King((piece > 0));
						break;
					case 2: //Queen
						board[i][k] = new Queen((piece > 0));
						break;
					case 3: // Rook
						board[i][k] = new Rook((piece > 0));
						break;
					case 4: //Bishop
						board[i][k] = new Bishop((piece > 0));
						break;
					case 5: //Knight
						board[i][k] = new Knight((piece > 0));
						break;
					case 6: //Pawn
						board[i][k] = new Pawn((piece > 0));
						break;
					default:
						break;
					}
				}
			}
			master = new GameMaster((PlayerType)whitePlayerType, (PlayerType)blackPlayerType, board, history, isWhiteTurn, Difficulty);
		}

		JNIEXPORT jboolean JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_MakeMove(JNIEnv* env, jobject obj, jint originalx, jint originaly, jint newx, jint newy )
		{
			Coord original(originalx, originaly);
			Coord newone(newx, newy);
			Move move(original, newone, NoPiece);
			return master->MakeMove(move);
		}

		JNIEXPORT void JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_MakeComputerMove(JNIEnv* env, jobject obj, jint originalx, jint originaly, jint newx, jint newy )
		{
			return master->MakeComputerMove();
		}

		JNIEXPORT void JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_Undo(JNIEnv* env, jobject obj)
		{
			master->UndoMove();
		}

		JNIEXPORT jint JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_GetPiece(JNIEnv* env, jobject obj, jint x, jint y)
		{
			Coord c(x, y);
			return master->GetPiece(c);
		}

		JNIEXPORT void JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_ConvertPawn(JNIEnv* env, jobject obj, jint row, jint column, jint replacementType)
		{
			Coord c(row == 1337 ? 0 : row, column == 1337 ? 0 : column);
			master->ConvertPawn(c, (PieceType)replacementType);
		}

		JNIEXPORT jstring JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_GetHistory(JNIEnv* env, jobject obj)
		{
			std::string content;
			std::ostringstream ostr;
			std::deque<Move> moves = master->GetHistory();

			for(std::deque<Move>::iterator it = moves.begin(); it != moves.end(); it++)
			{
				Move move = *it;
				ostr << (int)move.From.Row;
				ostr.put(':');

				ostr << (int)move.From.Col;
				ostr.put('~');
								
				ostr << (int)move.To.Row;
				ostr.put(':');
				
				ostr << (int)move.To.Col;
				ostr.put('~');
				
				ostr << move.Capture;
				ostr.put('~');
				
				ostr << move.ChangedType ? 1 : 0;				
				ostr.put('&');
			}
			content = ostr.str();
			char * st = new char[content.length()+1];
			strcpy(st, content.c_str());
			return env->NewStringUTF(st);
		}

		JNIEXPORT jboolean JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_CheckCheck(JNIEnv* env, jobject obj)
		{
			return master->IsInCheck();
		}

		JNIEXPORT jboolean JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_CheckCheckmate(JNIEnv* env, jobject obj)
		{
			return master->IsInCheckMate();
		}
		JNIEXPORT jboolean JNICALL Java_com_chessmob_chessmobandroid_GameNDKFunctions_CheckStalemate(JNIEnv* env, jobject obj)
		{
			return master->IsStaleMate();
		}
	};
}
