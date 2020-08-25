#include <iostream>
#include <Windows.h>
#include "..\ChessMob_BackEnd\GameMaster.h"
#include "..\ChessMob_BackEnd\Computer_old.h"

using namespace ChessMob;
using namespace std;

int comp_main()
{
	Piece* board[8][8] = {NULL};
	bool isWhiteTurn = true;
	int count = 1;
	GameMaster game(HumanPlayer, RookPiece, HumanPlayer, QueenPiece, 3);
	Computer_Old ai_1(3, true);
	Computer ai_2(2, false);
	Move lastMove;
	HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );	

	do
	{
		if(isWhiteTurn)
		{
			cout << "\nPlayer Blue's Turn \n\n";
		}
		else
		{
			cout << "\nPlayer Red's Turn \n\n";
		}

		SetConsoleTextAttribute(hConsole, 0x08);

		cout << "col:  ";
		for(int col = 0; col < 8; col++)
		{
			cout << " " << col;
		}

		for(int row = 0; row < 8; row++)
		{

			cout << endl;
			cout << "row " << row << ":";
			for(int col = 0; col < 8; col++)
			{
				int piece = game.GetPiece(Coord(row, col));
				bool isWhite = true;

				if(col+row & 1)
				{
					SetConsoleTextAttribute(hConsole, 0x09);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 0x89);
				}

				if(piece < 0)
				{
					piece = -1*piece;
					isWhite = false;
					if(row+col & 1)
					{
						SetConsoleTextAttribute(hConsole, 0x0C);
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 0x8C);
					}
				}

				if(board[row][col])
				{
					delete board[row][col];
				}
				switch(piece)
				{
				case NoPiece:
					{
						board[row][col] = NULL;
						if(row+col & 1)
						{
							SetConsoleTextAttribute(hConsole, 0x08);
						}
						else
						{
							SetConsoleTextAttribute(hConsole, 0x80);
						}
						cout << "  ";
						break;
					}
				case KingPiece:
					{
						board[row][col] = new King(isWhite);
						cout << " K";
						break;
					}
				case QueenPiece:
					{
						board[row][col] = new Queen(isWhite);
						cout << " Q";
						break;
					}
				case RookPiece:
					{
						board[row][col] = new Rook(isWhite);
						cout << " R";
						break;
					}
				case BishopPiece:
					{
						board[row][col] = new Bishop(isWhite);
						cout << " B";
						break;
					}
				case KnightPiece:
					{
						board[row][col] = new Knight(isWhite);
						cout << " N";
						break;
					}
				case PawnPiece:
					{
						board[row][col] = new Pawn(isWhite);
						cout << " P";
						break;
					}
				default:
					{
						cout << " !";
						break;
					}
				}
			}
			SetConsoleTextAttribute( hConsole, 0x08);
		}

		if(isWhiteTurn)
		{
			Move playerMove = ai_1.MakeMove(board, lastMove);
			if(!game.MakeMove(playerMove))
			{
				cout << "\n\n**********************Illegal Move***************\n\n";
				cout << (int)playerMove.From.Row << ":" << (int)playerMove.From.Col << ":" << (int)playerMove.To.Row << ":" << (int)playerMove.To.Col << endl << endl;
			}
			delete board[playerMove.To.Row][playerMove.To.Col];
			board[playerMove.To.Row][playerMove.To.Col] = board[playerMove.From.Row][playerMove.From.Col];
			board[playerMove.From.Row][playerMove.From.Col] = NULL;
			for(int col = 0; col < 8; col++)
			{
				if(board[0][col] && board[0][col]->IsWhitePiece() && board[0][col]->GetType() == PawnPiece)
				{
					game.ConvertPawn(Coord(0,col),QueenPiece);
				}
			}
		}
		else
		{
			Move playerMove = ai_2.MakeMove(board);
			lastMove = playerMove;
			if(!game.MakeMove(playerMove))
			{
				cout << "\n\n**********************Illegal Move***************\n\n";
				cout << (int)playerMove.From.Row << ":" << (int)playerMove.From.Col << ":" << (int)playerMove.To.Row << ":" << (int)playerMove.To.Col << endl << endl;
			}
			delete board[playerMove.To.Row][playerMove.To.Col];
			board[playerMove.To.Row][playerMove.To.Col] = board[playerMove.From.Row][playerMove.From.Col];
			board[playerMove.From.Row][playerMove.From.Col] = NULL;
			for(int col = 0; col < 8; col++)
			{
				if(board[7][col] && !board[7][col]->IsWhitePiece() && board[7][col]->GetType() == PawnPiece)
				{
					game.ConvertPawn(Coord(7,col),QueenPiece);
				}
			}
		}

		isWhiteTurn = !isWhiteTurn;

	}while(!game.IsInCheckMate());

	SetConsoleTextAttribute(hConsole, 0x08);

	cout << "\n\ncol:  ";
	for(int col = 0; col < 8; col++)
	{
		cout << " " << col;
	}

	for(int row = 0; row < 8; row++)
	{

		cout << endl;
		cout << "row " << row << ":";
		for(int col = 0; col < 8; col++)
		{
			int piece = game.GetPiece(Coord(row, col));
			bool isWhite = true;

			if(col+row & 1)
			{
				SetConsoleTextAttribute(hConsole, 0x09);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 0x89);
			}

			if(piece < 0)
			{
				piece = -1*piece;
				isWhite = false;
				if(row+col & 1)
				{
					SetConsoleTextAttribute(hConsole, 0x0C);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 0x8C);
				}
			}

			if(board[row][col])
			{
				delete board[row][col];
			}
			switch(piece)
			{
			case NoPiece:
				{
					board[row][col] = NULL;
					if(row+col & 1)
					{
						SetConsoleTextAttribute(hConsole, 0x08);
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 0x80);
					}
					cout << "  ";
					break;
				}
			case KingPiece:
				{
					board[row][col] = new King(isWhite);
					cout << " K";
					break;
				}
			case QueenPiece:
				{
					board[row][col] = new Queen(isWhite);
					cout << " Q";
					break;
				}
			case RookPiece:
				{
					board[row][col] = new Rook(isWhite);
					cout << " R";
					break;
				}
			case BishopPiece:
				{
					board[row][col] = new Bishop(isWhite);
					cout << " B";
					break;
				}
			case KnightPiece:
				{
					board[row][col] = new Knight(isWhite);
					cout << " N";
					break;
				}
			case PawnPiece:
				{
					board[row][col] = new Pawn(isWhite);
					cout << " P";
					break;
				}
			default:
				{
					cout << " !";
					break;
				}
			}
		}
		SetConsoleTextAttribute( hConsole, 0x08);
	}

	if(isWhiteTurn)
	{
		cout << "\nRed Won" << endl;
	}
	else
	{
		cout << "\nBlue Won" << endl;
	}

	return 0;
}

