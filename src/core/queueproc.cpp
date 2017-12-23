//
//  queueproc.cpp
//  libaft
//
//  Copyright © 2016 Andy Warner. All rights reserved.
//

#include <queue>

#include "base/blob.h"
#include "queueproc.h"

using namespace aft::base;
using namespace aft::core;

// Internal implementation class
class aft::core::QueueProcImpl : public WriterContract, public ReaderContract
{
public:
    QueueProcImpl(unsigned int maxSize)
    : maxSize_(maxSize)
    {  }
    virtual ~QueueProcImpl()
    {  }

    /** Returns the type of product this writer has ready to write. */
    virtual ProductType hasData()
    {
        if (queue_.empty()) return ProductType::NONE;

        return ProductType::BLOB;
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
        if (queue_.empty()) return false;
        
        blob = queue_.front();
        queue_.pop();
        return true;
    }

    // Reader contract
    virtual bool pushData(const TObject& object)
    {
        return false;
    }
    virtual bool pushData(const Result& result)
    {
        return false;
    }
    virtual bool pushData(const Blob& blob)
    {
        if (queue_.size() < maxSize_)
        {
            queue_.push(blob);
            return true;
        }
        return false;
    }
    bool roomForData() const
    {
        return queue_.size() < maxSize_;
    }
    bool roomForObject(ProductType productType) const
    {
        return productType == ProductType::BLOB && roomForData();
    }

private:
    unsigned int maxSize_;
    std::queue<base::Blob> queue_;
};


QueueProc::QueueProc(int maxSize)
: base::BaseProc(0, 0)
, impl_(*new QueueProcImpl(maxSize))
{
    
}

QueueProc::~QueueProc()
{
    delete &impl_;
}

// Producer contract
Result QueueProc::read(base::TObject& object)
{
    return false;
}

Result QueueProc::read(base::Result& result)
{
    return false;
}

Result QueueProc::read(base::Blob& blob)
{
    return impl_.getData(blob);
}

bool QueueProc::hasData()
{
    return impl_.hasData() == ProductType::BLOB;
}

bool QueueProc::hasObject(base::ProductType productType)
{
    return impl_.hasData() == productType;
}

// Consumer contract
bool QueueProc::canAcceptData()
{
    return impl_.roomForData();
}

Result QueueProc::write(const base::TObject& object)
{
    return false;
}

Result QueueProc::write(const base::Result& result)
{
    return false;
}

Result QueueProc::write(const base::Blob& blob)
{
    return impl_.pushData(blob);
}
