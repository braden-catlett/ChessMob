    //
//  MenuScreen.m
//  ChessMob_iOS
//
//  Created by Michael A Weidemann on 5/2/13.
//  Copyright 2013 Hubbell Games. All rights reserved.
//

#import "Structs.h"
#import "SettingsMenuController.h"
#import "GameScreenController.h"

@implementation SettingsMenuController

@synthesize label, difficultySelectorLabel, opponentSelector, difficultySelector, colorSelector, p1PieceSelector, p2PieceSelector, startGameButton, rootViewController;
//hessMob::QueenPiece, ChessMob::RookPiece,
ChessMob::PieceType pieceTypes[] = {ChessMob::BishopPiece, ChessMob::KnightPiece, ChessMob::PawnPiece};
int p1PieceType = 0;
int p2PieceType = 0;

- (id) init {
	self = [super init];
	if (self != nil) {
		self.title = NSLocalizedString(@"Game Settings",@"");
	}
	return self;
}

- (IBAction) OnOpponentSelectorClick:(id)sender{
    if (opponentSelector.selectedSegmentIndex == 0){ // Human
        difficultySelectorLabel.alpha = 0;
        difficultySelector.alpha = 0;
    }
    else{
        difficultySelectorLabel.alpha = 1;
        difficultySelector.alpha = 1;
    }
}

- (IBAction) OnP1PieceButtonClick: (id) sender{
	p1PieceType = (p1PieceType + 1) % 3;//5; // Since there are 5 types of pieces
	
	NSString *newLabel;
	switch (pieceTypes[p1PieceType]) {
		/*case ChessMob::QueenPiece:
			newLabel = @"Queens";
			break;
		case ChessMob::RookPiece:
			newLabel = @"Rooks";
			break;*/
		case ChessMob::BishopPiece:
			newLabel = @"Bishops";
			break;
		case ChessMob::KnightPiece:
			newLabel = @"Knights";
			break;
		case ChessMob::PawnPiece:
			newLabel = @"Pawns";
			break;
		default:
			newLabel = @"Unknown, error";
			break;
	}

	[sender setTitle: newLabel forState: UIControlStateNormal];
}

- (IBAction) OnP2PieceButtonClick: (id) sender{
	p2PieceType = (p2PieceType + 1) % 3;//5; // Since there are 5 types of pieces
	
	NSString *newLabel;
	switch (pieceTypes[p2PieceType]) {
		/*case ChessMob::QueenPiece:
			newLabel = @"Queens";
			break;
		case ChessMob::RookPiece:
			newLabel = @"Rooks";
			break;*/
		case ChessMob::BishopPiece:
			newLabel = @"Bishops";
			break;
		case ChessMob::KnightPiece:
			newLabel = @"Knights";
			break;
		case ChessMob::PawnPiece:
			newLabel = @"Pawns";
			break;
		default:
			newLabel = @"Unknown, error";
			break;
	}
	
	[sender setTitle: newLabel forState: UIControlStateNormal];
}

- (IBAction) OnStartButtonClick:(id) sender{
	bool hasComputer;
	bool computerGoesFirst = NO;
    int ptype;
    int otype;
	ChessMob::PlayerType whitePlayerType, blackPlayerType;
	ChessMob::PieceType blackPieceType, whitePieceType;
    whitePieceType = pieceTypes[ p1PieceType ];
    blackPieceType = pieceTypes[ p2PieceType ];
    if ([opponentSelector selectedSegmentIndex] == 0){ // human v human
        hasComputer = NO;
        computerGoesFirst = NO;
        whitePlayerType = ChessMob::HumanPlayer;
        blackPlayerType = ChessMob::HumanPlayer;
        ptype = whitePieceType;
        otype = -blackPieceType;
    }
    else{ // Human v Comp or Comp v Human
        hasComputer = YES;
        if ([colorSelector selectedSegmentIndex] == 0){ // Player wants to be white
            computerGoesFirst = NO;
			whitePlayerType = ChessMob::HumanPlayer;
			blackPlayerType = ChessMob::ComputerPlayer;
            ptype = whitePieceType;
            otype = -blackPieceType;
        }
        else { // Player 1 wants to be black
			computerGoesFirst = YES;
			whitePlayerType = ChessMob::ComputerPlayer;
			blackPlayerType = ChessMob::HumanPlayer;
            ptype = -blackPieceType;
            otype = whitePieceType;
		}
	}

    [[NSUserDefaults standardUserDefaults] setInteger: [difficultySelector selectedSegmentIndex]+1 forKey:@"difficulty"];
	
	[rootViewController StartGameWithGameMaster: new ChessMob::GameMaster( whitePlayerType,  whitePieceType,  blackPlayerType,  blackPieceType,
																		  [difficultySelector selectedSegmentIndex] + 1) withComputer: hasComputer
                              computerGoesFirst: computerGoesFirst
                                     playerType:ptype
                                   opponentType:otype]; // offset to match scale of 1-3 from scale of 0-2.
}

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    [label setFont:[UIFont fontWithName:@"UnifrakturMaguntia" size:45]];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
