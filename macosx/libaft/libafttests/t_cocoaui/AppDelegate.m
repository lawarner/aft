//
//  AppDelegate.m
//  t_cocoaui
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#import "AppDelegate.h"
#import "cppmain.hpp"

#import <ui/uiaftbridge.h>

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;

@property UiAftBridge *uiaft;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.uiaft = [[UiAftBridge alloc] initWithWindow:self.window];
    [self.uiaft setCppMain:CppMain];
    [self.uiaft setup];
    [self.uiaft run];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
