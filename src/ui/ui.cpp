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
, uiDelegate_(uiDelegate)
{

}

UI::~UI()
{
}

bool UI::addElement(Element* element)
{
    if (maxSize_ == 0 || uiElements_.size() < maxSize_)
    {
        auto it = findElement(element);
        if (it == uiElements_.end())
        {
            uiElements_.push_back(element);
            return true;
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

UI::ElementList::iterator
UI::findElement(Element* element) {
    ElementList::iterator it = std::find(uiElements_.begin(), uiElements_.end(), element);
    return it;
}

bool UI::firstElement() {
    if (uiElements_.empty()) return false;
    
    currentElement_ = 0;
    return true;
}

unsigned int UI::nextElement()
{
    currentElement_ = (currentElement_ + 1) % uiElements_.size();
    return currentElement_;
}

bool UI::removeElement(Element* element)
{
    auto it = findElement(element);
    if (it != uiElements_.end())
    {
        uiElements_.erase(it);
        return true;
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
bool UI::canAcceptData(bool isRequired)
{
    return !uiElements_.empty();
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
