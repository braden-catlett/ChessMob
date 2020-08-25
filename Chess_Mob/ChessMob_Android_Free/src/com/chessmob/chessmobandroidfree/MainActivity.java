package com.chessmob.chessmobandroidfree;

import com.chessmob.chessmob_android_free.R;

import android.net.Uri;
import android.os.Bundle;
import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Typeface;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager.LayoutParams;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity 
{
	public static final String Saved_Preferences = "GameSavedData";
	public static SharedPreferences preferences;

	@Override
	protected void onResume() 
	{
		super.onResume();
		com.facebook.AppEventsLogger.activateApp(getApplicationContext(), getString(R.string.app_id));
		CheckforSaveFile();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		if(this.getIntent() != null)
		{
			if(this.getIntent().getBooleanExtra("ClearSavedData", false))
			{
				preferences.edit().clear().commit();
			}
		}
		setContentView(R.layout.activity_main);
		preferences = getSharedPreferences(Saved_Preferences, Context.MODE_PRIVATE);
		Init_OnClicks();
	}
	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	
	void Init_OnClicks()
	{
		Typeface customfont = Typeface.createFromAsset(getAssets(), "fonts/UnifrakturMaguntia.ttf");
		
		TextView title = (TextView)findViewById(R.id.ChessMobTitle);
		title.setTypeface(customfont);

		TextView Resume = (TextView)findViewById(R.id.Resume);
		Resume.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View arg0) 
			{
				Bundle info = new Bundle();
				String player = "";
				String difficulty = "";
				String color = "";
				String yourset = "";
				String opposet = "";
				info.putString("player", player);
				info.putString("difficulty", difficulty);
				info.putString("color", color);
				info.putString("yourset", yourset);
				info.putString("opposet", opposet);
				info.putBoolean("load", true);
				Intent intent = new Intent();
				intent.setClass(getApplicationContext(), com.chessmob.chessmobandroidfree.GameActivity.class);
				intent.putExtras(info);
				startActivity(intent);	
			}			
		});
		
		TextView NewGame = (TextView)findViewById(R.id.NewGame);
		NewGame.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View arg0) 
			{
				if(CheckforSaveFile())
				{
					ShowNewGameMenu();
				}
				else
				{
					Intent intent = new Intent();
					intent.setClass(getApplicationContext(), com.chessmob.chessmobandroidfree.GameSettings.class);
					startActivity(intent);
				}
			}			
		});
		
		TextView Settings = (TextView)findViewById(R.id.updategame);
		Settings.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View arg0) 
			{
				try {
				    startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=com.chessmob.chessmob_android")));
				} catch (android.content.ActivityNotFoundException anfe) {
				    startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/details?id=com.chessmob.chessmob_android&hl=en")));
				}
			}			
		});	
	}


	private boolean CheckforSaveFile() {
		TextView Resume = (TextView)findViewById(R.id.Resume);
	    if(!preferences.getAll().isEmpty())
	    {
			Resume.setVisibility(View.VISIBLE);
			return true;
	    }
		else
		{
			Resume.setVisibility(View.INVISIBLE);
			return false;
		}
	}
	
	
	private void ShowNewGameMenu() 
	{
		final Dialog log = new Dialog(MainActivity.this);
		log.setTitle("Wait!");		
		log.setCancelable(true);
		
		LayoutParams param =  log.getWindow().getAttributes();
		param.windowAnimations = R.style.dialog_animation;
		
		LayoutParams params = new LayoutParams();
		params.height = 700;
		params.width = param.width;
		
		View view = getLayoutInflater().inflate(R.layout.newgameprompt, null);
		log.setContentView(view, params);
		
		
		Button ok = (Button)log.findViewById(R.id.okbutton);
		ok.setOnClickListener(new OnClickListener() 
		{
			@Override
			public void onClick(View v)
			{
				preferences.edit().clear().commit();
				log.dismiss();
				Intent intent = new Intent();
				intent.setClass(getApplicationContext(), com.chessmob.chessmobandroidfree.GameSettings.class);
				startActivity(intent);
			}
		});
		
		Button cancel = (Button)log.findViewById(R.id.cancel);
		cancel.setOnClickListener(new OnClickListener() 
		{			
			@Override
			public void onClick(View v)
			{
				log.dismiss();
			}
		});
		log.show();
		log.getWindow().setLayout(param.width, 700);
	}
}
