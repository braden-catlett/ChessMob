//
//  SaveGameObject.h
//  ChessMob_iOS
//
//  Created by Michael Weidemann on 8/15/13.
//  Copyright 2013 Hubbell Games. All rights reserved.
//
//

#import <Foundation/Foundation.h>
#import "Structs.h"
#import "Piece.h"


@interface SaveGameObject : NSObject <NSCoding>{
@public
    ChessMob::PlayerType whitePlayerType;
    ChessMob::PlayerType blackPlayerType;
    int playerType;
    int opponentType;
    ChessMob::Piece* board[8][8];
    //History
    NSMutableArray* moveHistory;
    bool isWhiteTurn;
}
- (id) initWithWhitePlayerType: (ChessMob::PlayerType) wPlayerType
            AndBlackPlayerType: (ChessMob::PlayerType) bPlayerType
            AndPlayerPieceType: (int) pType
          AndOpponentPieceType: (int) oType
                      AndBoard: (ChessMob::Piece*[8][8]) theBoard
                AndMoveHistory: (NSMutableArray*) theMoveHistory
                AndIsWhiteTurn: (BOOL) isWTurn;

@end
