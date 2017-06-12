/*
 *   Copyright © 2016 Andy Warner. All rights reserved.
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

#include <algorithm>
#include <cassert>
#include <vector>

#include "base/blob.h"
#include "base/tobject.h"
#include "base/tobjecttype.h"
#include "core/logger.h"
#include "element.h"
#include "ui.h"
#include "uicommand.h"
#include "uidelegate.h"

using namespace aft::base;
using aft::core::aftlog;
using namespace std;


namespace aft {
namespace ui {

UI::UI(Element* element, unsigned int maxSize, UIDelegate* uiDelegate)
    : base::BaseProc(nullptr, nullptr)
    , maxSize_(maxSize)
    , currentElement_(0)
    , uiDelegate_(uiDelegate == nullptr ? make_unique<BaseUIDelegate>() : unique_ptr<UIDelegate>(uiDelegate)) {

    addElement(element);
}

UI::~UI() {

}

ElementHandle
UI::addElement(Element* element) {

    if (element != nullptr && (maxSize_ == 0 || uiElements_.size() < maxSize_)) {
        ssize_t idx = findElement(element);
        if (idx == -1) {
            callListeners(UiEventType::Adding, element);
            if (uiDelegate_->add(*element)) {
                uiElements_.push_back(element);
                return ElementHandle(element);
            }
        }
    }
    return ElementHandle();
}

Element*
UI::currentElement() const {
    if (currentElement_ < uiElements_.size()) {
        return uiElements_[currentElement_];
    }
    return nullptr;
}

ssize_t
UI::findElement(const ElementHandle& handle) const {
    auto it = std::begin(uiElements_);
    for ( ; it != std::end(uiElements_); ++it) {
        if (handle.getId() == (*it)->getId()) {
            return std::distance(std::begin(uiElements_), it);
        }
    }
    return -1;
}
    
bool UI::firstElement() {
    if (uiElements_.empty()) return false;
    
    currentElement_ = 0;
    return true;
}

Element*
UI::getElement(unsigned int idx) {
    return uiElements_[idx];
}

Element*
UI::getElement(const ElementHandle& handle) {
    ssize_t idx = findElement(handle);
    if (-1 == idx) {
        return nullptr;
    }
    return uiElements_[idx];
}

UI::ElementList&
UI::getElementList() {
    return uiElements_;
}

bool UI::getElementValue(const ElementHandle& handle, std::string& value) {
    Element* element = getElement(handle);
    if (nullptr == element) {
        return false;
    }
    value = element->getValue();
    return true;
}
    
bool UI::getElementValue(std::string& value) {
    Element* element = currentElement();
    if (nullptr == element) {
        return false;
    }
    value = element->getValue();
    return true;
}

ssize_t
UI::nextElement() {
    if (uiElements_.size() == 0) return -1;
    if (currentElement_ + 1 >= uiElements_.size()) return -1;

    return ++currentElement_;
}

bool UI::removeElement(const ElementHandle& handle) {
    Element* element = getElement(handle);
    if (nullptr == element) {
        return false;
    }
    callListeners(UiEventType::Removing, element);
    if (uiDelegate_->remove(*element)) {
        ssize_t idx = findElement(element);
        if (idx != -1) {
            auto it = uiElements_.begin() + idx;
            uiElements_.erase(it);
            return true;
        }
    }
    return false;
}

Result UI::init(base::Context* context)
{
    return Result(true);
}

Result UI::deinit(base::Context* context)
{
    return Result(true);
}

void UI::draw() {
    for (auto element : uiElements_) {
        uiDelegate_->output(*element, true);
        uiDelegate_->flush(*element);
    }
}

void UI::erase() {
    
}

void UI::input() {
    auto& element = *uiElements_[currentElement_];
    std::string value;
    uiDelegate_->input(element, value);
    element.setValue(value);
    callListeners(UiEventType::ValueChanged, &element);
}
    
void UI::output() {
    auto& element = *uiElements_[currentElement_];
    uiDelegate_->output(element);
}
    
void UI::registerListener(CallbackFunction* listener) {
    for (auto it = begin(listeners_); it != end(listeners_); ++it) {
        if (*it == listener) return;
    }
    listeners_.push_back(listener);
}

void UI::unregisterListener(CallbackFunction* listener) {
    auto it = find(begin(listeners_), end(listeners_), listener);
    if (it != end(listeners_)) {
        listeners_.erase(it);
    }
}

bool UI::callListeners(UiEventType event, Element* element) {
    bool retval = true;
    for (auto it = begin(listeners_); it != end(listeners_); ++it) {
        if (!(*it)->operator()(event, element)) {
            retval = false;
            break;
        }
    }
    return retval;
}

// Producer contract
bool UI::read(base::TObject& object) {
    return false;
}

bool UI::read(base::Result& result) {
    return false;
}

bool UI::read(base::Blob& blob) {

    if (hasData()) {
        Element* element = uiElements_[currentElement_];
        blob = Blob(element->getName(), Blob::STRING, element->getValue());
        return true;
    }
    return false;
}

bool UI::hasData() {
    if (uiElements_.empty()) return false;
    
    // ask current element for data
    if (uiElements_[currentElement_]->hasValue()) {
        return true;
    }
    return false;
}

bool UI::hasObject(base::ProductType productType)
{
    if (base::ProductType::BLOB != productType || uiElements_.empty()) {
        return false;
    }
    
    // ask current element for data
    Element* currElem = uiElements_[currentElement_];
    return currElem->hasValue();
}

// Consumer contract
bool UI::canAcceptData()
{
    //TODO accept ui-wide commands
    //return !uiElements_.empty();
    return true;
}

bool UI::write(const base::TObject& object) {
    if (!canAcceptData()) return false;

    if (TObjectType::TypeUiCommand != object.getType()) {
        return false;
    }
    Result result(false);
    const UICommand& command = dynamic_cast<const UICommand&>(object);
    switch (command.getCommandType()) {
        case CommandType::Start:
            result = init();
            break;
        case CommandType::AddElement:
            addElement(command.getElement());
            break;
        case CommandType::RemoveElement:
            removeElement(command.getElement());
            break;
        case CommandType::Exit:
            result = deinit();
            break;
        //TODO pause, hide, show, ...
        default:
            break;
    }
    return result;
}

bool UI::write(const base::Result& result) {
    return false;
}

// For a UI these are basically the commands to construct and control the UI
//TODO use entities
bool UI::write(const base::Blob& blob) {
    if (!canAcceptData()) return false;
    
    //TODO These commands would result in a TObject of subclass UICommand
    if (blob.getType() == Blob::COMMAND) {
        
    }
    else if (blob.getType() == Blob::STRING) {
        aftlog << "UI::write=" << blob.getString() << endl;
    }
    return false;
}

} // namespace ui
} // namespace aft
