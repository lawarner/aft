/*
 *   Copyright 2015 Andy Warner
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

#include "blob.h"
#include "consumer.h"
#include "producer.h"
#include "result.h"
#include "tobject.h"
#include "typedblob.h"

using namespace aft::base;

BaseProducer::BaseProducer(WriterContract* writerDelegate)
    : writerDelegate_(writerDelegate)
{
}

BaseProducer::~BaseProducer()
{
}


bool BaseProducer::read(TObject& object)
{
    if (writerDelegate_ && writerDelegate_->hasData() == TYPE_TOBJECT)
    {
        return writerDelegate_->getData(object);
    }
    return false;
}

bool BaseProducer::read(Result& result)
{
    if (writerDelegate_ && writerDelegate_->hasData() == TYPE_RESULT)
    {
        return writerDelegate_->getData(result);
    }
    return false;
}

bool BaseProducer::read(Blob& blob)
{
    if (writerDelegate_ && writerDelegate_->hasData() == TYPE_BLOB)
    {
        return writerDelegate_->getData(blob);
    }
    return false;
}

bool BaseProducer::hasData()
{
    if (writerDelegate_)
    {
        return writerDelegate_->hasData() != TYPE_NONE;
    }
    return false;
}

bool BaseProducer::hasObject(ProductType productType)
{
    if (writerDelegate_)
    {
        return writerDelegate_->hasData() == productType;
    }

    if (productType == TYPE_NONE) return true;
    return false;
}

bool BaseProducer::registerDataCallback(const ReaderContract* reader)
{
    if (!reader) return false;

    std::vector<ReaderContract*>::iterator it
        = std::find(readers_.begin(), readers_.end(), reader);
    if (it != readers_.end())
    {
        return false;   // reader already in callback list
    }
    readers_.push_back(const_cast<ReaderContract*>(reader));
    return true;
}

bool BaseProducer::unregisterDataCallback(const ReaderContract* reader)
{
    if (!reader) return false;

    std::vector<ReaderContract*>::iterator it
        = std::find(readers_.begin(), readers_.end(), reader);
    if (it == readers_.end())
    {
        return false;   // reader not in callback list
    }
    readers_.erase(it);
    return true;
}

void BaseProducer::flowData()
{
    if (!writerDelegate_ || writerDelegate_->hasData() == TYPE_NONE)
    {
        return;
    }

    ProductType productType;
    do
    {
        productType = writerDelegate_->hasData();
        switch (productType)
        {
        case TYPE_TOBJECT:
        {
            TObject tobject;
            if (writerDelegate_->getData(tobject))
            {
                // iterate thru readers until one reads the object
                std::vector<ReaderContract*>::iterator it;
                for (it = readers_.begin(); it != readers_.end(); ++it)
                {
                    if ((*it)->dataAvailable(tobject))
                    {
                        break;
                    }
                }
            }
        }
            break;
        case TYPE_RESULT:
        {
            Result result;
            if (writerDelegate_->getData(result))
            {
                // iterate thru readers until one reads the object
                std::vector<ReaderContract*>::iterator it;
                for (it = readers_.begin(); it != readers_.end(); ++it)
                {
                    if ((*it)->dataAvailable(result))
                    {
                        break;
                    }
                }
            }
        }
            break;
        case TYPE_BLOB:
        {
            Blob blob("");
            if (writerDelegate_->getData(blob))
            {
                // iterate thru readers until one reads the object
                std::vector<ReaderContract*>::iterator it;
                for (it = readers_.begin(); it != readers_.end(); ++it)
                {
                    if ((*it)->dataAvailable(blob))
                    {
                        break;
                    }
                }
            }
        }
            break;
        case TYPE_TYPEDBLOB:
        {
            TypedBlob blob("");
            if (writerDelegate_->getData(blob))
            {
                // iterate thru readers until one reads the object
                std::vector<ReaderContract*>::iterator it;
                for (it = readers_.begin(); it != readers_.end(); ++it)
                {
                    if ((*it)->dataAvailable(blob))
                    {
                        break;
                    }
                }
            }
        }
            break;
        case TYPE_NONE:
            break;
        }
    } while (productType != TYPE_NONE);

}

