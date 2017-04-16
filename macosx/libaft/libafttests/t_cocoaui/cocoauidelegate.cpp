/*
 *  cocoauidelegate.cpp
 *  libafttests
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
#include <memory>
#include "cocoauidelegate.h"
#include "ui/element.h"
#include "ui/uidelegate.h"
#include "core/logger.h"

using namespace aft::base;
using namespace aft::core;
using namespace aft::ui;
using namespace std;

CocoaUIDelegate::CocoaUIDelegate()
: root_(nullptr) {
    
}

bool CocoaUIDelegate::add(const Element& element) {
    if (root_ == nullptr) {
        root_ = std::make_unique<Element>(element);
        return true;
    }
    return false;
}

bool CocoaUIDelegate::focus(const Element& element) {
    return false;
}

const Element*
CocoaUIDelegate::get(const std::string& name) {
    if (root_ != nullptr && root_->getName() == name) {
        return root_.get();
    }
    return nullptr;
}

bool CocoaUIDelegate::hide(const Element& element) {
    return false;
}

bool CocoaUIDelegate::input(const Element& element, std::string& value) {
    char cname[200];
    std::cin.getline(cname, sizeof(cname));
    std::string name(cname, strlen(cname));
    value = name.empty() ? element.getDefault() : name;
    
    return true;
}

/** Output the element to the user interface.
 *
 *  The various output formats for an element are:
 *  Fully specified:   <element_name>: <prompt> [<default_value>]? value
 *  Default missing:   <element_name>: <prompt>?
 *  Prompt missing:    <element_name> [<default_value>]?
 *  No Prompt,Default: <element_name>?
 */
bool CocoaUIDelegate::output(const Element& element) const {
    aftlog << "Element " << element.getName();
    if (!element.getPrompt().empty()) aftlog << ": " << element.getPrompt();
    std::string defValue = element.getDefault().empty() ? "? " : " [" + element.getDefault() + "]? ";
    aftlog << defValue;
    if (element.hasValue()) {
        aftlog << element.getValue() << " ";
    }
    aftlog << std::endl;
    
    return true;
}

bool CocoaUIDelegate::remove(const Element& element) {
    if (root_ != nullptr && root_->getName() == element.getName()) {
        root_ = nullptr;
        return true;
    }
    return false;
}

bool CocoaUIDelegate::show(const Element& element, bool showValue) {
    aftlog << "Element " << element.getName();
    if (!element.getPrompt().empty()) aftlog << ": " << element.getPrompt();
    std::string defValue = element.getDefault().empty() ? "? " : " [" + element.getDefault() + "]? ";
    aftlog << defValue;
    if (showValue && element.hasValue()) {
        aftlog << element.getValue() << " ";
    }
    aftlog << std::flush;
    
    return true;
}
