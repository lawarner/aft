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

using namespace aft::base;

BaseProducer::BaseProducer(WriterContract* writerDelegate)
    : writerDelegate_(writerDelegate)
{
}

BaseProducer::~BaseProducer()
{
}


Result BaseProducer::read(TObject& object)
{
    if (writerDelegate_ && writerDelegate_->hasData() == ProductType::TOBJECT)
    {
        return writerDelegate_->getData(object);
    }
    return false;
}

Result BaseProducer::read(Result& result)
{
    if (writerDelegate_ && writerDelegate_->hasData() == ProductType::RESULT)
    {
        return writerDelegate_->getData(result);
    }
    return false;
}

Result BaseProducer::read(Blob& blob)
{
    if (writerDelegate_ && writerDelegate_->hasData() == ProductType::BLOB)
    {
        return writerDelegate_->getData(blob);
    }
    return false;
}

bool BaseProducer::hasData()
{
    if (writerDelegate_)
    {
        return writerDelegate_->hasData() != ProductType::NONE;
    }
    return false;
}

bool BaseProducer::hasObject(ProductType productType)
{
    if (writerDelegate_)
    {
        return writerDelegate_->hasData() == productType;
    }

    if (productType == ProductType::NONE) return true;
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
    if (!writerDelegate_ || writerDelegate_->hasData() == ProductType::NONE)
    {
        return;
    }

    ProductType productType;
    do
    {
        productType = writerDelegate_->hasData();
        switch (productType)
        {
        case ProductType::TOBJECT:
        {
            TObject tobject;
            if (writerDelegate_->getData(tobject))
            {
                // iterate thru readers until one reads the object
                std::vector<ReaderContract*>::iterator it;
                for (it = readers_.begin(); it != readers_.end(); ++it)
                {
                    if ((*it)->pushData(tobject))
                    {
                        break;
                    }
                }
            }
        }
            break;
        case ProductType::RESULT:
        {
            Result result;
            if (writerDelegate_->getData(result))
            {
                // iterate thru readers until one reads the object
                std::vector<ReaderContract*>::iterator it;
                for (it = readers_.begin(); it != readers_.end(); ++it)
                {
                    if ((*it)->pushData(result))
                    {
                        break;
                    }
                }
            }
        }
            break;
        case ProductType::BLOB:
        {
            Blob blob("");
            if (writerDelegate_->getData(blob)) {
                // iterate thru readers until one reads the object
                std::vector<ReaderContract*>::iterator it;
                for (it = readers_.begin(); it != readers_.end(); ++it)
                {
                    if ((*it)->pushData(blob))
                    {
                        break;
                    }
                }
            }
        }
            break;
        case ProductType::NONE:
            break;
        }
    } while (productType != ProductType::NONE);

}
