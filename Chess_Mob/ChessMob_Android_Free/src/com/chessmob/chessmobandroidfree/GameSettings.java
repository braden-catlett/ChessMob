package com.chessmob.chessmobandroidfree;

import com.chessmob.chessmob_android_free.R;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class GameSettings extends Activity 
{
	private String OpponentChoice = "Computer";
	
	private String Difficulty = "Beginner";
	
	private String Color = "White";
	//"Queens","Rooks",
	private static final String[] PieceOptions = {"Knights","Bishops","Pawns"};
	private static int YourPieceIndex = 0;
	private static int OppoPieceIndex = 0;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_game_settings);
		com.facebook.AppEventsLogger.activateApp(getApplicationContext(), getString(R.string.app_id));
		
		if(this.getIntent() != null)
		{
			if(this.getIntent().getBooleanExtra("ClearSavedData", false))
			{
				MainActivity.preferences.edit().clear().commit();
			}
		}
		Settings_Init();
		Start_Init();
	}
	

	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.game_settings, menu);
		return true;
	}

	
	private void Settings_Init()
	{
		//Opponent		
		Button player = (Button)findViewById(R.id.Player);
		player.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				OpponentChoice = "Player";
				Button p = (Button)v;
				p.setBackgroundResource(R.drawable.highlightedroundedcorners);
				Button computer = (Button)findViewById(R.id.Computer);
				computer.setBackgroundResource(R.drawable.roundedcorners);
				HideComputerOptions();
			}
		});
		Button computer = (Button)findViewById(R.id.Computer);
		computer.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				OpponentChoice = "Computer";
				Button p = (Button)v;
				p.setBackgroundResource(R.drawable.highlightedroundedcorners);
				Button player = (Button)findViewById(R.id.Player);
				player.setBackgroundResource(R.drawable.roundedcorners);
				ShowComputerOptions();
			}
		});
		//Difficulty
		Button easy = (Button)findViewById(R.id.Beginner);
		easy.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				Difficulty = "Beginner";
				Button p = (Button)v;
				p.setBackgroundResource(R.drawable.highlightedroundedcorners);
				Button med = (Button)findViewById(R.id.Amateur);
				med.setBackgroundResource(R.drawable.roundedcorners);
				Button hard = (Button)findViewById(R.id.Professional);
				hard.setBackgroundResource(R.drawable.roundedcorners);
			}
		});
		Button med = (Button)findViewById(R.id.Amateur);
		med.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				Difficulty = "Amateur";
				Button p = (Button)v;
				p.setBackgroundResource(R.drawable.highlightedroundedcorners);
				Button easy = (Button)findViewById(R.id.Beginner);
				easy.setBackgroundResource(R.drawable.roundedcorners);
				Button hard = (Button)findViewById(R.id.Professional);
				hard.setBackgroundResource(R.drawable.roundedcorners);
			}
		});
		Button hard = (Button)findViewById(R.id.Professional);
		hard.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				Difficulty = "Professional";
				Button p = (Button)v;
				p.setBackgroundResource(R.drawable.highlightedroundedcorners);
				Button easy = (Button)findViewById(R.id.Beginner);
				easy.setBackgroundResource(R.drawable.roundedcorners);
				Button hard = (Button)findViewById(R.id.Amateur);
				hard.setBackgroundResource(R.drawable.roundedcorners);
			}
		});
		//Player Color
		Button white = (Button)findViewById(R.id.White);
		white.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				Color = "White";
				Button p = (Button)v;
				p.setBackgroundResource(R.drawable.highlightedroundedcorners);
				Button black = (Button)findViewById(R.id.Black);
				black.setBackgroundResource(R.drawable.roundedcorners);
			}
		});
		Button black = (Button)findViewById(R.id.Black);
		black.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				Color = "Black";
				Button p = (Button)v;
				p.setBackgroundResource(R.drawable.highlightedroundedcorners);
				Button white = (Button)findViewById(R.id.White);
				white.setBackgroundResource(R.drawable.roundedcorners);
			}
		});
		//Your Pieces
		ImageView left = (ImageView)findViewById(R.id.LeftConvertPiece);
		ImageView right = (ImageView)findViewById(R.id.RightConvertPiece);
		
		left.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				YourPieceIndex--;
				if(YourPieceIndex < 0)
					YourPieceIndex = PieceOptions.length - 1;
				TextView content = (TextView)findViewById(R.id.YourPiece);
				content.setText(PieceOptions[YourPieceIndex]);
				ImageView image = (ImageView)findViewById(R.id.YourSet);
				SetImageResource(image, content.getText().toString());
			}
		});
		
		right.setOnClickListener(new OnClickListener() {	
			@Override
			public void onClick(View v) {
				YourPieceIndex++;
				if(YourPieceIndex >= PieceOptions.length)
					YourPieceIndex = 0;
				TextView content = (TextView)findViewById(R.id.YourPiece);
				content.setText(PieceOptions[YourPieceIndex]);
				ImageView image = (ImageView)findViewById(R.id.YourSet);
				SetImageResource(image, content.getText().toString());
			}
		});
		//Opponent Pieces
		left = (ImageView)findViewById(R.id.LeftSelectOppoPiece);
		right = (ImageView)findViewById(R.id.RightSelectOppoPiece);
		
		left.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				OppoPieceIndex--;
				if(OppoPieceIndex < 0)
					OppoPieceIndex = PieceOptions.length - 1;
				TextView content = (TextView)findViewById(R.id.OppoPiece);
				content.setText(PieceOptions[OppoPieceIndex]);
				ImageView image = (ImageView)findViewById(R.id.OpponentSet);
				SetImageResource(image, content.getText().toString());
			}
		});
		
		right.setOnClickListener(new OnClickListener() {	
			@Override
			public void onClick(View v) {
				OppoPieceIndex++;
				if(OppoPieceIndex >= PieceOptions.length)
					OppoPieceIndex = 0;
				TextView content = (TextView)findViewById(R.id.OppoPiece);
				content.setText(PieceOptions[OppoPieceIndex]);
				ImageView image = (ImageView)findViewById(R.id.OpponentSet);
				SetImageResource(image, content.getText().toString());
			}
		});
	}

	
	private void Start_Init()
	{
		Button start = (Button)findViewById(R.id.StartGame);
		start.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v) 
			{
				Bundle info = new Bundle();
				String yourset = ((TextView)findViewById(R.id.YourPiece)).getText().toString();
				String opposet = ((TextView)findViewById(R.id.OppoPiece)).getText().toString();
				info.putString("player", OpponentChoice);
				info.putString("difficulty", Difficulty);
				info.putString("color", Color);
				info.putString("yourset", yourset);
				info.putString("opposet", opposet);
				info.putBoolean("load", false);
				Intent intent = new Intent();
				intent.setClass(getApplicationContext(), com.chessmob.chessmobandroidfree.GameActivity.class);
				intent.putExtras(info);
				startActivity(intent);				
			}			
		});
	}
	
	
	private void ShowComputerOptions()
	{
		TextView difficultytitle = (TextView)findViewById(R.id.DifficultyTitle);
		Button easy = (Button)findViewById(R.id.Beginner);
		Button med = (Button)findViewById(R.id.Amateur);
		Button hard = (Button)findViewById(R.id.Professional);
		difficultytitle.setVisibility(View.VISIBLE); 
		easy.setVisibility(View.VISIBLE);
		med.setVisibility(View.VISIBLE);
		hard.setVisibility(View.VISIBLE);	
		
		TextView PlayerColortitle = (TextView)findViewById(R.id.PlayerColorTitle);
		Button white = (Button)findViewById(R.id.White);
		Button black = (Button)findViewById(R.id.Black);		
		PlayerColortitle.setVisibility(View.VISIBLE); 
		white.setVisibility(View.VISIBLE);
		black.setVisibility(View.VISIBLE);
	}

	
	private void HideComputerOptions() 
	{
		TextView difficultytitle = (TextView)findViewById(R.id.DifficultyTitle);
		Button easy = (Button)findViewById(R.id.Beginner);
		Button med = (Button)findViewById(R.id.Amateur);
		Button hard = (Button)findViewById(R.id.Professional);
		difficultytitle.setVisibility(View.INVISIBLE); 
		easy.setVisibility(View.INVISIBLE);
		med.setVisibility(View.INVISIBLE);
		hard.setVisibility(View.INVISIBLE);			
				
		TextView PlayerColortitle = (TextView)findViewById(R.id.PlayerColorTitle);
		Button white = (Button)findViewById(R.id.White);
		Button black = (Button)findViewById(R.id.Black);		
		PlayerColortitle.setVisibility(View.INVISIBLE); 
		white.setVisibility(View.INVISIBLE);
		black.setVisibility(View.INVISIBLE);
	}
	
	private void SetImageResource(ImageView item, String type)
	{
		if(type.contains("Queen"))
		{
			item.setImageResource(R.drawable.queen_white);
		}
		else if(type.contains("Knight"))
		{
			item.setImageResource(R.drawable.knight_white);
		}
		else if(type.contains("Bishop"))
		{
			item.setImageResource(R.drawable.bishop_white);
		}
		else if(type.contains("Rook"))
		{
			item.setImageResource(R.drawable.rook_white);
		}
		else if(type.contains("Pawn"))
		{
			item.setImageResource(R.drawable.pawn_white);
		}
	}
}
