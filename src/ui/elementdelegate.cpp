/*
 *  elementdelegate.cpp
 *  libaft
 *
 *  Copyright © 2017 Andy Warner. All rights reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <iostream>
#include <sstream>
#include "element.h"
#include "elementdelegate.h"
using namespace aft::ui;
using namespace std;

BaseElementDelegate::BaseElementDelegate() {
    
}

bool BaseElementDelegate::getFocus(Element* element) const {
    return false;
}

bool BaseElementDelegate::setFocus(Element* element, bool hasFocus) {
    return false;
}

bool BaseElementDelegate::getFacet(Element* element, const std::string& name,
                                   UIFacetCategory category, UIFacet& facet) const {
    return false;
}

bool BaseElementDelegate::setFacet(Element* element, const UIFacet& facet) {
    return false;
}

bool BaseElementDelegate::input(Element* element, std::string& value) {
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

bool BaseElementDelegate::output(Element* element) {
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
