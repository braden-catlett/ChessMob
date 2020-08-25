//
//  RootViewController.h
//  ChessMob_iOS
//
//  Created by Michael A Weidemann on 5/2/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GameMaster.h"

@interface RootViewController : UIViewController {
	IBOutlet NSMutableArray *views;
    IBOutlet UILabel *chessmobtitle;
}

@property (nonatomic, retain) IBOutlet NSMutableArray *views;
@property(retain) IBOutlet UIButton *resumeButton;
- (void) ChangeView: (NSString*) newView;
- (IBAction) OnStartButtonClick:(id) sender;
- (IBAction) OnResumeButtonClick:(id)sender;
- (void) StartGameWithGameMaster: (ChessMob::GameMaster*) gameMaster withComputer: (bool) hasComputer computerGoesFirst: (bool) compGoesFirst;

@end
