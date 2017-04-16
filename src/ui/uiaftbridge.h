#pragma once
//
//  uiaftbridge.h
//  libaft
//
//  Created by Lynn Warner on 4/23/17.
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface UiAftBridge : NSObject

- (instancetype)initWithWindow:(NSWindow *)window;

- (int)run;

- (BOOL)setup;

- (void)setCppMain:(void *)cppMainFunction;

@end
