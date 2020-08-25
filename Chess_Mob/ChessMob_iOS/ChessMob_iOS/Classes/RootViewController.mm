//
//  RootViewController.m
//  ChessMob_iOS
//
//  Created by Michael A Weidemann on 5/2/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import "RootViewController.h"
#import "GameScreenController.h"
#import "SettingsMenuController.h"
#import "SaveGameObject.h"
#import "GameMaster.h"

@implementation RootViewController

@synthesize views, resumeButton;

#pragma mark -
#pragma mark View lifecycle

- (void) awakeFromNib{
	views = [ [NSMutableArray alloc] init];
	
	SettingsMenuController *gameOptions = [[SettingsMenuController alloc] init];
	gameOptions.rootViewController = self;
	gameOptions.title = @"Game Options";
	[views addObject:[NSDictionary dictionaryWithObjectsAndKeys:
					  @"Game Options",		@"title",
					  gameOptions,			@"controller",
					  nil] ];
	[gameOptions release];
	
	GameScreenController *gameScreen = [[GameScreenController alloc] init];
    gameScreen.rootViewController = self;
	gameScreen.title = @"Chess Mob";
	[views addObject:[NSDictionary dictionaryWithObjectsAndKeys:
					  @"Chess Mob",		@"title",
					  gameScreen,		@"controller",
					  nil] ];
	[gameScreen release];
	
	self.title = @"Main Menu";
}

-(void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    switch (buttonIndex){
        case 0: //Cancel
            break;
        case 1: //Go to settings screen
            break;
    }
}

- (void) StartGameWithGameMaster: (ChessMob::GameMaster*) gameMaster withComputer: (bool) hasComputer computerGoesFirst: (bool) compGoesFirst playerType: (int) ptype opponentType: (int) otype{
    
	GameScreenController *gameScreen = [[views objectAtIndex:1] objectForKey:@"controller"];
    NSLog([NSString stringWithFormat:@"%d compgoesfirst",compGoesFirst]);
	if (compGoesFirst){
		gameMaster->MakeComputerMove();
		gameScreen.isWhiteTurn = NO;
	} else {
		gameScreen.isWhiteTurn = YES;
	}
    gameScreen.playerType = ptype;
    gameScreen.opponentType = otype;
	[gameScreen setGameMaster: gameMaster];
	gameScreen.computerPlayerExists = hasComputer;
	[self ChangeView:@"GameScreen"];
}

- (void) ResumeGameWithGameMaster: (ChessMob::GameMaster*) gameMaster withComputer: (bool) hasComputer isWhiteTurn: (bool) isWhiteTurn playerType: (int) ptype opponentType: (int) otype{
    
	GameScreenController *gameScreen = [[views objectAtIndex:1] objectForKey:@"controller"];
    gameScreen.isWhiteTurn = isWhiteTurn;
    gameScreen.playerType = ptype;
    gameScreen.opponentType = otype;
	[gameScreen setGameMaster: gameMaster];
	gameScreen.computerPlayerExists = hasComputer;
	[self ChangeView:@"GameScreen"];
}


- (void) ChangeView: (NSString*) newView{
	if ([newView isEqualToString:@"GameScreen"]){
		[[self navigationController] popViewControllerAnimated:NO];
		UIViewController *targetViewController = [[views objectAtIndex:1] objectForKey:@"controller"];
		[[self navigationController] pushViewController:targetViewController animated:YES];
	}
}

- (IBAction) OnStartButtonClick:(id) sender{
	UIViewController *targetViewController = [[views objectAtIndex: 0] objectForKey:@"controller"];
    [[self navigationController] pushViewController:targetViewController animated:YES];
}

- (IBAction)OnResumeButtonClick:(id)sender{
    //[[NSUserDefaults standardUserDefaults] removeObjectForKey:@"sg"];

    //Load info from saved file
    NSData* sgData = [[NSUserDefaults standardUserDefaults] objectForKey:@"sg"];
    if (sgData){
        SaveGameObject* loadedGame = [[NSKeyedUnarchiver unarchiveObjectWithData:sgData] retain];
        
        //Create move history deque from Array
        std::deque<ChessMob::Move> moveHistoryDeque;
        for (int i = 0; i < [loadedGame->moveHistory count]; i++){
            ChessMob::Move move;
            [[loadedGame->moveHistory objectAtIndex:i ] getBytes:&move length: [[loadedGame->moveHistory objectAtIndex:i] length]];
            moveHistoryDeque.push_back(move);
        }
            
        ChessMob::GameMaster* loadedGm = new ChessMob::GameMaster(loadedGame->whitePlayerType, loadedGame->blackPlayerType, loadedGame->board, moveHistoryDeque, loadedGame->isWhiteTurn,[[NSUserDefaults standardUserDefaults] integerForKey: @"difficulty"] );

        bool hasComputer = NO;
        if (loadedGame->whitePlayerType == ChessMob::ComputerPlayer || loadedGame->blackPlayerType == ChessMob::ComputerPlayer){
            hasComputer = YES;
        }
        //Switch to game screen
        [self ResumeGameWithGameMaster:loadedGm withComputer:hasComputer isWhiteTurn:loadedGame->isWhiteTurn playerType:loadedGame->playerType opponentType:loadedGame->opponentType];
    }
}


- (void)viewDidLoad {
    [super viewDidLoad];
    [chessmobtitle setFont:[UIFont fontWithName:@"UnifrakturMaguntia" size:45]];
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

/*
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}
*/

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    //Uncomment to delete save game key
    //[[NSUserDefaults standardUserDefaults] removeObjectForKey:@"sg"];
    
    //Check if saved game is present. Deactivate resume button if not.
    if ([[NSUserDefaults  standardUserDefaults] objectForKey:@"sg"] == nil){
        resumeButton.enabled = NO;
        resumeButton.alpha = 0;
    } else{
        resumeButton.enabled = YES;
        resumeButton.alpha = 1;
    }
}

/*
- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
}
*/
/*
- (void)viewDidDisappear:(BOOL)animated {
	[super viewDidDisappear:animated];
}
*/

/*
 // Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations.
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
 */


#pragma mark -
#pragma mark Table view data source
/*
// Customize the number of sections in the table view.
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [views count];
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	// Configure the cell.
	cell.text = [[views objectAtIndex:indexPath.row] objectForKey:@"title"];
    return cell;
}
*/

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/


/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source.
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view.
    }   
}
*/


/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/


/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/


#pragma mark -
#pragma mark Table view delegate
/*
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	UIViewController *targetViewController = [[views objectAtIndex: indexPath.row] objectForKey:@"controller"];
    [[self navigationController] pushViewController:targetViewController animated:YES];

	/*
	 <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
	 [self.navigationController pushViewController:detailViewController animated:YES];
	 [detailViewController release];
	 
}
*/

#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}


- (void)dealloc {
	[views dealloc];
    [super dealloc];
}


@end

