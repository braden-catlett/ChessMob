//
//  C__ObjCTestAppDelegate.h
//  C++ObjCTest
//
//  Created by Michael A Weidemann on 5/7/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class C__ObjCTestViewController;

@interface C__ObjCTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    C__ObjCTestViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet C__ObjCTestViewController *viewController;

@end

