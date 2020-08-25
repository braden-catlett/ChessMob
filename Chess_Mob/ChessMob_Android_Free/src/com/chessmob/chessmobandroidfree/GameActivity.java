package com.chessmob.chessmobandroidfree;

import java.util.ArrayList;

import com.chessmob.chessmob_android_free.R;

import de.keyboardsurfer.android.widget.crouton.Crouton;
import de.keyboardsurfer.android.widget.crouton.Style;

import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager.LayoutParams;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class GameActivity extends Activity 
{
	private Piece[][] BoardPieces = new Piece[8][8];
	private int ConvertedPieceIndex = 0;
	private ImageView selectedPiece = null;
	private String opponentType = "";
	private String difficulty = "";
	private String playerColor = "";
	private String playerSet = "";
	private String opponentSet = "";
	private boolean loadGameFromFile = false;
	private boolean IsWhiteTurn = true;
	private boolean gameOver = false;
	private ArrayList<ImageView> images = new ArrayList<ImageView>();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_game);
		
		Bundle bundle = this.getIntent().getExtras();
		Init_Game(bundle);
		Init_MenuButton();
		Init_UndoButton();
	}

	
	
	@Override
	protected void onResume()
	{
		super.onResume();
		com.facebook.AppEventsLogger.activateApp(getApplicationContext(), getString(R.string.app_id));
	}



	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.game, menu);
		Init_BoardPositions();
		//If the opponent is a computer and we are not resuming a game in the middle of a convert then call computer make move
		if(opponentType.contains("Computer") && !MainActivity.preferences.getBoolean("DisplayConvertPiece", false))
		{
			//And its the computers turn
			if(IsWhiteTurn && playerColor.contains("Black") || !IsWhiteTurn && playerColor.contains("White") )
			{
				MakeComputerMove();
				//Display messages
	            DisplayGameStatusMessages(!IsWhiteTurn);
			}
		}
		DrawBoard();
		return true;
	}
	
	
	@Override
	public void onBackPressed()
	{
		ShowMenu(false);
	}

	
	public void Init_MenuButton() 
	{
		Button menu = (Button) findViewById(R.id.Menu);
		menu.setOnClickListener(new OnClickListener() 
		{
			@Override
			public void onClick(View v)
			{
				ShowMenu(false);
			}
		});
	}
	
	
	public void Init_UndoButton()
	{
		Button Undo = (Button)findViewById(R.id.Undo);
		Undo.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				GameNDKFunctions.Undo();
				if(opponentType.compareTo("Player") == 0)
					IsWhiteTurn = !IsWhiteTurn;
				DrawBoard();
			}
		});
	}

	
	public void Init_Game(Bundle bundle)
	{
		opponentType = bundle.getString("player");
		difficulty = bundle.getString("difficulty");
		playerColor = bundle.getString("color");
		playerSet = bundle.getString("yourset");
		opponentSet = bundle.getString("opposet");
		loadGameFromFile = bundle.getBoolean("load");
		if(loadGameFromFile)
		{
			LoadGame();
		}
		else
		{
			int whitePlayerType = (playerColor.contains("White")) ? PlayerType.Human.ordinal() : (opponentType.contains("Computer")) ? PlayerType.Computer.ordinal() : PlayerType.Human.ordinal();
			int blackPlayerType = (playerColor.contains("Black")) ? PlayerType.Human.ordinal() : (opponentType.contains("Computer")) ? PlayerType.Computer.ordinal() : PlayerType.Human.ordinal();
			
			int whitePieceType = (playerColor.contains("White")) ? GetPieceSetType(playerSet) : GetPieceSetType(opponentSet);
			int blackPieceType = (playerColor.contains("Black")) ? GetPieceSetType(playerSet) : GetPieceSetType(opponentSet);
			
			int difficultyType = ((difficulty.contains("Beginner")) 
								 ? DifficultyOptions.Beginner : (difficulty.contains("Amateur")) 
								 ? DifficultyOptions.Amateur : DifficultyOptions.Professional).ordinal();
			//Difficulty is incremented to match brayen's values
			GameNDKFunctions.InitGame(whitePlayerType, whitePieceType, blackPlayerType, blackPieceType, difficultyType + 1);
		}
		if (playerColor.contains("White")) 
		{
			HighlightPlayerAsWhite(GetPieceSetType(playerSet), GetPieceSetType(opponentSet));
		} 
		else if (playerColor.contains("Black")) 
		{
			HighlightOpponentAsWhite(GetPieceSetType(playerSet), GetPieceSetType(opponentSet));
		}		
	}


	private void LoadGame() 
	{
	    int whitePlayerType = MainActivity.preferences.getInt("WhitePlayerType", 0);
	    int blackPlayerType = MainActivity.preferences.getInt("BlackPlayerType", 0);
	    int difficulty = MainActivity.preferences.getInt("DifficultyType", 0);
	    this.difficulty = difficulty == 0 ? "Beginner" : difficulty == 1 ? "Amateur" : difficulty == 2 ? "Professional" : "";
	    String History = MainActivity.preferences.getString("History", "");
	    String[] board = MainActivity.preferences.getString("Board", "").split("\n");

		opponentType = (whitePlayerType == PlayerType.Computer.ordinal() || blackPlayerType == PlayerType.Computer.ordinal()) ? "Computer" : "Player";
		playerColor = whitePlayerType == 0 ? "White" : blackPlayerType == 0 ? "Black" : "White";
	    this.IsWhiteTurn = MainActivity.preferences.getBoolean("Turn", (opponentType.contains("Computer") && playerColor.contains("White")) ? true : false);
	    playerSet = MainActivity.preferences.getString("YourSet", "");
	    opponentSet = MainActivity.preferences.getString("OpponentSet", "");
		
		int PieceIndex = 0;
		int[] pieces = new int[64];
		int count = 0;
		for(int i = 0; i < BoardPieces.length; i++)
		{
			String[] data = board[PieceIndex++].split(":");
			for(int k = 0; k < data.length; k++)
			{
				int type = Integer.parseInt(data[k]);
				boolean iswhite = (type > 0);
				pieces[count++] = type;
				BoardPieces[i][k] = new Piece(type, iswhite);
			}
		}
		
		int values[] = new int[]{};
		if(History != null && History.length() > 5)
		{
			String[] moves = History.split("&");
			values = new int[moves.length * 6];
			PieceIndex = 0;
			for(int i = 0; i < moves.length; i++)
			{
				//move.From.Row + ":" + move.From.Col + "~" + move.To.Row + ":" + move.To.Col + "~" + move.Capture + "~" + move.ChangedType + "&" 
				String[] localHistory = moves[i].split("~");
				if(localHistory.length == 0)
					continue;
				String[] From = localHistory[0].split(":");
				String[] To = localHistory[1].split(":");
				if(From == null || To == null)
					continue;
				values[PieceIndex++] = Integer.parseInt(From[0]);
				values[PieceIndex++] = Integer.parseInt(From[1]);
				values[PieceIndex++] = Integer.parseInt(To[0]);
				values[PieceIndex++] = Integer.parseInt(To[1]);
				values[PieceIndex++] = Integer.parseInt(localHistory[2]);
				values[PieceIndex++] = Integer.parseInt(localHistory[3]);
			}
		}
		//Difficulty is incremented to match brayen's values
		GameNDKFunctions.ResumeGame(whitePlayerType, blackPlayerType, difficulty + 1, pieces, values, IsWhiteTurn);
		
		if(MainActivity.preferences.getBoolean("DisplayConvertPiece", false))
		{
			int row = MainActivity.preferences.getInt("ConvertPieceRow", -1);
			int column = MainActivity.preferences.getInt("ConvertPieceColumn", -1);
			DisplayConvertPawnMenu(row, column, !IsWhiteTurn);
		}
		else
		{
			if(IsWhiteTurn && whitePlayerType == PlayerType.Computer.ordinal() && playerColor.contains("White")  || !IsWhiteTurn && blackPlayerType == PlayerType.Computer.ordinal() && playerColor.contains("Black"))
			{
				MakeComputerMove();
				//Display messages
	            DisplayGameStatusMessages(!IsWhiteTurn);
				DrawBoard();
			}
		}
	}
	
	
	public void Init_BoardPositions() 
	{
		ImageView board = (ImageView) findViewById(R.id.Board);

		float height = board.getHeight();
		float width = board.getWidth();
		float delta = (width / height) / 2.0f;
		int left = (int) (board.getLeft() * delta);
		int top = (int) (board.getTop() * delta);
		int div_height = (int) (height / 8);
		int div_width = (int) (width / 8);
		for (int i = 0; i < 8; i++)
		{
			for (int k = 0; k < 8; k++)
			{
				BoardPieces[i][k] = new Piece(0, true);
				BoardPieces[i][k].Position = new Coord(left + (div_width * k), top + (div_height * i));
			}
		}
	}

	
	private void ReadBoard()
	{
		for(int i = 0; i < BoardPieces.length; i++)
		{
			for(int k = 0; k < BoardPieces.length; k++)
			{
				int piece = GameNDKFunctions.GetPiece(i, k);
				BoardPieces[i][k].IsWhite = (piece > 0);
				BoardPieces[i][k].Type = Math.abs(piece);
			}
		}
	}
	
	
	private void SaveBoardAndState()
	{
		String board = "";		
		for(int i = 0; i < BoardPieces.length; i++)
		{
			for(int k = 0; k < BoardPieces.length; k++)
			{
				board += ((BoardPieces[i][k].IsWhite) ? BoardPieces[i][k].Type : -BoardPieces[i][k].Type) + ":";
			}
			board = board.substring(0, board.length() - 1) + "\n";
		}
		int whitePlayerType = (playerColor.contains("White")) ? PlayerType.Human.ordinal() : (opponentType.contains("Computer")) ? PlayerType.Computer.ordinal() : PlayerType.Human.ordinal();
		int blackPlayerType = (playerColor.contains("Black")) ? PlayerType.Human.ordinal() : (opponentType.contains("Computer")) ? PlayerType.Computer.ordinal() : PlayerType.Human.ordinal();
		int difficultyType = ((difficulty.contains("Beginner")) 
				 ? DifficultyOptions.Beginner : (difficulty.contains("Amateur")) 
				 ? DifficultyOptions.Amateur : DifficultyOptions.Professional).ordinal();
		
		// Access the default SharedMainActivity.preferences Editor
	    SharedPreferences.Editor editor = MainActivity.preferences.edit();

	    // Edit the saved MainActivity.preferences
	    editor.putBoolean("Turn", IsWhiteTurn);
	    editor.putString("History", GameNDKFunctions.GetHistory());
	    editor.putInt("WhitePlayerType", whitePlayerType);
	    editor.putInt("BlackPlayerType", blackPlayerType);
	    editor.putInt("DifficultyType", difficultyType);
	    editor.putString("Board", board);
	    editor.putString("YourSet", playerSet);
	    editor.putString("OpponentSet", opponentSet);
	    editor.commit();
	}
	
	
	private void DrawBoard()
	{
		ImageView board = (ImageView) findViewById(R.id.Board);
		int height = board.getHeight();
		int width = board.getWidth();
		int div_height = (int) ((height / 8) * .85);
		int div_width = (int) ((width / 8) * .85);
		ReadBoard();
		RelativeLayout holder = (RelativeLayout) findViewById(R.id.GameMainRelativeLayout);
		RemovePieceImages(holder);
		
		for (int i = 0; i < BoardPieces.length; i++)
		{
            for (int k = 0; k < BoardPieces.length; k++) 
            {
                ImageView item = new ImageView(this.getApplicationContext());
                images.add(item);
                item.setOnClickListener(new OnClickListener() 
                {
                    @Override
                    public void onClick(View v) 
                    {
                        ImageView item = (ImageView) v;
                        Coord pos = GetBoardSector(item.getLeft(), item.getTop());
                        if (selectedPiece == null && ((IsWhiteTurn && !BoardPieces[pos.x][pos.y].IsWhite) || (!IsWhiteTurn && BoardPieces[pos.x][pos.y].IsWhite)))
                            return;

                        if (selectedPiece != null) 
                        {
                            Coord pieceposition = GetBoardSector(selectedPiece.getLeft(), selectedPiece.getTop());
                            Coord moveposition = GetBoardSector(item.getLeft(), item.getTop());
                            SetDrawableResource(pieceposition, selectedPiece);

                            selectedPiece = null;
                            
                            if (GameNDKFunctions.MakeMove(pieceposition.x, pieceposition.y, moveposition.x, moveposition.y))
                            {                            	
                            	//Draw the board
                                DrawBoard();
                                
								//Switch the turn
                            	IsWhiteTurn = !IsWhiteTurn;
                                
                                //Display messages
                                DisplayGameStatusMessages(!IsWhiteTurn);
                                
                                //Save the game
                                SaveBoardAndState();
                            	
								// check if the player moved a pawn to the opposite side of the board and convert it if so
								int endRow = !IsWhiteTurn ? 0 : 7;
								if (moveposition.x == endRow && BoardPieces[moveposition.x][moveposition.y].Type == PieceType.PawnPiece.ordinal())
								{
									DisplayConvertPawnMenu(moveposition.x, moveposition.y, !IsWhiteTurn);
								}
								else if (opponentType.compareTo("Computer") == 0 && !gameOver)
                                {	
                                    MakeComputerMove();
                                    Handler handler = new Handler(); 
                                    handler.postDelayed(new Runnable() { 
                                         @Override
										public void run() { 
                                             //Display messages
                                             DisplayGameStatusMessages(!IsWhiteTurn);
                                             DrawBoard();
                                             SaveBoardAndState();
                                         } 
                                    }, 1000); 
                                }
                            }
                            //Crouton.makeText(GameActivity.this, pieceposition.x + " " + pieceposition.y + " : " + moveposition.x + " " + moveposition.y + " : " + successful, Style.ALERT).show();
                            return;
                        }
                        Coord position = GetBoardSector(item.getLeft(), item.getTop());
                        
                        //Check for someone selecting a sqaure with nothing in it
                        if (BoardPieces[position.x][position.y].Type == PieceType.None.ordinal())
                            return;

                        selectedPiece = item;
                        SetHighlightedDrawableResource(position, item);
                    }
                });
                RelativeLayout.LayoutParams para = new RelativeLayout.LayoutParams(div_width, div_height);
                para.leftMargin = BoardPieces[i][k].Position.x;
                para.topMargin = BoardPieces[i][k].Position.y;
                holder.addView(item, para);
                SetDrawableResource(new Coord(i, k), item);
            }
		}
	}


	private void SetDrawableResource(Coord position, ImageView item) 
	{
		switch (BoardPieces[position.x][position.y].Type)
		{
		case 0: //None
			item.setBackgroundResource(0);
			break;
		case 1: //King
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.king_white);
			else
				item.setBackgroundResource(R.drawable.king_black);
			break;
		case 2: //Queen
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.queen_white);
			else
				item.setBackgroundResource(R.drawable.queen_black);
			break;
		case 3: // Rook
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.rook_white);
			else
				item.setBackgroundResource(R.drawable.rook_black);
			break;
		case 4: //Bishop
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.bishop_white);
			else
				item.setBackgroundResource(R.drawable.bishop_black);
			break;
		case 5: //Knight
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.knight_white);
			else
				item.setBackgroundResource(R.drawable.knight_black);
			break;
		case 6: //Pawn
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.pawn_white);
			else
				item.setBackgroundResource(R.drawable.pawn_black);
			break;
		default:
	        Log.e("TAG", "Hit the Default case in the game activity switch");
			break;
		}
	}

	
	private void SetHighlightedDrawableResource(Coord position, ImageView item) 
	{
		switch (BoardPieces[position.x][position.y].Type)
		{
		case 0: //None
			item.setBackgroundResource(0);
			break;
		case 1: //King
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.king_white_highlighted);
			else
				item.setBackgroundResource(R.drawable.king_black_highlighted);
			break;
		case 2: //Queen
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.queen_white_highlighted);
			else
				item.setBackgroundResource(R.drawable.queen_black_highlighted);
			break;
		case 3: //Rook
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.rook_white_highlighted);
			else
				item.setBackgroundResource(R.drawable.rook_black_highlighted);
			break;
		case 4: //Bishop
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.bishop_white_highlighted);
			else
				item.setBackgroundResource(R.drawable.bishop_black_highlighted);
			break;
		case 5: //Knight
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.knight_white_highlighted);
			else
				item.setBackgroundResource(R.drawable.knight_black_highlighted);
			break;
		case 6: //Pawn
			if (BoardPieces[position.x][position.y].IsWhite)
				item.setBackgroundResource(R.drawable.pawn_white_highlighted);
			else
				item.setBackgroundResource(R.drawable.pawn_black_highlighted);
			break;
		default:
	        Log.e("TAG", "Hit the Default case in the game activity switch");
			break;
		}
	}
	
	
	public Coord GetBoardSector(int x, int y) 
	{
		ImageView board = (ImageView) findViewById(R.id.Board);
		int height = board.getHeight();
		int width = board.getWidth();
		int div_height = height / 8;
		int div_width = width / 8;

		int xresult = x / div_width;
		int yresult = y / div_height;

		return new Coord(yresult, xresult);
	}
	
	
	public int GetPieceSetType(String piece)
	{
		if(piece.contains("Kings"))
			return PieceType.KingPiece.ordinal();
		else if(piece.contains("Queens"))
			return PieceType.QueenPiece.ordinal();
		else if(piece.contains("Knights"))
			return PieceType.KnightPiece.ordinal();
		else if(piece.contains("Rooks"))
			return PieceType.RookPiece.ordinal();
		else if(piece.contains("Bishops"))
			return PieceType.BishopPiece.ordinal();
		else if(piece.contains("Pawns"))
			return PieceType.PawnPiece.ordinal();
		else
			return 1337;
	}
	
	
	public void HighlightPlayerAsWhite(int yourset, int opposet)
	{
		ImageView player = (ImageView) findViewById(R.id.PlayerTurnIcon);
		switch (yourset)
		{
		case 1: //King
			player.setBackgroundResource(R.drawable.king_white);
			break;
		case 2: //Queen
			player.setBackgroundResource(R.drawable.queen_white);
			break;
		case 3: // Rook
			player.setBackgroundResource(R.drawable.rook_white);
			break;
		case 4: //Bishop
			player.setBackgroundResource(R.drawable.bishop_white);
			break;
		case 5: //Knight
			player.setBackgroundResource(R.drawable.knight_white);
			break;
		case 6: //Pawn
			player.setBackgroundResource(R.drawable.pawn_white);
			break;
		default:
	        Log.e("TAG", "Hit the Default case in the game activity switch");
			break;
		}
		ImageView oppo = (ImageView) findViewById(R.id.OppoTurnIcon);
		switch (opposet)
		{
		case 1: //King
			oppo.setBackgroundResource(R.drawable.king_black);
			break;
		case 2: //Queen
			oppo.setBackgroundResource(R.drawable.queen_black);
			break;
		case 3: // Rook
			oppo.setBackgroundResource(R.drawable.rook_black);
			break;
		case 4: //Bishop
			oppo.setBackgroundResource(R.drawable.bishop_black);
			break;
		case 5: //Knight
			oppo.setBackgroundResource(R.drawable.knight_black);
			break;
		case 6: //Pawn
			oppo.setBackgroundResource(R.drawable.pawn_black);
			break;
		default:
	        Log.e("TAG", "Hit the Default case in the game activity switch");
			break;
		}
	}
	
	
	public void HighlightOpponentAsWhite(int yourset, int opposet)
	{
		ImageView player = (ImageView) findViewById(R.id.PlayerTurnIcon);
		switch (yourset)
		{
		case 1: //King
			player.setBackgroundResource(R.drawable.king_black);
			break;
		case 2: //Queen
			player.setBackgroundResource(R.drawable.queen_black);
			break;
		case 3: // Rook
			player.setBackgroundResource(R.drawable.rook_black);
			break;
		case 4: //Bishop
			player.setBackgroundResource(R.drawable.bishop_black);
			break;
		case 5: //Knight
			player.setBackgroundResource(R.drawable.knight_black);
			break;
		case 6: //Pawn
			player.setBackgroundResource(R.drawable.pawn_black);
			break;
		default:
	        Log.e("TAG", "Hit the Default case in the game activity switch");
			break;
		}
		ImageView oppo = (ImageView) findViewById(R.id.OppoTurnIcon);
		switch (opposet)
		{
		case 1: //King
			oppo.setBackgroundResource(R.drawable.king_white);
			break;
		case 2: //Queen
			oppo.setBackgroundResource(R.drawable.queen_white);
			break;
		case 3: // Rook
			oppo.setBackgroundResource(R.drawable.rook_white);
			break;
		case 4: //Bishop
			oppo.setBackgroundResource(R.drawable.bishop_white);
			break;
		case 5: //Knight
			oppo.setBackgroundResource(R.drawable.knight_white);
			break;
		case 6: //Pawn
			oppo.setBackgroundResource(R.drawable.pawn_white);
			break;
		default:
	        Log.e("TAG", "Hit the Default case in the game activity switch");
			break;
		}
	}	
	
	
	private void ShowMenu(final boolean HideReturnToGame) 
	{
		final Dialog log = new Dialog(GameActivity.this);
		log.setTitle("Menu Options");
		log.setCancelable(false);
		
		LayoutParams param =  log.getWindow().getAttributes();
		param.windowAnimations = R.style.dialog_animation;
		
		LayoutParams params = new LayoutParams();
		params.height = HideReturnToGame ? 500 : 600;
		params.width = param.width;
		
		View view = getLayoutInflater().inflate(R.layout.menudialog, null);
		log.setContentView(view, params);
		
		TextView returntomenu = (TextView)log.findViewById(R.id.returntomenu);
		returntomenu.setOnClickListener(new OnClickListener() 
		{
			@Override
			public void onClick(View v)
			{
				Intent intent = new Intent();
				intent.setClass(getApplicationContext(), com.chessmob.chessmobandroidfree.MainActivity.class);
				intent.putExtra("ClearSavedData", HideReturnToGame);
				startActivity(intent);
			}
		});
		
		TextView newgame = (TextView)log.findViewById(R.id.newgame);
		newgame.setOnClickListener(new OnClickListener() 
		{			
			@Override
			public void onClick(View v)
			{
				Intent intent = new Intent();
				intent.setClass(getApplicationContext(), com.chessmob.chessmobandroidfree.GameSettings.class);
				intent.putExtra("ClearSavedData", HideReturnToGame);
				startActivity(intent);
			}
		});
		TextView returntogame = (TextView)log.findViewById(R.id.returntogame);
		returntogame.setOnClickListener(new OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{
				log.cancel();
			}
		});
		
		int variableHeight = 600;
		if(HideReturnToGame)
		{
			returntogame.setVisibility(View.GONE);
			log.setTitle("Checkmate! " + (!IsWhiteTurn ? "White Has " : "Black Has ") + "Won");
		    SharedPreferences.Editor edit = MainActivity.preferences.edit();
		    edit.clear();
		    edit.commit();
			variableHeight = 500;
		}
		log.show();
		log.getWindow().setLayout(param.width, variableHeight);
	}
	
	
	public void DisplayConvertPawnMenu(final int row, final int column, final boolean isWhiteTurn)
	{ 	//R.drawable.queen_white, R.drawable.rook_white,
		//R.drawable.queen_black, R.drawable.rook_black, 
		final int[] piecesWhite = { R.drawable.bishop_white, R.drawable.knight_white};
		final int[] piecesBlack = { R.drawable.bishop_black, R.drawable.knight_black};
		ConvertedPieceIndex = 0;
		

		// Access the default SharedMainActivity.preferences Editor
	    SharedPreferences.Editor editor = MainActivity.preferences.edit();
	    editor.putBoolean("DisplayConvertPiece", true);
	    editor.putInt("ConvertPieceRow", row);
	    editor.putInt("ConvertPieceColumn", column);
	    editor.commit();
	    
	    //Set up the Dialog object
		final Dialog log = new Dialog(GameActivity.this);
		log.setTitle("Conversion Options");
		log.setCancelable(false);
		
		LayoutParams param =  log.getWindow().getAttributes();
		param.windowAnimations = R.style.dialog_animation;
		
		LayoutParams params = new LayoutParams();
		params.height = 700;
		params.width = param.width;
		
		View view = getLayoutInflater().inflate(R.layout.convertpiecemenu, null);
		log.setContentView(view, params);
		
		//Switch color of queen image if player is black
		final ImageView icon = (ImageView)log.findViewById(R.id.PieceIcon);
		if(isWhiteTurn)
			icon.setImageResource(R.drawable.bishop_white);
		else
			icon.setImageResource(R.drawable.bishop_black);
		
		ImageView leftarrow = (ImageView)log.findViewById(R.id.LeftConvertPiece);
		leftarrow.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				if(--ConvertedPieceIndex < 0)
					ConvertedPieceIndex = 1;
				if(isWhiteTurn)
					icon.setImageResource(piecesWhite[ConvertedPieceIndex]);
				else
					icon.setImageResource(piecesBlack[ConvertedPieceIndex]);
			}			
		});
		ImageView rightarrow = (ImageView)log.findViewById(R.id.RightConvertPiece);
		rightarrow.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				if(++ConvertedPieceIndex > 1)
					ConvertedPieceIndex = 0;
				if(isWhiteTurn)
					icon.setImageResource(piecesWhite[ConvertedPieceIndex]);
				else
					icon.setImageResource(piecesBlack[ConvertedPieceIndex]);
			}			
		});
		Button accept = (Button)log.findViewById(R.id.AcceptButton);
		accept.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				switch(ConvertedPieceIndex)
				{
				case 0:
					ConvertedPieceIndex = 4;
					break;
				case 1:
					ConvertedPieceIndex = 5;
					break;
				}
				GameNDKFunctions.ConvertPawn(row, column, ConvertedPieceIndex);
                DrawBoard();
				if (opponentType.compareTo("Computer") == 0 && !gameOver)
                {
                    MakeComputerMove();
                    //Display messages
                    DisplayGameStatusMessages(!IsWhiteTurn);
                    DrawBoard();
                }
				
			    SharedPreferences.Editor editor = MainActivity.preferences.edit();
			    editor.putBoolean("DisplayConvertPiece", false);
			    editor.putInt("ConvertPieceRow", -1);
			    editor.putInt("ConvertPieceColumn", -1);
			    editor.commit();
			    
                SaveBoardAndState();
				log.dismiss();
			}			
		});
		//Add button listeners 
		log.show();
		log.getWindow().setLayout(param.width, 700);
	}
	
	
	//Clears the board of all piece imageviews
	private void RemovePieceImages(RelativeLayout holder)
	{
		for(int i = 0; i < images.size(); i++)
		{
			holder.removeView(images.get(i));
		}
		images.clear();
		holder.invalidate();
	}
	
	
	private void MakeComputerMove()
	{
		GameNDKFunctions.MakeComputerMove();
		IsWhiteTurn = !IsWhiteTurn;
	}

	
	//Displays Check, Checkmate, and Stalemate messages
	//Input: boolean to determine who's turn it is since the global variable is manipulated differently
	//when the player or computer makes a move
	private void DisplayGameStatusMessages(boolean Turn)
	{
		if (GameNDKFunctions.CheckCheck())
		{
			Crouton.cancelAllCroutons();
		    if (GameNDKFunctions.CheckCheckmate())
		    {
		    	gameOver = true;
		        ShowMenu(true);
		    }
		    else
		    {
		        Crouton.makeText(GameActivity.this, (!Turn ? "White Is " : "Black Is ") + "In Check", Style.ALERT).show();
		    }
		}
		else if (GameNDKFunctions.CheckStalemate())
		{
			Crouton.cancelAllCroutons();
		    Crouton.makeText(GameActivity.this, "Stalemate", Style.ALERT).show();
		    gameOver = true;
		    ShowMenu(true);
		}
	}
}