int new_comp_main()
{
	Piece* board[8][8] = {NULL};
	bool isWhiteTurn = true;
	int count = 1;
	GameMaster game(HumanPlayer, QueenPiece, HumanPlayer, QueenPiece, 3);
	Computer ai_1(3, true);
	Computer ai_2(3, false);
	Move lastMove;
	HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );	

	do
	{
		if(isWhiteTurn)
		{
			cout << "\nPlayer Blue's Turn \n\n";
		}
		else
		{
			cout << "\nPlayer Red's Turn \n\n";
		}

		SetConsoleTextAttribute(hConsole, 0x08);

		cout << "col:  ";
		for(int col = 0; col < 8; col++)
		{
			cout << " " << col;
		}

		for(int row = 0; row < 8; row++)
		{

			cout << endl;
			cout << "row " << row << ":";
			for(int col = 0; col < 8; col++)
			{
				int piece = game.GetPiece(Coord(row, col));
				bool isWhite = true;

				if(col+row & 1)
				{
					SetConsoleTextAttribute(hConsole, 0x09);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 0x89);
				}

				if(piece < 0)
				{
					piece = -1*piece;
					isWhite = false;
					if(row+col & 1)
					{
						SetConsoleTextAttribute(hConsole, 0x0C);
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 0x8C);
					}
				}

				if(board[row][col])
				{
					delete board[row][col];
				}
				switch(piece)
				{
				case NoPiece:
					{
						board[row][col] = NULL;
						if(row+col & 1)
						{
							SetConsoleTextAttribute(hConsole, 0x08);
						}
						else
						{
							SetConsoleTextAttribute(hConsole, 0x80);
						}
						cout << "  ";
						break;
					}
				case KingPiece:
					{
						board[row][col] = new King(isWhite);
						cout << " K";
						break;
					}
				case QueenPiece:
					{
						board[row][col] = new Queen(isWhite);
						cout << " Q";
						break;
					}
				case RookPiece:
					{
						board[row][col] = new Rook(isWhite);
						cout << " R";
						break;
					}
				case BishopPiece:
					{
						board[row][col] = new Bishop(isWhite);
						cout << " B";
						break;
					}
				case KnightPiece:
					{
						board[row][col] = new Knight(isWhite);
						cout << " N";
						break;
					}
				case PawnPiece:
					{
						board[row][col] = new Pawn(isWhite);
						cout << " P";
						break;
					}
				default:
					{
						cout << " !";
						break;
					}
				}
			}
			SetConsoleTextAttribute( hConsole, 0x08);
		}

		if(isWhiteTurn)
		{
			Move playerMove = ai_1.MakeMove(board);
			if(!game.MakeMove(playerMove))
			{
				cout << "\n\n**********************Illegal Move***************\n\n";
				cout << (int)playerMove.From.Row << ":" << (int)playerMove.From.Col << ":" << (int)playerMove.To.Row << ":" << (int)playerMove.To.Col << endl << endl;
			}
			delete board[playerMove.To.Row][playerMove.To.Col];
			board[playerMove.To.Row][playerMove.To.Col] = board[playerMove.From.Row][playerMove.From.Col];
			board[playerMove.From.Row][playerMove.From.Col] = NULL;
			for(int col = 0; col < 8; col++)
			{
				if(board[0][col] && board[0][col]->IsWhitePiece() && board[0][col]->GetType() == PawnPiece)
				{
					game.ConvertPawn(Coord(0,col),QueenPiece);
				}
			}
		}
		else
		{
			Move playerMove = ai_2.MakeMove(board);
			if(!game.MakeMove(playerMove))
			{
				cout << "\n\n**********************Illegal Move***************\n\n";
				cout << (int)playerMove.From.Row << ":" << (int)playerMove.From.Col << ":" << (int)playerMove.To.Row << ":" << (int)playerMove.To.Col << endl << endl;
			}
			delete board[playerMove.To.Row][playerMove.To.Col];
			board[playerMove.To.Row][playerMove.To.Col] = board[playerMove.From.Row][playerMove.From.Col];
			board[playerMove.From.Row][playerMove.From.Col] = NULL;
			for(int col = 0; col < 8; col++)
			{
				if(board[7][col] && !board[7][col]->IsWhitePiece() && board[7][col]->GetType() == PawnPiece)
				{
					game.ConvertPawn(Coord(7,col),QueenPiece);
				}
			}
		}

		isWhiteTurn = !isWhiteTurn;

	}while(!game.IsInCheckMate());

	SetConsoleTextAttribute(hConsole, 0x08);

	cout << "\n\ncol:  ";
	for(int col = 0; col < 8; col++)
	{
		cout << " " << col;
	}

	for(int row = 0; row < 8; row++)
	{

		cout << endl;
		cout << "row " << row << ":";
		for(int col = 0; col < 8; col++)
		{
			int piece = game.GetPiece(Coord(row, col));
			bool isWhite = true;

			if(col+row & 1)
			{
				SetConsoleTextAttribute(hConsole, 0x09);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 0x89);
			}

			if(piece < 0)
			{
				piece = -1*piece;
				isWhite = false;
				if(row+col & 1)
				{
					SetConsoleTextAttribute(hConsole, 0x0C);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 0x8C);
				}
			}

			if(board[row][col])
			{
				delete board[row][col];
			}
			switch(piece)
			{
			case NoPiece:
				{
					board[row][col] = NULL;
					if(row+col & 1)
					{
						SetConsoleTextAttribute(hConsole, 0x08);
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 0x80);
					}
					cout << "  ";
					break;
				}
			case KingPiece:
				{
					board[row][col] = new King(isWhite);
					cout << " K";
					break;
				}
			case QueenPiece:
				{
					board[row][col] = new Queen(isWhite);
					cout << " Q";
					break;
				}
			case RookPiece:
				{
					board[row][col] = new Rook(isWhite);
					cout << " R";
					break;
				}
			case BishopPiece:
				{
					board[row][col] = new Bishop(isWhite);
					cout << " B";
					break;
				}
			case KnightPiece:
				{
					board[row][col] = new Knight(isWhite);
					cout << " N";
					break;
				}
			case PawnPiece:
				{
					board[row][col] = new Pawn(isWhite);
					cout << " P";
					break;
				}
			default:
				{
					cout << " !";
					break;
				}
			}
		}
		SetConsoleTextAttribute( hConsole, 0x08);
	}

	if(isWhiteTurn)
	{
		cout << "\nRed Won" << endl;
	}
	else
	{
		cout << "\nBlue Won" << endl;
	}

	return 0;
}

