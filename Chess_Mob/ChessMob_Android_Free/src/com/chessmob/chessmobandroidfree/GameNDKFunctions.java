package com.chessmob.chessmobandroidfree;

public class GameNDKFunctions 
{
    public static native void InitGame(int whitePlayerType, int whitePieceType, int blackPlayerType, int blackPieceType, int difficulty);
	public static native void ResumeGame(int whitePlayerType, int blackPlayerType, int difficulty, int[] pieces, int[] moves, boolean IsWhite);
	public static native boolean MakeMove(int originalx, int originaly, int newx, int newy);
	public static native void MakeComputerMove();
	public static native void Undo();
	public static native int GetPiece(int x, int y);
	public static native void ConvertPawn(int row, int column, int replacementType);
	
	public static native boolean CheckCheck();
	public static native boolean CheckCheckmate();
	public static native boolean CheckStalemate();
	
	public static native String GetHistory();

	static 
	{
        System.loadLibrary("ChessMobAndroid");
    }
}
