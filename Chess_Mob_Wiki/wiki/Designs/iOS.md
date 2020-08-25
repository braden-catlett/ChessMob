# iOS Design Documents #
## Overview
Since ChessMob is a multiplatform game with shared core c++ code, individual platforms need to interface with this c++ code. On iOS, this will be completed using wrapper Objective-C++ classes. Please see [this article](http://philjordan.eu/article/mixing-objective-c-c++-and-objective-c++) for methods of doing that.

iOS Cocoa handles checking if a button is pressed and state changes for down/up/inactive.

##Functions
#Display

Draws the current view

Input - NA

Output - Current view is displayed

Returns - NA

##Button Functions
#NewGamePressed

Sets up and starts a new game (asks for confirmation of new game if another game already exists)

Input - Button id

Output - View changes to game screen

Returns - IBAction

#ResumePressed

Loads and starts a previous game if it exists. Shouldn't be visible and therefore not called if it doesn't exist.

Input - Button id

Output - View changes to game screen

Returns - IBAction

#SettingsPressed

Loads the settings view.

Input - Button id

Output - View changes to settings screen

Returns - IBAction

#BackPressed

Unloads the settings view, returns to previous view.

Input - Button id

Output - View changes to settings screen

Returns - IBAction

#MenuPressed

Called when the Menu button is pressed in the game or settings menu. Changes the view to the previous view.

Input - Button id

Output - View changes to menu

Returns - IBAction

#UndoPressed

Called when the Undo button is pressed in the game menu. Calls UndoMove in the backend.

#GridButtonPressed

Called when any of the board buttons is pressed. If no grids are already selected and the grid has a piece on it, select this grid and display possible moves.
If a grid is already selected, check if the new square is the current square. If so, deselect it. Else, check if the new square is an acceptable possible move. If so, move the piece there (using backend). Else, deselect the old square, turn off possible move highlighting.