//
//  uiaftbridge.mm
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#import "uiaftbridge.h"

typedef int (*CppMainFunction)(int argc, const char*argv[]);

@interface UiAftBridge ()

@property NSWindow *window;
//@property CppMainFunction cppMain;

@property NSTextView *labelView;
@property NSTextView *editView;
@property NSButton   *okButton;

@end

@implementation UiAftBridge {
    CppMainFunction cppMain;
}

- (instancetype)initWithWindow:(NSWindow *)window {
    self = [super init];
    if (self) {
        _window = window;
    }
    return self;
}

//TODO run in a thread so the ui is not blocked
- (void)run {
    if (cppMain) {
        const char *args[] = { "AftCocoa" };
        constexpr int numArgs = sizeof(args) / sizeof(args[0]);
        cppMain(numArgs, args);
    }
}

- (BOOL)setup {
    NSRect rect = self.window.contentLayoutRect;
    float wh = rect.size.height;
    float ww = rect.size.width;
    NSLog(@"Window rect is (%f, %f, %f, %f)", rect.origin.x, rect.origin.y, ww, wh);
    
    self.okButton = [[NSButton alloc] initWithFrame:NSMakeRect(ww / 2, 2, 30, 28)];
    [self.okButton setTitle:@"OK"];
    [self.okButton setTarget:self];
    [self.okButton setAction:@selector(handleOkButton:)];
    
    float textHeight = 22;
    self.labelView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, wh - textHeight, 100, textHeight)];
    [self.labelView insertText:@"Hello there, aft!" replacementRange:NSMakeRange(0, 1)];
    self.labelView.editable = NO;
    self.editView = [[NSTextView alloc] initWithFrame:NSMakeRect(102, wh - textHeight, 100, textHeight)];
    self.editView.editable = YES;
    
    NSView *mainView = [[NSView alloc] init];
    [mainView addSubview:self.labelView];
    [mainView addSubview:self.editView];
    [mainView addSubview:self.okButton];
    [self.window setContentView:mainView];
    return YES;
}

- (void)setCppMain:(void *)cppMainFunction {
    cppMain = (CppMainFunction)cppMainFunction;
}

- (void)handleOkButton:(NSButton *)sender {
    NSString *inputText = [[self.editView textStorage] string];
    NSLog(@"OK already %@", inputText);
    NSApplication *app = [NSApplication sharedApplication];
    [app terminate:self];
}

@end