int hum_comp_main()
{
	bool isWhiteTurn = true;
	int count = 1;
	GameMaster game(HumanPlayer, QueenPiece, ComputerPlayer, QueenPiece, 3);
	HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );	

	do
	{
		system("cls");
		if(isWhiteTurn)
		{
			cout << "Player Blue's Turn \n\n";
		}
		else
		{
			cout << "Player Red's Turn \n\n";
		}

		SetConsoleTextAttribute(hConsole, 0x08);

		cout << "col:  ";
		for(int col = 0; col < 8; col++)
		{
			cout << " " << col;
		}

		for(int row = 0; row < 8; row++)
		{

			cout << endl;
			cout << "row " << row << ":";
			for(int col = 0; col < 8; col++)
			{
				int piece = game.GetPiece(Coord(row, col));
				bool isWhite = true;

				if(col+row & 1)
				{
					SetConsoleTextAttribute(hConsole, 0x09);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 0x89);
				}

				if(piece < 0)
				{
					piece = -1*piece;
					isWhite = false;
					if(row+col & 1)
					{
						SetConsoleTextAttribute(hConsole, 0x0C);
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 0x8C);
					}
				}

				switch(piece)
				{
				case NoPiece:
					{
						if(row+col & 1)
						{
							SetConsoleTextAttribute(hConsole, 0x08);
						}
						else
						{
							SetConsoleTextAttribute(hConsole, 0x80);
						}
						cout << "  ";
						break;
					}
				case KingPiece:
					{
						cout << " K";
						break;
					}
				case QueenPiece:
					{
						cout << " Q";
						break;
					}
				case RookPiece:
					{
						cout << " R";
						break;
					}
				case BishopPiece:
					{
						cout << " B";
						break;
					}
				case KnightPiece:
					{
						cout << " N";
						break;
					}
				case PawnPiece:
					{
						cout << " P";
						break;
					}
				default:
					{
						cout << " !";
						break;
					}
				}
			}
			SetConsoleTextAttribute( hConsole, 0x08);
		}

		int rowFromMove = 0;
		int colFromMove = 0;
		int rowToMove = 0;
		int colToMove = 0;

		do
		{
			cout << "\n\nRow From? ";
			cin >> rowFromMove;
			cout << "Col From? ";			
			cin >> colFromMove;
			cout << "Row To? ";			
			cin >> rowToMove;
			cout << "Col To? ";			
			cin >> colToMove;
		}while(rowToMove > 7 || rowFromMove > 7 || colToMove > 7 || colFromMove > 7 || !game.MakeMove(Move(Coord(rowFromMove, colFromMove), Coord(rowToMove, colToMove),NoPiece)));

		isWhiteTurn = !isWhiteTurn;


		if(!game.IsInCheckMate())
		{
			game.MakeComputerMove();
			isWhiteTurn = !isWhiteTurn;
		}
	}while(!game.IsInCheckMate());

	if(isWhiteTurn)
	{
		cout << "Red Won" << endl;
	}
	else
	{
		cout << "Blue Won" << endl;
	}

	return 0;
}

