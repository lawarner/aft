//
//  dumbttyelementdelegate.cpp
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "dumbttyelementdelegate.h"

#include <iostream>
#include <sstream>
#include "element.h"
#include "elementdelegate.h"
using namespace aft::ui;
using namespace std;

DumbTtyElementDelegate::DumbTtyElementDelegate() {
    
}

void DumbTtyElementDelegate::flush(const Element* element) {
    cout << endl;
}

const std::string&
DumbTtyElementDelegate::getValue(const Element* element) const {
    return element->getValue();
}

void DumbTtyElementDelegate::setValue(Element* element, const std::string& value) {
    element->setValue(value);
}

bool DumbTtyElementDelegate::getFocus(Element* element) const {
    return false;
}

bool DumbTtyElementDelegate::setFocus(Element* element, bool hasFocus) {
    //TODO probably just reprompt the element
    return false;
}

bool DumbTtyElementDelegate::getFacet(Element* element, const std::string& name,
                                   UIFacetCategory category, UIFacet& facet) const {
    return false;
}

bool DumbTtyElementDelegate::setFacet(Element* element, const UIFacet& facet) {
    return false;
}

/** Output the element to the user interface.
 *
 *  The various output formats for an element are:
 *  Fully specified:   <element_name>: <prompt> [<default_value>]? value
 *  Default missing:   <element_name>: <prompt>?
 *  Prompt missing:    <element_name> [<default_value>]?
 *  No Prompt,Default: <element_name>?
 */
bool DumbTtyElementDelegate::input(Element* element, std::string& value) {
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
