//
//  cocoaelementdelegate.cpp
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "cocoaelementdelegate.h"
#include "uiaftbridge.h"
#include "ui/element.h"
#include <chrono>
#include <thread>

using namespace aft::osdep;
using namespace aft::ui;

void CocoaElementDelegate::flush(const Element* element) {
    
}

const std::string&
CocoaElementDelegate::getValue(const Element* element) const {
    return element->getValue();
}

void CocoaElementDelegate::setValue(Element* element, const std::string& value) {
    element->setValue(value);
}

bool CocoaElementDelegate::getFocus(Element* element) const {
    return false;
}

bool CocoaElementDelegate::setFocus(Element* element, bool hasFocus) {
    return false;
}

bool CocoaElementDelegate::getFacet(Element* element, const std::string& name, UIFacetCategory category,
                                    UIFacet& facet) const {
    return false;
}

bool CocoaElementDelegate::setFacet(Element* element, const UIFacet& facet) {
    return false;
}
    
bool CocoaElementDelegate::input(Element* element, std::string& value) {
    element->block();
    while (element->isBlocked()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    value = element->getValue();
    if (value.empty()) {
        value = element->getDefault();
    }
    return true;
}

bool CocoaElementDelegate::output(const Element* element, bool showValue) {
    UiAftBridge* uiaft = UiAftBridge::instance();
    return uiaft->outputElement(const_cast<Element *>(element));
}
#if 0
    bool DumbTtyElementDelegate::output(const Element* element, bool showValue) {
        ostringstream oss;
        string prompt = element->getPrompt();
        if (prompt.empty()) {
            oss << "Element value";
        }
        else {
            oss << prompt;
        }
        string defValue = element->getDefault();
        if (!defValue.empty()) {
            oss << " [" << defValue << "]";
        }
        oss << ": ";
        if (showValue && element->hasValue()) {
            oss << element->getValue() << " ";
        }
        cout << oss.str() << std::flush;
        return true;
    }
#endif
