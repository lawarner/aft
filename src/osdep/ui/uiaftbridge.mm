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
@property NSButton   *cancelButton;
@property NSString   *inputText;
@property void       *data;

@property float windowHeight;
@property float windowWidth;

- (BOOL)addFieldWithPrompt:(NSString *)prompt andData:(void *)data;

- (BOOL)outputField:(NSString *)data;

- (BOOL)setup;

- (void)setWindow:(NSWindow *)window;

@end

@implementation UiAftCocoaData {
    NSWindow *mainWindow;
}

//TODO andFacets:
- (BOOL)addFieldWithPrompt:(NSString *)prompt andData:(void *)data {
    constexpr float TextHeight = 22;
    constexpr float TextWidth  = 200;
    float wh = self.windowHeight;
    float ww = self.windowWidth;

    self.labelView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, wh - TextHeight, TextWidth, TextHeight)];
    [self.labelView insertText:prompt replacementRange:NSMakeRange(0, 1)];
    self.labelView.editable = NO;
    self.editView = [[NSTextView alloc] initWithFrame:NSMakeRect(TextWidth + 2, wh - TextHeight, 100, TextHeight)];
    self.editView.editable = YES;
    self.data = data;

    NSView *mainView = mainWindow.contentView;
    [mainView addSubview:self.labelView];
    [mainView addSubview:self.editView];

    return YES;
}

- (BOOL)outputField:(NSString *)data {
    [self.editView setString:data];
    return YES;
}

- (BOOL)setup {
    NSRect rect = mainWindow.contentLayoutRect;
    self.windowHeight = rect.size.height;
    self.windowWidth  = rect.size.width;
    NSLog(@"Window rect is (%f, %f, %f, %f)", rect.origin.x, rect.origin.y, self.windowWidth, self.windowHeight);

    auto midWidth = self.windowWidth / 2;
    self.okButton = [[NSButton alloc] initWithFrame:NSMakeRect(midWidth - 30, 2, 30, 28)];
    [self.okButton setTitle:@"OK"];
    [self.okButton setTarget:self];
    [self.okButton setAction:@selector(handleOkButton:)];
    self.cancelButton = [[NSButton alloc] initWithFrame:NSMakeRect(midWidth + 18, 2, 50, 28)];
    [self.cancelButton setTitle:@"Cancel"];
    [self.cancelButton setTarget:self];
    [self.cancelButton setAction:@selector(handleCancelButton:)];

    NSView *mainView = [[NSView alloc] init];
    [mainView addSubview:self.okButton];
    [mainView addSubview:self.cancelButton];
    [mainWindow setContentView:mainView];
    return YES;
}

- (void)setWindow:(NSWindow *)window {
    mainWindow = window;
}

- (void)showValue:(NSString *)value {
    [self.editView setString:value];
}

- (void)handleOkButton:(NSButton *)sender {
    self.inputText = [[self.editView textStorage] string];
    NSLog(@"OK already %@", self.inputText);
    Element* element = (Element *)self.data;
    NSUInteger inputSize = [self.inputText length];
    char chInput[inputSize + 1];
    if ([self.inputText getCString:chInput maxLength:sizeof(chInput) encoding:NSASCIIStringEncoding]) {
        element->setValue(chInput);
    }
    element->unblock();
}

- (void)handleCancelButton:(NSButton *)sender {
    NSLog(@"Goodbye");
    NSApplication *app = [NSApplication sharedApplication];
    [app terminate:self];
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
    return [cocoaData addFieldWithPrompt:prompt andData:element] == YES;
}

bool UiAftBridge::outputElement(Element* element) {
    return false;
}

int UiAftBridge::run() {
    // Run in a thread so the ui is not blocked
    if (cppMain_) {
        const char *args[] = { "AftCocoa" };
        constexpr int numArgs = sizeof(args) / sizeof(args[0]);
        mainThread_ = std::async(std::launch::async, cppMain_, numArgs, args);
        return 0;
    }
    return -1;
}

bool UiAftBridge::setup() {
    bool ret = [cocoaData setup] == YES;
    return ret;
}

void UiAftBridge::setValue(Element* element) {
    const char* cstr = element->getValue().c_str();
    NSString *value = [NSString stringWithUTF8String:cstr];
    
}

void UiAftBridge::setCppMain(void* cppMainFunction) {
    cppMain_ = (CppMainFunction)cppMainFunction;
}
