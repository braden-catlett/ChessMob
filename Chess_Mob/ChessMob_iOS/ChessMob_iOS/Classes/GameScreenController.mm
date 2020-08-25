//
//  GameScreenController.m
//  ChessMob_iOS
//
//  Created by Michael A Weidemann on 5/7/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import "GameScreenController.h"
#import "SaveGameObject.h"
#import "Toast+UIView.h"

@implementation GameScreenController
@synthesize rootViewController;
@synthesize isWhiteTurn;
@synthesize undoButton;
@synthesize highlightView;
@synthesize computerPlayerExists;
@synthesize playerType, opponentType;
@synthesize playerIcon, opponentIcon;
NSMutableArray *pieceImages = [[NSMutableArray alloc ] init];

bool isPawnUpgradeNeeded = NO;

ChessMob::Coord UpgradeCandidate = ChessMob::Coord(-1,-1);

ChessMob::Coord highlighted = ChessMob::Coord(-1,-1);

ChessMob::Coord topLeft = ChessMob::Coord(15,16); //left, down
ChessMob::Coord buttonSize = ChessMob::Coord(37, 36); //vert, horiz

NSString* CheckmateTitle = @"Checkmate";
NSString* StalemateTitle = @"Stalemate";
NSString* UpgradeTitle = @"Choose an Upgrade";

- (id) init {
	self = [super init];
	if (self != nil) {
		self.title = NSLocalizedString(@"Chess Mob",@"");
	}
	return self;
}

- (ChessMob::GameMaster*) getGameMaster{
	return gameMaster;
}
- (void) setGameMaster: (ChessMob::GameMaster*) gameMaster {
	highlightView.alpha = 0.0;
	highlighted.Row = -1;
	highlighted.Col = -1;
	if (self->gameMaster){
//        self->gameMaster->~GameMaster();
        self->gameMaster = nil;
	}
	self->gameMaster = gameMaster;
    undoButton.alpha = 1;
    [self setPlayerIcons];
	[self updateBoard];
	
}

- (void) setPlayerIcons{
    if (playerType > 0){ // White Piece
        [playerIcon setImage: [pieceImages objectAtIndex: 2 * playerType - 1 ]];
    } else { // Black Piece
        [playerIcon setImage: [pieceImages objectAtIndex: 2 * -playerType ]];
    }
         
    if (opponentType > 0){ // White Piece
        [opponentIcon setImage: [pieceImages objectAtIndex: 2 * opponentType - 1 ]];
    } else { // Black Piece
        [opponentIcon setImage: [pieceImages objectAtIndex: 2 * -opponentType ]];
    }
}


- (void) OnBoardButtonClick: (UIButton*) buttonClicked{
    
	int piece = self->gameMaster->GetPiece(ChessMob::Coord(buttonClicked.tag / 8, buttonClicked.tag % 8));
	if ((isWhiteTurn && piece > 0)|| (!isWhiteTurn && piece < 0)){	// If clicking on a piece on the current player's side, highlight the button, unless it's the same one.
		if (highlighted.Row == buttonClicked.tag % 8 && highlighted.Col == buttonClicked.tag / 8){
			highlighted.Row = -1; 
			highlighted.Col = -1; 
			highlightView.alpha = 0.0;
		} else{
			highlighted.Row = buttonClicked.tag % 8;
			highlighted.Col = buttonClicked.tag / 8;
			highlightView.alpha = 1.0;
			highlightView.frame = CGRectMake(topLeft.Row + highlighted.Row * buttonSize.Col,
											 topLeft.Col + highlighted.Col * buttonSize.Row,
											 buttonSize.Row, buttonSize.Col);
		}
	} else if (highlighted.Row != -1 && highlighted.Col !=-1) { // A piece is highlighted and they did not click on another of their peices
		//Try to make the move
		char temp = highlighted.Row;
		highlighted.Row = highlighted.Col;
		highlighted.Col = temp;


		if (gameMaster->MakeMove(ChessMob::Move(highlighted, ChessMob::Coord(buttonClicked.tag / 8, buttonClicked.tag % 8)))){
			isWhiteTurn= !isWhiteTurn;
            
			if (computerPlayerExists && !(gameMaster->IsInCheckMate() || gameMaster->IsStaleMate())){
                //[self updateBoard];
                //Put the following code threaded and only happen after any pawn selection.
                double delayInSeconds = 0.5;
                dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
                dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
                    //code to be executed on the main queue after delay
                    gameMaster->MakeComputerMove();
                    isWhiteTurn = !isWhiteTurn;
                    
                    //Dehighlight
                    highlighted.Row = -1;
                    highlighted.Col = -1;
                    highlightView.alpha = 0.0;
                    //update the board
                    [self updateBoard];
                });
			}
		}
        //Dehighlight
        highlighted.Row = -1;
        highlighted.Col = -1;
        highlightView.alpha = 0.0;
        //update the board
        [self updateBoard];

	}
}

