//
//  cocoaelementdelegate.cpp
//  libaft
//
//  Created by Lynn Warner on 7/3/17.
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "cocoaelementdelegate.h"
#include "element.h"
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
    
bool CocoaElementDelegate::input(const Element* element, std::string& value) {
    return false;
}

bool CocoaElementDelegate::output(const Element* element, bool showValue) {
    return false;
}
#if 0
    /** Output the element to the user interface.
     *
     *  The various output formats for an element are:
     *  Fully specified:   <element_name>: <prompt> [<default_value>]? value
     *  Default missing:   <element_name>: <prompt>?
     *  Prompt missing:    <element_name> [<default_value>]?
     *  No Prompt,Default: <element_name>?
     */
    bool DumbTtyElementDelegate::input(const Element* element, std::string& value) {
        output(element);
        char cinbuf[100];
        std::cin.getline(cinbuf, sizeof(cinbuf));
        size_t strLen = strnlen(cinbuf, sizeof(cinbuf));
        if (strLen > 0) {
            value = std::string(cinbuf, strlen(cinbuf));
        }
        else {
            value = element->getDefault();
        }
        return true;
    }
    
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
