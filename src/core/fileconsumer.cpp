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

#include <fstream>

#include "base/blob.h"
#include "base/producer.h"
#include "base/result.h"
#include "base/tobject.h"
#include "fileconsumer.h"


using namespace aft::base;
using namespace aft::core;


class aft::core::FileWriterImpl : public aft::base::ReaderContract
{
public:
    FileWriterImpl(const std::string& fileName, bool overwrite);
    virtual ~FileWriterImpl();

    virtual bool pushData(const TObject& object) override;
    virtual bool pushData(const Result& result) override;
    virtual bool pushData(const Blob& blob) override;

    bool roomForData() const;
    bool roomForObject(ProductType productType) const;

    bool is_open();
    bool open();
    void close();

private:
    std::string fileName_;
    bool overwrite_;
    std::ofstream outfile_;
    std::string buffer_;
};

FileWriterImpl::FileWriterImpl(const std::string& fileName, bool overwrite)
    : fileName_(fileName)
    , overwrite_(overwrite)
{
}

FileWriterImpl::~FileWriterImpl()
{
}


bool FileWriterImpl::pushData(const TObject& object)
{
    return false;
}

bool FileWriterImpl::pushData(const Result& result)
{
    return false;
}

bool FileWriterImpl::pushData(const Blob& blob)
{
    const std::string& strBlob = blob.getString();
    outfile_.write(strBlob.c_str(), strBlob.length());
    outfile_.flush();

    return !(outfile_.rdstate() & std::ofstream::failbit);
}

bool FileWriterImpl::roomForData() const {
    return outfile_.good();
}

bool FileWriterImpl::roomForObject(ProductType productType) const {
    if (roomForData() && productType == ProductType::BLOB) {
        return true;
    }
    return false;
}

bool FileWriterImpl::is_open()
{
    return outfile_.is_open();
}

bool FileWriterImpl::open()
{
    if (!overwrite_ && std::ifstream(fileName_.c_str()).good())
    {
        return false;
    }

    outfile_.open(fileName_.c_str(), std::ofstream::out);
    return outfile_.is_open();
}

void FileWriterImpl::close()
{
    if (outfile_.is_open())
    {
        outfile_.close();
    }
}


FileConsumer::FileConsumer(const std::string& fileName, bool overwrite)
    : writer_(new FileWriterImpl(fileName, overwrite))
{
    writer_->open();
}

FileConsumer::~FileConsumer()
{
    writer_->close();
    delete writer_;
}

Result FileConsumer::write(const TObject& object)
{
    return writer_->pushData(object);
}

Result FileConsumer::write(const Result& result)
{
    return writer_->pushData(result);
}

Result FileConsumer::write(const Blob& blob)
{
    return writer_->pushData(blob);
}

bool FileConsumer::canAcceptData()
{
    return writer_->is_open();
}
