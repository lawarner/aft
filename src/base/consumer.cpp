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


BaseConsumer::BaseConsumer(ReaderContract* readerDelegate)
    : readerDelegate_(readerDelegate)
{
}

BaseConsumer::~BaseConsumer()
{
}

bool BaseConsumer::write(const TObject& object)
{
    if (readerDelegate_)
    {
        return readerDelegate_->dataAvailable(object);
    }
    return false;
}

bool BaseConsumer::write(const Result& result)
{
    if (readerDelegate_)
    {
        return readerDelegate_->dataAvailable(result);
    }
    return false;
}

bool BaseConsumer::write(const Blob& blob)
{
    if (readerDelegate_)
    {
        return readerDelegate_->dataAvailable(blob);
    }
    return false;
}

bool BaseConsumer::needsData()
{
    return readerDelegate_ != 0;
}

bool BaseConsumer::registerWriteCallback(const WriterContract* writer)
{
    if (!writer) return false;

    std::vector<WriterContract*>::iterator it
        = std::find(writers_.begin(), writers_.end(), writer);
    if (it != writers_.end())
    {
        return false;   // writer already in callback list
    }
    writers_.push_back(const_cast<WriterContract*>(writer));
    return true;
}

bool BaseConsumer::unregisterWriteCallback(const WriterContract* writer)
{
    if (!writer) return false;

    std::vector<WriterContract*>::iterator it
        = std::find(writers_.begin(), writers_.end(), writer);
    if (it == writers_.end())
    {
        return false;   // writer not in callback list
    }
    writers_.erase(it);
    return true;
}

// writers are serialize: each is completely read before going to the next.
void BaseConsumer::flowData()
{
    if (!readerDelegate_ || writers_.empty()) return;

    ProductType productType;
    std::vector<WriterContract*>::iterator it;
    for (it = writers_.begin(); it != writers_.end(); ++it)
    {
        do
        {
            productType = (*it)->hasData();
            if (productType == TYPE_NONE) break;

            switch (productType)
            {
            case TYPE_TOBJECT:
            {
                TObject tobject;
                if ((*it)->getData(tobject))
                {
                    readerDelegate_->dataAvailable(tobject);
                }
            }
            break;
            case TYPE_RESULT:
            {
                Result result;
                if ((*it)->getData(result))
                {
                    readerDelegate_->dataAvailable(result);
                }
            }
            break;
            case TYPE_BLOB:
            {
                Blob blob("");
                if ((*it)->getData(blob))
                {
                    readerDelegate_->dataAvailable(blob);
                }
            }
            break;
            case TYPE_TYPEDBLOB:
            {
                TypedBlob blob("");
                if ((*it)->getData(blob))
                {
                    readerDelegate_->dataAvailable(blob);
                }
            }
            break;
            case TYPE_NONE:
                break;
            }
        } while (productType != TYPE_NONE);
    }
}
