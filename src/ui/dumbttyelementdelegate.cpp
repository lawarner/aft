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

bool DumbTtyElementDelegate::input(Element* element, std::string& value) {
    string defValue = element->getDefault();
    if (!defValue.empty()) {
        defValue = " [" + defValue + "]";
    }
    std::cout << element->getPrompt() << defValue << ": " << std::flush;
    char cname[100];
    std::cin.getline(cname, sizeof(cname));
    std::string name(cname, strlen(cname));
    if (name.empty()) {
        value = element->getDefault();
    }
    else {
        value = name;
    }
    return true;
}

bool DumbTtyElementDelegate::output(const Element* element) {
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
    oss << ": " << element->getValue();
    cout << oss.str() << endl;
    return true;
}
