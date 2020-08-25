# Android Design Documents #
## Overview
Both Mobile platforms will look similar to each other just so they have commonality but each will be using different software to communicate with our backend.
##Activities
Main Activity - Creates Main Menu

Game Settings Activity - Creates the Game Settings Menu and allows the user to select their settings

Game Activity - Creates the board, passes the game settings to the backend, and then draws the board. During game play this activity will be communicating with the backend to perform moves, update the board and manage the game.

Settings Activity - Creates the settings activity that will allow the user to set general settings for the game

##Front End Specific Functions
#DrawBoard 

Draws the game board based off of values in the board array

Input - NA

Output - Board is drawn

Returns NA


#GetBoardSector 

Gets the coordinates of the board position

Input - x and y coordinates of the touch event (Standard Window Coordinate system)

Output - x and y coordinates in Board Coordinate system

Returns Coord object


#Init_BoardPositions 

Fills piece coordinates in the piece array

Input - NA

Output - each piece in piece array has coordinates

Returns - NA


#SetDrawableResource 

Sets the correct image to piece at the specified coordinates

Input - Coordinates of piece and the ImageView that will visually represent the piece

Output - ImageView has correct image

Returns - NA


#SetHighlightedDrawableResource 

Sets the correct highlighted image to piece at the specified coordinates because the user selected it

Input - Coordinates of piece and the ImageView that will visually represent the piece

Output - ImageView has correct image

Returns - NA


##Use Diagram
The application will start with a main menu that will give the user the option to start a new game, change any of the game settings or resume a previous game that has not been finished. If a user decides to change some settings they will be brought to a settings screen that will display all changeable settings. Once the user to satisfied, the new settings will be saved and the user will be brought back to the main menu. If a user had a previous game saved then the resume option will become visible and if a user selects it, they will be taken to the game screen while the android front end passing the board and player information to our backend so their game can resume right where they left off. The information passed will contain the position of all pieces on the board along with what type each piece is. From there normal game play will begin. The most common case will be the user selecting new game. This will bring them to the game settings screen where they can select what type of opponent they want to face (Computer or Player). If they chose Computer then they can select the difficulty they want to play against. They will also need to select their piece set that they want to use along with the piece set that they want to face unless they are facing another real life opponent. After all of those are selected then the user can press start game and they will be taken to the game screen with all their chosen settings already sent off to the backend who will initialize the game and set up and maintain the board.
##Communication with Backend
Both mobile platforms will need to be able to communication with our backend through some form of interface. For Android this will be done with the NDK (Native Development Kit). The ways they will talk will be first by the front end sending off the game settings in order to allow the backend to know who will be playing (Computer or Players), Computer difficulty, the player's piece set and the opponent's piece set if they are facing a computer player. The backend will need to know what color the player and the opponent is. Once the game is initialized then the front will repeatedly ask the backend if the move that the player has chosen is valid, get the piece positions of the board after each move, check for victory after each move and tell the backend to save the game state after each move.
