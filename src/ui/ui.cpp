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

#include "base/blob.h"
#include "ui.h"

using namespace aft::base;
using namespace aft::ui;

// Internal implementation class
class aft::ui::UIProcImpl : public ReaderWriterContract
{
public:
    UIProcImpl(unsigned int maxSize)
    : maxSize_(maxSize)
    {  }
    virtual ~UIProcImpl()
    {  }
    
    /** Returns the type of product this writer has ready to write. */
    virtual ProductType hasData()
    {
        //if (queue_.empty()) return TYPE_NONE;
        
        return TYPE_BLOB;
    }
    
    /** Return true if data was written. */
    virtual bool getData(TObject& object)
    {
        return false;
    }
    virtual bool getData(Result& result)
    {
        return false;
    }
    virtual bool getData(Blob& blob)
    {
        //if (queue_.empty()) return false;
        
        //blob = queue_.front();
        //queue_.pop();
        return true;
    }
    
    // Reader contract
    virtual bool dataAvailable(const TObject& object)
    {
        return false;
    }
    virtual bool dataAvailable(const Result& result)
    {
        return false;
    }
    virtual bool dataAvailable(const Blob& blob)
    {
        //if (queue_.size() < maxSize_)
        //{
        //    queue_.push(blob);
        //    return true;
        //}
        return false;
    }
    bool roomForData() const
    {
        return false;  // queue_.size() < maxSize_;
    }
    bool roomForObject(ProductType productType) const
    {
        return productType == TYPE_BLOB && roomForData();
    }
    
private:
    unsigned int maxSize_;
};


UI::UI(int maxSize)
: base::BaseProc(0, 0)
, impl_(*new UIProcImpl(maxSize))
{
    
}

UI::~UI()
{
    delete &impl_;
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
bool UI::needsData()
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
