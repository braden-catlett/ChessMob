//
//  MenuScreen.h
//  ChessMob_iOS
//
//  Created by Michael A Weidemann on 5/2/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import "RootViewController.h"
#import <UIKit/UIKit.h>
#import "GameMaster.h"


@interface SettingsMenuController : UIViewController {
    IBOutlet UILabel *label;
	IBOutlet UILabel *difficultySelectorLabel;
	IBOutlet UISegmentedControl *opponentSelector;
	IBOutlet UISegmentedControl *difficultySelector;
	IBOutlet UISegmentedControl *dolorSelector;
	IBOutlet UIButton *p1PieceSelector;
	IBOutlet UIButton *p2PieceSelector;
	IBOutlet UIButton *startGameButton;
	
}
@property (retain, nonatomic) IBOutlet UILabel *label;
@property (retain, nonatomic) IBOutlet UILabel *difficultySelectorLabel;
@property (retain, nonatomic) RootViewController *rootViewController;
@property (retain, nonatomic) IBOutlet UISegmentedControl *opponentSelector;
@property (retain, nonatomic) IBOutlet UISegmentedControl *difficultySelector;
@property (retain, nonatomic) IBOutlet UISegmentedControl *colorSelector;
@property (retain, nonatomic) IBOutlet UIButton *p1PieceSelector;
@property (retain, nonatomic) IBOutlet UIButton *p2PieceSelector;
@property (retain, nonatomic) IBOutlet UIButton *startGameButton;

- (IBAction) OnP1PieceButtonClick: (id) sender;
- (IBAction) OnP2PieceButtonClick: (id) sender;
- (IBAction) OnStartButtonClick: (id) sender;
- (IBAction) OnOpponentSelectorClick: (id) sender;
@end
