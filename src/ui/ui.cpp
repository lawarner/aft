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
#include "element.h"
#include "ui.h"
#include "uidelegate.h"

using namespace aft::base;
using namespace std;


namespace aft
{
namespace ui
{

UI::UI(Element* element, unsigned int maxSize, UIDelegate* uiDelegate)
    : base::BaseProc(nullptr, nullptr)
    , maxSize_(maxSize)
    , currentElement_(0)
    , uiDelegate_(uiDelegate == nullptr ? make_unique<BaseUIDelegate>() : unique_ptr<UIDelegate>(uiDelegate)) {

}

UI::~UI() {

}

bool UI::addElement(Element* element) {
    if (maxSize_ == 0 || uiElements_.size() < maxSize_) {
        ssize_t idx = findElement(element);
        if (idx == -1) {
            if (uiDelegate_->add(*element)) {
                uiElements_.push_back(element);
                return true;
            }
        }
    }
    return false;
}

Element* UI::currentElement() const {
    if (currentElement_ < uiElements_.size()) {
        return uiElements_[currentElement_];
    }
    return nullptr;
}

ssize_t
UI::findElement(Element* element) const {
    auto it = std::find(uiElements_.begin(), uiElements_.end(), element);
    if (it == uiElements_.end()) {
        return -1;
    }
    return std::distance(uiElements_.begin(), it);
}
    
bool UI::firstElement() {
    if (uiElements_.empty()) return false;
    
    currentElement_ = 0;
    return true;
}

Element* UI::getElement(unsigned int idx) {
    return uiElements_[idx];
}

ssize_t
UI::nextElement()
{
    if (uiElements_.size() == 0) return -1;
    
    currentElement_ = (currentElement_ + 1) % uiElements_.size();
    return currentElement_;
}

bool UI::removeElement(Element* element)
{
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
        uiDelegate_->output(*element);
    }
}

void UI::erase() {
    
}

void UI::input() {
    auto& element = *uiElements_[currentElement_];
    std::string value;
    uiDelegate_->input(element, value);
    element.setValue(value);
}
    
void UI::output() {
    auto& element = *uiElements_[currentElement_];
    uiDelegate_->show(element, true);
    
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

// Producer contract
bool UI::read(base::TObject& object)
{
    return false;
}

bool UI::read(base::Result& result)
{
    return false;
}

bool UI::read(base::Blob& blob)
{
    if (uiElements_.empty()) return false;
    
    // ask current element for data
    if (uiElements_[currentElement_]->hasValue())
    {
        Element* element = uiElements_[currentElement_];
        blob = Blob(element->getName(), Blob::STRING, element->getValue());
        return true;
    }
    
    return false;
}

bool UI::hasData()
{
    if (uiElements_.empty()) return false;
    
    // ask current element for data
    if (uiElements_[currentElement_]->hasValue())
    {
        return true;
    }
    
    return false;
}

bool UI::hasObject(base::ProductType productType)
{
    if (uiElements_.empty()) return false;
    
    // ask current element for data
    if (uiElements_[currentElement_]->hasValue())
    {
        return true;
    }

    return false;
}

// Consumer contract
bool UI::canAcceptData()
{
    //TODO accept ui-wide commands
    //return !uiElements_.empty();
    return true;
}

bool UI::write(const base::TObject& object)
{
    return false;
}

bool UI::write(const base::Result& result)
{
    return false;
}

// For a UI these are basically the commands to construct and control the UI
//TODO use entities
bool UI::write(const base::Blob& blob)
{
    if (!canAcceptData()) return false;
    
    if (blob.getType() == Blob::COMMAND)
    {
        
    }
    else if (blob.getType() == Blob::STRING)
    {
        
    }
    //if (queue_.size() < maxSize_)
    //{
    //    queue_.push(blob);
    //    return true;
    //}
    return false;
}

} // namespace ui
} // namespace aft
