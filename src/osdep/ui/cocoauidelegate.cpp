/*
 *  cocoauidelegate.cpp
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

#include "cocoaelementdelegate.h"
#include "cocoauidelegate.h"
#include "element.h"
#include "uiaftbridge.h"
using namespace aft::base;
using namespace aft::osdep;
using namespace aft::ui;

CocoaUIDelegate::CocoaUIDelegate(UI* ui)
    : BaseUIDelegate(ui)
    , delegate_(std::make_unique<CocoaElementDelegate>()) {
    
}

bool CocoaUIDelegate::add(const Element& element) {
    UiAftBridge* uiaft = UiAftBridge::instance();
    return uiaft->addElement(const_cast<Element *>(&element));
}

void CocoaUIDelegate::flush(const Element& element) {
    delegate_->flush(&element);
}

bool CocoaUIDelegate::focus(const Element& element) {
    return false;
}

bool CocoaUIDelegate::hide(const Element& element) {
    return false;
}

bool CocoaUIDelegate::input(const Element& element, std::string& value) {
    return delegate_->input(&element, value);
}

bool CocoaUIDelegate::output(const Element& element, bool showValue) const {
    return delegate_->output(&element, showValue);
}

bool CocoaUIDelegate::show(const Element& element) {
    return output(element);
}

