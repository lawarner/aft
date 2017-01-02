/*
 *   Copyright © 2015-2017 Andy Warner
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
#include "base/producer.h"
#include "base/result.h"
#include "base/tobject.h"
#include "stringconsumer.h"


using namespace aft::base;
using namespace aft::core;


class aft::core::StringWriterImpl : public aft::base::ReaderContract
{
public:
    StringWriterImpl();
    StringWriterImpl(std::string& contents, bool overwrite);
    virtual ~StringWriterImpl();

    virtual bool dataAvailable(const TObject& object);
    virtual bool dataAvailable(const Result& result);
    virtual bool dataAvailable(const Blob& blob);

    virtual bool roomForData() const;
    virtual bool roomForObject(ProductType productType) const;

    std::string& buffer_;
private:
    std::string  strInternal_;
};

StringWriterImpl::StringWriterImpl()
    : buffer_(strInternal_)
{
}

StringWriterImpl::StringWriterImpl(std::string& contents, bool overwrite)
    : buffer_(contents)
{
    if (overwrite)
    {
        buffer_.clear();
    }
}

StringWriterImpl::~StringWriterImpl()
{
}


bool StringWriterImpl::dataAvailable(const TObject& object)
{
    return false;
}

bool StringWriterImpl::dataAvailable(const Result& result)
{
    return false;
}

bool StringWriterImpl::dataAvailable(const Blob& blob)
{
    const std::string& strBlob = blob.getString();
    buffer_.append(strBlob);
    return true;
}

bool StringWriterImpl::roomForData() const
{
    return true;
}

bool StringWriterImpl::roomForObject(ProductType productType) const
{
    if (roomForData() && productType == ProductType::BLOB)
    {
        return true;
    }
    return false;
}


StringConsumer::StringConsumer()
: writer_(std::make_unique<StringWriterImpl>())
{
}

StringConsumer::StringConsumer(std::string& contents, bool overwrite)
: writer_(std::make_unique<StringWriterImpl>(contents, overwrite))
{
}

StringConsumer::~StringConsumer() {

}

bool StringConsumer::canAcceptData(bool isRequired)
{
    return true;
}

bool StringConsumer::write(const TObject& object)
{
    return false;
}

bool StringConsumer::write(const Result& result)
{
    return false;
}

bool StringConsumer::write(const Blob& blob)
{
    return writer_->dataAvailable(blob);
}

void StringConsumer::clear()
{
    writer_->buffer_.clear();
}

std::string StringConsumer::getContents() const
{
    return writer_->buffer_;
}