-(void) makeComputerMove{
    if (!isPawnUpgradeNeeded){
        
    }
        
}

-(void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    if ([[alertView title] isEqualToString:CheckmateTitle]){
        switch (buttonIndex){
            case 0: //View board
                break;
            case 1: //Return to main menu, destroy game
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"sg"];
                //gameMaster->~GameMaster();
                gameMaster = nil;
                [rootViewController.navigationController popToRootViewControllerAnimated:YES];
                break;
        }
    } else if ([[alertView title] isEqualToString:UpgradeTitle]){ //Pawn upgrade
        switch (buttonIndex){
            case 0://Undo move
                gameMaster->UndoMove();
                break;
            case 1: //Queen
                gameMaster->ConvertPawn(UpgradeCandidate, ChessMob::QueenPiece);
                break;
            case 2:
                gameMaster->ConvertPawn(UpgradeCandidate, ChessMob::RookPiece);
                break;
            case 3:
                gameMaster->ConvertPawn(UpgradeCandidate, ChessMob::BishopPiece);
                break;
            case 4:
                gameMaster->ConvertPawn(UpgradeCandidate, ChessMob::KnightPiece);
                break;
        }
        isPawnUpgradeNeeded = NO;
        [self updateBoard];
    } else if ([[alertView title] isEqualToString:StalemateTitle]){
        switch (buttonIndex){
            case 0: //View board
                break;
            case 1: //Return to main menu, destroy game
  //              gameMaster->~GameMaster();
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"sg"];
                gameMaster = nil;
                [rootViewController.navigationController popToRootViewControllerAnimated:YES];
                break;
        }
    }
}

- (IBAction) OnUndoButtonClick: (id) sender{
	
	// Call Backend Undo function
	if (!computerPlayerExists){ // change whose turn it is, unless you're playing a computer.
		isWhiteTurn = !isWhiteTurn;
	}
	gameMaster->UndoMove();
	[self updateBoard];
}


// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
NSMutableArray* boardButtons = [[NSMutableArray alloc ] init];

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
	if (self) {
		// Custom initialization.
		[pieceImages addObject:[[UIImage alloc] init]]; // Blank image
		[pieceImages addObject:[UIImage imageNamed: @"king_white.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"king_black.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"queen_white.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"queen_black.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"rook_white.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"rook_black.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"bishop_white.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"bishop_black.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"knight_white.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"knight_black.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"pawn_white.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"pawn_black.png"]]; // Load queen image
		[pieceImages addObject:[UIImage imageNamed: @"SelectCircle.png"]]; // Load Select circle
		
		for (int i = 0; i < 64; i++){ // Create buttons
			UIButton * boardButton = [[UIButton buttonWithType: UIButtonTypeCustom] retain];
			boardButton.tag = i;
			boardButton.frame = CGRectMake(topLeft.Row + (i % 8) * buttonSize.Col, topLeft.Col + (i / 8) * buttonSize.Row, buttonSize.Row, buttonSize.Col);
			[boardButton addTarget: self action: @selector(OnBoardButtonClick:) forControlEvents: UIControlEventTouchUpInside ];
			[boardButton setImage: [pieceImages objectAtIndex:[pieceImages count]-1] forState: UIControlStateNormal]; // Set blank image
			[self.view addSubview: boardButton];
			[boardButtons addObject: boardButton];
		}
	}
	return self;
}
 

/*
 // Implement loadView to create a view hierarchy programmatically, without using a nib.
 - (void)loadView {
 }
 */

/*
 // Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
 - (void)viewDidLoad {
 [super viewDidLoad];
 }
 */

/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations.
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */

- (void)updateBoard{
    //Check for pawns that need to be upgraded
    //If they're pawns and reached the final row, pop up a dialog to choose.
    if (!isPawnUpgradeNeeded){ //Don't redisplay the popup
        for (int i = 0; i < 8;i++){
            if (gameMaster->GetPiece(ChessMob::Coord(0,i)) == ChessMob::PawnPiece){// Negative because black pieces are negative
                UpgradeCandidate.Col = i;
                UpgradeCandidate.Row = 0;
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:UpgradeTitle
                                                                message:nil
                                                               delegate:self
                                                      cancelButtonTitle:@"Undo"
                                                      otherButtonTitles:@"Queen", @"Rook",@"Bishop",@"Knight", nil];
                [alert show];
                [alert release];
                isPawnUpgradeNeeded = YES;
                break;
            }
            else if (- gameMaster->GetPiece(ChessMob::Coord(7,i)) == ChessMob::PawnPiece){
                UpgradeCandidate.Col = i;
                UpgradeCandidate.Row = 7;
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:UpgradeTitle
                                                                message:nil
                                                               delegate:self
                                                      cancelButtonTitle:@"Undo"
                                                      otherButtonTitles:@"Queen", @"Rook",@"Bishop",@"Knight", nil];
                [alert show];
                [alert release];
                isPawnUpgradeNeeded = YES;
                break;
            }
        }
    }
    //Update image of board
	for (int i = 0; i < 64; i++){
		int pieceTypeInt = gameMaster->GetPiece(ChessMob::Coord(i / 8, i % 8));
		if (pieceTypeInt > 0){ // White Piece
			[[boardButtons objectAtIndex:i] setImage: [pieceImages objectAtIndex: 2 * pieceTypeInt - 1 ] forState: UIControlStateNormal];			
		} else { // Black Piece
			pieceTypeInt = - pieceTypeInt;
			[[boardButtons objectAtIndex:i] setImage: [pieceImages objectAtIndex: 2 * pieceTypeInt ] forState: UIControlStateNormal];
		}
	}
    
    //Check if they are in check mate and display a message, then end the game.
    if (gameMaster->IsInCheckMate()){
        
        NSString * color;
        if (isWhiteTurn){
            color = @"White";
        } else{
            color = @"Black";
        }
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:CheckmateTitle
                                                        message: [color stringByAppendingString: @" is in checkmate." ]
                                                       delegate:self
                                              cancelButtonTitle:@"View Board"
                                              otherButtonTitles:@"Main Menu", nil];
        [alert show];
        [alert release];
        undoButton.alpha = 0;
    }
    //check if they are in check and display an alert.
    else if (gameMaster->IsInCheck()){
        
        NSString * color;
        if (isWhiteTurn){
            color = @"White";
        } else{
            color = @"Black";
        }
        [[self view] makeToast: [color stringByAppendingString: @" is in check"]];
    }
    else if (gameMaster->IsStaleMate()){
        NSString * color;
        if (isWhiteTurn){
            color = @"White";
        } else{
            color = @"Black";
        }
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:StalemateTitle
                                                        message: [color stringByAppendingString: @" is in stalemate." ]
                                                       delegate:self
                                              cancelButtonTitle:@"View Board"
                                              otherButtonTitles:@"Main Menu", nil];
        [alert show];
        [alert release];
        undoButton.alpha = 0;
    }
}

- (void)viewDidDisappear:(BOOL)animated {
	[super viewDidDisappear:animated];
    [self saveGame];
}


- (void)saveGame {
    if (gameMaster){
        //Save Game
        
        NSMutableArray* mhArray = [[NSMutableArray alloc] init];
        std::deque<ChessMob::Move> mh = gameMaster->GetHistory();
        
        //Convert from deque to NSMutableArray
        while (mh.size() > 0){
            ChessMob::Move move = mh.front();
            mh.pop_front();
            [mhArray addObject:[NSData dataWithBytes:&move length: sizeof(ChessMob::Move)]];
        }
        
        ChessMob::PlayerType whitePlayerType = ChessMob::HumanPlayer, blackPlayerType = ChessMob::HumanPlayer;
        if (computerPlayerExists){
            if (isWhiteTurn) { // Computer player is black, human is white
                blackPlayerType = ChessMob::ComputerPlayer;
            } else{
                whitePlayerType = ChessMob::ComputerPlayer;
            }
                
        }
        
        //Create SG
        SaveGameObject* sg = [[[SaveGameObject alloc] initWithWhitePlayerType: whitePlayerType AndBlackPlayerType:blackPlayerType AndPlayerPieceType:playerType AndOpponentPieceType:opponentType AndBoard:gameMaster->Board AndMoveHistory: mhArray AndIsWhiteTurn:isWhiteTurn] retain];
        
        //Store Data
        NSData *data = [NSKeyedArchiver archivedDataWithRootObject:sg];
        [[NSUserDefaults standardUserDefaults] setObject:data forKey:@"sg"];
        
    }

}

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
