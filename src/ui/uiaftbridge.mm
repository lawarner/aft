//
//  uiaftbridge.mm
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "uiaftbridge.h"
using namespace aft::ui;

UiAftBridge* UiAftBridge::instance_ = nullptr;

@interface UiAftCocoaData : NSObject

@property NSTextView *labelView;
@property NSTextView *editView;
@property NSButton   *okButton;
@property NSString   *inputText;

@property float windowHeight;
@property float windowWidth;

- (BOOL)addFieldWithPrompt:(NSString *)prompt;

- (BOOL)setup;

- (void)setWindow:(NSWindow *)window;

@end

@implementation UiAftCocoaData {
    NSWindow *mainWindow;
}

- (BOOL)addFieldWithPrompt:(NSString *)prompt {
    float textHeight = 22;
    float wh = self.windowHeight;
    float ww = self.windowWidth;

    self.labelView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, wh - textHeight, 100, textHeight)];
    [self.labelView insertText:prompt replacementRange:NSMakeRange(0, 1)];
    self.labelView.editable = NO;
    self.editView = [[NSTextView alloc] initWithFrame:NSMakeRect(102, wh - textHeight, 100, textHeight)];
    self.editView.editable = YES;
    
    NSView *mainView = mainWindow.contentView;
    [mainView addSubview:self.labelView];
    [mainView addSubview:self.editView];
    //[mainWindow display];
    return YES;
}

- (BOOL)setup {
    NSRect rect = mainWindow.contentLayoutRect;
    self.windowHeight = rect.size.height;
    self.windowWidth  = rect.size.width;
    NSLog(@"Window rect is (%f, %f, %f, %f)", rect.origin.x, rect.origin.y, self.windowWidth, self.windowHeight);

    self.okButton = [[NSButton alloc] initWithFrame:NSMakeRect(self.windowWidth / 2, 2, 30, 28)];
    [self.okButton setTitle:@"OK"];
    [self.okButton setTarget:self];
    [self.okButton setAction:@selector(handleOkButton:)];

    NSView *mainView = [[NSView alloc] init];
    [mainView addSubview:self.okButton];
    [mainWindow setContentView:mainView];
    return YES;
}

- (void)setWindow:(NSWindow *)window {
    mainWindow = window;
}

- (void)handleOkButton:(NSButton *)sender {
    self.inputText = [[self.editView textStorage] string];
    NSLog(@"OK already %@", self.inputText);
    //NSApplication *app = [NSApplication sharedApplication];
    //[app terminate:self];
}

@end

static UiAftCocoaData *cocoaData = 0;

UiAftBridge::UiAftBridge() {
    cocoaData = [[UiAftCocoaData alloc] init];
}

UiAftBridge* UiAftBridge::instance() {
    if (nullptr == instance_) {
        instance_ = new UiAftBridge;
    }
    return instance_;
}

void UiAftBridge::setWindow(void* wind) {
    [cocoaData setWindow:(__bridge NSWindow *)wind];
}

bool UiAftBridge::addElement(Element* element) {
    const char* cstr = element->getPrompt().empty() ? "Enter value:" : element->getPrompt().c_str();
    NSString *prompt = [NSString stringWithUTF8String:cstr];
    return [cocoaData addFieldWithPrompt:prompt] == YES;
}

int UiAftBridge::run() {
    //TODO run in a thread so the ui is not blocked
    if (cppMain_) {
        const char *args[] = { "AftCocoa" };
        constexpr int numArgs = sizeof(args) / sizeof(args[0]);
        return cppMain_(numArgs, args);
    }
    return -1;
}

bool UiAftBridge::setup() {
    bool ret = [cocoaData setup] == YES;
    return ret;
}

void UiAftBridge::setCppMain(void* cppMainFunction) {
    cppMain_ = (CppMainFunction)cppMainFunction;
}


#if 0
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
#endif
