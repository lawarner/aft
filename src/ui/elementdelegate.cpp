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

void BaseElementDelegate::flush(const Element* element) {

}

const std::string&
BaseElementDelegate::getValue(const Element* element) const {
    if (element->getValue().empty()) {
        return element->getDefault();
    }
    return element->getValue();
}

void BaseElementDelegate::setValue(Element* element, const std::string& value) {
    element->setValue(value);
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
    element->apply(facet);
    return true;
}

bool BaseElementDelegate::input(const Element* element, std::string& value) {
    value = element->getValue();
    return true;
}

bool BaseElementDelegate::output(const Element* element, bool showValue) {
    return true;
}
