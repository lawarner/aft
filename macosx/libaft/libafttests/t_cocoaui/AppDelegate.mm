//
//  AppDelegate.m
//  t_cocoaui
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#import "AppDelegate.h"
#import "cppmain.hpp"

#import <osdep/ui/uiaftbridge.h>
using namespace aft::ui;

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;

@property aft::ui::UiAftBridge *uiaft;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    UiAftBridge* uiaft = UiAftBridge::instance();
    uiaft->setWindow((void *)CFBridgingRetain(self.window));
    uiaft->setCppMain((void *)CppMain);
    uiaft->setup();
    uiaft->run();
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
