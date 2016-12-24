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

// Internal implementation class
class UIProcImpl : public ReaderWriterContract
{
public:
    typedef std::vector<Element *> ElementList;

    UIProcImpl(Element* element, unsigned int maxSize, UIDelegate* uiDelegate = 0);
    virtual ~UIProcImpl();

    bool addElement(Element* element);
    ElementList::iterator findElement(Element* element);
    unsigned int nextElement();
    bool removeElement(Element* element);

    /** Returns the type of product this writer has ready to write. */
    virtual ProductType hasData();

    /** Get data as TObject, if available.
     *  @return true if data was written. */
    virtual bool getData(TObject& object);
    virtual bool getData(Result& result);
    virtual bool getData(Blob& blob);

    // Reader contract
    // For a UI these are basically the commands to construct and control the UI
    //TODO use entities
    virtual bool dataAvailable(const TObject& object);
    virtual bool dataAvailable(const Result& result);
    virtual bool dataAvailable(const Blob& blob);

    bool roomForData() const;
    bool roomForObject(ProductType productType) const;

private:
    unsigned int maxSize_;
    unsigned int currentElement_;
    ElementList uiElements_;
    UIDelegate* uiDelegate_;
};

UIProcImpl::UIProcImpl(Element* element, unsigned int maxSize, UIDelegate* uiDelegate)
: maxSize_(maxSize)
, currentElement_(0)
, uiDelegate_(uiDelegate)
{
    if (element)
    {
        assert(maxSize > 0);
        uiElements_.push_back(element);
    }
}

UIProcImpl::~UIProcImpl()
{
}

bool UIProcImpl::addElement(Element* element)
{
    if (maxSize_ == 0 || uiElements_.size() < maxSize_)
    {
        ElementList::iterator it = findElement(element);
        if (it == uiElements_.end())
        {
            uiElements_.push_back(element);
            return true;
        }
    }
    return false;
}

UIProcImpl::ElementList::iterator
UIProcImpl::findElement(Element* element)
{
    UIProcImpl::ElementList::iterator it = std::find(uiElements_.begin(), uiElements_.end(), element);
    return it;
}

unsigned int UIProcImpl::nextElement()
{
    currentElement_ = (currentElement_ + 1) % uiElements_.size();
    return currentElement_;
}

bool UIProcImpl::removeElement(Element* element)
{
    ElementList::iterator it = findElement(element);
    if (it == uiElements_.end())
    {
        uiElements_.erase(it);
        return true;
    }
    return false;
}

ProductType UIProcImpl::hasData()
{
    if (uiElements_.empty()) return TYPE_NONE;

    // ask current element for data
    if (uiElements_[currentElement_]->hasValue())
    {
        return TYPE_BLOB;
    }

    return TYPE_NONE;
}

bool UIProcImpl::getData(TObject& object)
{
    return false;
}

bool UIProcImpl::getData(Result& result)
{
    return false;
}

bool UIProcImpl::getData(Blob& blob)
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

// Reader contract
// For a UI these are basically the commands to construct and control the UI
//TODO use entities
bool UIProcImpl::dataAvailable(const TObject& object)
{
    return false;
}
bool UIProcImpl::dataAvailable(const Result& result)
{
    return false;
}
bool UIProcImpl::dataAvailable(const Blob& blob)
{
    if (!roomForData()) return false;

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
bool UIProcImpl::roomForData() const
{
    return !uiElements_.empty();
}

bool UIProcImpl::roomForObject(ProductType productType) const
{
    return productType == TYPE_BLOB && roomForData();
}


UI::UI(Element* element, unsigned int maxSize, UIDelegate* uiDelegate)
: base::BaseProc(nullptr, nullptr)
, impl_(*new UIProcImpl(element, maxSize, uiDelegate))
{

}

UI::~UI()
{
    delete &impl_;
}

bool UI::addElement(Element* element)
{
    return impl_.addElement(element);
}

unsigned int UI::nextElement()
{
    return impl_.nextElement();
}

bool UI::removeElement(Element* element)
{
    return impl_.removeElement(element);
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
    return impl_.getData(blob);
}

bool UI::hasData()
{
    return impl_.hasData() == TYPE_BLOB;
}

bool UI::hasObject(base::ProductType productType)
{
    return impl_.hasData() == productType;
}

// Consumer contract
bool UI::canAcceptData(bool isRequired)
{
    return impl_.roomForData();
}

bool UI::write(const base::TObject& object)
{
    return false;
}

bool UI::write(const base::Result& result)
{
    return false;
}

bool UI::write(const base::Blob& blob)
{
    return impl_.dataAvailable(blob);
}

} // namespace ui
} // namespace aft
