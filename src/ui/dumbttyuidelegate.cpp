/*
 *  dumbttyuidelegate.cpp
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

#include "dumbttyelementdelegate.h"
#include "dumbttyuidelegate.h"

#include <iostream>
#include "element.h"
#include "uidelegate.h"
#include "core/logger.h"

using namespace aft::base;
using namespace aft::core;
using namespace aft::ui;
using namespace std;

DumbTtyUIDelegate::DumbTtyUIDelegate(UI* ui)
: BaseUIDelegate(ui)
, delegate_(std::make_unique<DumbTtyElementDelegate>()) {

}

void DumbTtyUIDelegate::flush(const Element& element) {
    delegate_->flush(&element);
}

bool DumbTtyUIDelegate::hide(const Element& element) {
    return false;
}

bool DumbTtyUIDelegate::input(const Element& element, std::string& value) {
    //TODO lookup element
    return delegate_->input(&element, value);
}

bool DumbTtyUIDelegate::output(const Element& element, bool showValue) const {
    return delegate_->output(&element, showValue);
}

bool DumbTtyUIDelegate::show(const Element& element) {
    return output(element);
}