int human_2_main()
{
	bool isWhiteTurn = true;
	int count = 1;
	GameMaster game(HumanPlayer, BishopPiece, HumanPlayer, QueenPiece, 3);
	HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );	

	do
	{
		system("cls");
		if(isWhiteTurn)
		{
			cout << "Player Blue's Turn \n\n";
		}
		else
		{
			cout << "Player Red's Turn \n\n";
		}

		SetConsoleTextAttribute(hConsole, 0x08);

		cout << "col:  ";
		for(int col = 0; col < 8; col++)
		{
			cout << " " << col;
		}

		for(int row = 0; row < 8; row++)
		{

			cout << endl;
			cout << "row " << row << ":";
			for(int col = 0; col < 8; col++)
			{
				int piece = game.GetPiece(Coord(row, col));
				bool isWhite = true;

				if(col+row & 1)
				{
					SetConsoleTextAttribute(hConsole, 0x09);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 0x89);
				}

				if(piece < 0)
				{
					piece = -1*piece;
					isWhite = false;
					if(row+col & 1)
					{
						SetConsoleTextAttribute(hConsole, 0x0C);
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 0x8C);
					}
				}

				switch(piece)
				{
				case NoPiece:
					{
						if(row+col & 1)
						{
							SetConsoleTextAttribute(hConsole, 0x08);
						}
						else
						{
							SetConsoleTextAttribute(hConsole, 0x80);
						}
						cout << "  ";
						break;
					}
				case KingPiece:
					{
						cout << " K";
						break;
					}
				case QueenPiece:
					{
						cout << " Q";
						break;
					}
				case RookPiece:
					{
						cout << " R";
						break;
					}
				case BishopPiece:
					{
						cout << " B";
						break;
					}
				case KnightPiece:
					{
						cout << " N";
						break;
					}
				case PawnPiece:
					{
						cout << " P";
						break;
					}
				default:
					{
						cout << " !";
						break;
					}
				}
			}
			SetConsoleTextAttribute( hConsole, 0x08);
		}

		int rowFromMove = 0;
		int colFromMove = 0;
		int rowToMove = 0;
		int colToMove = 0;

		do
		{
			cout << "\n\nRow From? ";
			cin >> rowFromMove;
			cout << "Col From? ";			
			cin >> colFromMove;
			cout << "Row To? ";			
			cin >> rowToMove;
			cout << "Col To? ";			
			cin >> colToMove;
		}while(rowToMove > 7 || rowFromMove > 7 || colToMove > 7 || colFromMove > 7 || !game.MakeMove(Move(Coord(rowFromMove, colFromMove), Coord(rowToMove, colToMove),NoPiece)));

		isWhiteTurn = !isWhiteTurn;


		/*if(!game.IsInCheckMate())
		{
		game.MakeComputerMove();
		isWhiteTurn = !isWhiteTurn;
		}*/
	}while(!game.IsInCheckMate());

	if(isWhiteTurn)
	{
		cout << "Red Won" << endl;
	}
	else
	{
		cout << "Blue Won" << endl;
	}

	return 0;
}

int oldMain()
{
	Piece * Board[8][8];
	bool isWhite = true;

	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			Board[i][j] = NULL;
		}
	};

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

	Computer aiPlayer(2,true);

	Move aiMove = aiPlayer.MakeMove(Board);

	return 0;
}

int main()
{
	//comp_main();
	new_comp_main();
	//hum_comp_main();
}