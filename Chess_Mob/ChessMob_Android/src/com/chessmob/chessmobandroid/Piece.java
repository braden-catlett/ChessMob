package com.chessmob.chessmobandroid;

public class Piece {
	public int Type;
	public boolean IsWhite;
	public Coord Position;
	
	public Piece(int type, boolean WhiteOrBlack)
	{
		this.Type = type;
		this.IsWhite = WhiteOrBlack;
	}
}
