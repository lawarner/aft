#pragma once
//
//  uiaftbridge.h
//  libaft
//
//  Created by Lynn Warner on 4/23/17.
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "ui/element.h"
#include <future>

namespace aft {
namespace ui {

class UiAftBridge {
private:
    UiAftBridge();
    virtual ~UiAftBridge() = default;
public:
    typedef int (*CppMainFunction)(int argc, const char*argv[]);

    static UiAftBridge* instance();
    static void setWindow(void* window);

    bool addElement(Element* element);
    int run();
    bool setup();
    void setCppMain(void* cppMainFunction);
private:
    CppMainFunction cppMain_;
    std::future<int> mainThread_;
    static UiAftBridge* instance_;
};
    
} // namespace ui
} // namespace aft

#if 0
#import <Cocoa/Cocoa.h>

@interface UiAftBridge : NSObject

- (instancetype)initWithWindow:(NSWindow *)window;

- (int)run;

- (BOOL)setup;

- (void)setCppMain:(void *)cppMainFunction;

@end
#endif
