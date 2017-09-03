/*
 *  uidelegate.cpp
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

#include "element.h"
#include "elementdelegate.h"
#include "ui.h"
#include "uidelegate.h"
#include "core/logger.h"

using namespace aft::base;
using namespace aft::core;
using namespace aft::ui;

BaseUIDelegate::BaseUIDelegate(UI* ui)
    : ui_(ui)
    , delegate_(std::make_unique<BaseElementDelegate>())
    , elements_(ui->getElementList()) {

}

bool BaseUIDelegate::add(const Element& element) {
    return true;
}

void BaseUIDelegate::flush(const Element& element) {
    delegate_->flush(&element);
}

bool BaseUIDelegate::focus(const Element& element) {
    return false;
}

const Element*
BaseUIDelegate::get(const ElementHandle& handle) {
    auto it = std::begin(elements_);
    for ( ; it != std::end(elements_); ++it) {
        if (handle.getId() == (*it)->getId()) {
            return *it;
        }
    }
    return nullptr;
}

const Element*
BaseUIDelegate::get(const std::string& name) {
    auto it = std::begin(elements_);
    for ( ; it != std::end(elements_); ++it) {
        if (name == (*it)->getName()) {
            return *it;
        }
    }
    return nullptr;
}

bool BaseUIDelegate::hide(const Element& element) {
    return false;
}

bool BaseUIDelegate::input(Element& element, std::string& value) {
    const Element* currEl = get(&element);
    if (nullptr == currEl) {
        return false;
    }
    if (currEl->hasValue()) {
        value = currEl->getValue();
    }
    else {
        value = currEl->getDefault();
    }

    return true;
}

bool BaseUIDelegate::output(const Element& element, bool showValue) const {
    //TODO aftlog
    return true;
}

bool BaseUIDelegate::remove(const Element& element) {
    return true;
}

bool BaseUIDelegate::show(const Element& element) {
    //TODO aftlog
    return true;
}
