//
//  GameScreenController.h
//  ChessMob_iOS
//
//  Created by Michael A Weidemann on 5/7/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import "RootViewController.h"
#import <UIKit/UIKit.h>
#ifndef GAME_MASTER_INCLUDED
#define GAME_MASTER_INCLUDED
#import "GameMaster.h"
#endif
@interface GameScreenController : UIViewController <UIAlertViewDelegate>{
	IBOutlet UIImageView *highlightView;
	IBOutlet UIButton *undoButton;
    IBOutlet UIImageView *playerIcon;
    IBOutlet UIImageView *opponentIcon;
	ChessMob::GameMaster *gameMaster;
	bool computerPlayerExists;
	bool isWhiteTurn;
    int playerType;
    int opponentType;
}

@property (retain, nonatomic) RootViewController *rootViewController;
@property (retain, nonatomic) IBOutlet UIImageView *playerIcon;
@property (retain, nonatomic) IBOutlet UIImageView *opponentIcon;
@property (nonatomic) bool computerPlayerExists;
@property (nonatomic) bool isWhiteTurn;
@property (nonatomic) int playerType;
@property (nonatomic) int opponentType;
@property (retain, nonatomic) IBOutlet UIButton *undoButton;
@property (retain, nonatomic) IBOutlet UIImageView *highlightView;
- (IBAction) OnUndoButtonClick: (id) sender;
- (IBAction) OnBoardButtonClick: (id) sender;
- (ChessMob::GameMaster*) getGameMaster;
- (void) setGameMaster: (ChessMob::GameMaster*) gameMaster;
- (void) saveGame;
- (void) setPlayerIcons;

@end
