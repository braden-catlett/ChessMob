//
//  SaveGameObject.m
//  ChessMob_iOS
//
//  Created by Michael Weidemann on 8/15/13.
//
//

#import "SaveGameObject.h"
#import "GameMaster.h"

@implementation SaveGameObject

- (id) initWithWhitePlayerType: (ChessMob::PlayerType) wPlayerType
            AndBlackPlayerType: (ChessMob::PlayerType) bPlayerType
            AndPlayerPieceType: (int) pType
          AndOpponentPieceType: (int) oType
                      AndBoard: (ChessMob::Piece*[8][8]) theBoard
                AndMoveHistory: (NSMutableArray*) theMoveHistory
                AndIsWhiteTurn: (BOOL) isWTurn{
    self = [super init];
    if (self){
        playerType = pType;
        opponentType = oType;
        whitePlayerType = wPlayerType;
        blackPlayerType = bPlayerType;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                board[i][j] = theBoard[i][j];
            }
        }
        moveHistory = theMoveHistory;
        isWhiteTurn = isWTurn;
    }
    return self;
}

- (void) encodeWithCoder:(NSCoder*)encoder {
    // If parent class also adopts NSCoding, include a call to
    // [super encodeWithCoder:encoder] as the first statement.
    // Encode board
    
    NSString* boardString = @"";
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (board[i][j] != NULL){
                int type =board[i][j]->GetType();
                if (!board[i][j]->IsWhitePiece()){
                    type = -type;
                }
                boardString = [boardString stringByAppendingString: [NSString stringWithFormat:@"%d",   type]];
            }
            else{
                boardString = [boardString stringByAppendingString:@"0"];
            }
        }
    }
    
    [encoder encodeObject:boardString forKey:@"board"];
    [encoder encodeObject:moveHistory forKey:@"moveHistory"];
    [encoder encodeInteger:playerType forKey:@"playerType"];
    [encoder encodeInteger:opponentType forKey:@"opponentType"];
    [encoder encodeInteger:whitePlayerType forKey:@"whitePlayerType"];
    [encoder encodeInteger:blackPlayerType forKey:@"blackPlayerType"];
    [encoder encodeBool:isWhiteTurn forKey:@"isWhiteTurn"];
}

- (id) initWithCoder:(NSCoder*)decoder {
    if (self = [super init]) {
        // If parent class also adopts NSCoding, replace [super init]
        // with [super initWithCoder:decoder] to properly initialize.
        
        // NOTE: Decoded objects are auto-released and must be retained
        //Board setup
        NSString* boardString = [[decoder decodeObjectForKey:@"board"] retain];
        //Translate it back into pieces
        
        for (int stringIndex = 0, boardIndex = 0; boardIndex < 64; stringIndex++, boardIndex++){
            switch ([boardString characterAtIndex:stringIndex]){
                case '0': //No piece
                    board[boardIndex/8][boardIndex%8] = NULL;
                    break;
                case '1':// White King
                    board[boardIndex/8][boardIndex%8] = new ChessMob::King(YES);
                    break;
                case '2'://White Queen
                    board[boardIndex/8][boardIndex%8] = new ChessMob::Queen(YES);
                    break;
                case '3'://White Rook
                    board[boardIndex/8][boardIndex%8] = new ChessMob::Rook(YES);
                    break;
                case '4'://White Bishop
                    board[boardIndex/8][boardIndex%8] = new ChessMob::Bishop(YES);
                    break;
                case '5'://White Knight
                    board[boardIndex/8][boardIndex%8] = new ChessMob::Knight(YES);
                    break;
                case '6'://White Pawn
                    board[boardIndex/8][boardIndex%8] = new ChessMob::Pawn(YES);
                    break;
                case '-': //Black
                    stringIndex++;
                    switch ([boardString characterAtIndex:stringIndex]){
                        case '1'://Black King
                            board[boardIndex/8][boardIndex%8] = new ChessMob::King(NO);
                            break;
                        case '2'://Black Queen
                            board[boardIndex/8][boardIndex%8] = new ChessMob::Queen(NO);
                            break;
                        case '3'://Black Rook
                            board[boardIndex/8][boardIndex%8] = new ChessMob::Rook(NO);
                            break;
                        case '4'://Black Bishop
                            board[boardIndex/8][boardIndex%8] = new ChessMob::Bishop(NO);
                            break;
                        case '5'://Black Knight
                            board[boardIndex/8][boardIndex%8] = new ChessMob::Knight(NO);
                            break;
                        case '6'://Black Pawn
                            board[boardIndex/8][boardIndex%8] = new ChessMob::Pawn(NO);
                            break;
                    }
                    
            }
        }
        
        [boardString release];

        //History setup
        moveHistory = [(NSMutableArray*) [decoder decodeObjectForKey:@"moveHistory"] retain];
        
        whitePlayerType = (ChessMob::PlayerType)[decoder decodeIntegerForKey:@"whitePlayerType"];
        blackPlayerType = (ChessMob::PlayerType)[decoder decodeIntegerForKey:@"blackPlayerType"];
        playerType = (int)[decoder decodeIntegerForKey:@"playerType"];
        opponentType = (int)[decoder decodeIntegerForKey:@"opponentType"];
        isWhiteTurn = (ChessMob::PlayerType)[decoder decodeBoolForKey:@"isWhiteTurn"];
        
    }
    return self;
}


@end
