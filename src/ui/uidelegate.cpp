#pragma once
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
#include "uidelegate.h"

using namespace aft::ui;

bool BaseUIDelegate::add(const Element& element) {
    return false;
}

bool BaseUIDelegate::focus(const Element& element) {
    return false;
}
bool BaseUIDelegate::hide(const Element& element) {
    return false;
}
bool BaseUIDelegate::input(const Element& element, std::string& value) {
    return false;
}
bool BaseUIDelegate::output(const Element& element) {
    return false;
}

bool BaseUIDelegate::remove(const Element& element) {
    return false;
}

bool BaseUIDelegate::show(const Element& element) {
    return false;
}

