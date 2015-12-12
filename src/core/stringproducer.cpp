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

#include <strings.h>

#include "base/blob.h"
#include "base/consumer.h"
#include "base/result.h"
#include "base/tobject.h"
#include "stringproducer.h"

using namespace aft::base;
using namespace aft::core;


static const char* WORD_SEPARATORS = " \t\n\r;:()/#*";
static const char* LINE_SEPARATORS = "\n\r";

// Utility function for extracting the next word token from a string.
static bool getWord(std::string& istr, std::string& strWord,
                    const char* separators = WORD_SEPARATORS)
{
    strWord.clear();
    int ch;
    size_t len;
    for (len = 0; len < istr.length(); ++len)
    {
        ch = istr.at(len);
        if (!index(separators, ch))
        {
            strWord.push_back(ch);
        } else if (!strWord.empty()) { // Skip over delimiters till we find something
            break;
        }
    }

    if (len > 0)
    {
        istr.erase(0, len);
    }
    return !strWord.empty();
}


class aft::core::StringReaderImpl : public aft::base::WriterContract
{
public:
    StringReaderImpl(const std::string& contents, aft::base::ParcelType parcelType)
        : contents_(contents)
        , parcelType_(parcelType)
        {   }
    virtual ~StringReaderImpl() { }

    /** Returns the type of product this writer has ready to write. */
    virtual ProductType hasData()
        {
            if (contents_.empty()) return TYPE_NONE;

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
            bool retval = false;

            switch (parcelType_)
            {
            case PARCEL_BLOB_CHARACTER:
                if (!contents_.empty())
                {
                    int ch = contents_.at(0);
                    buffer_ = ch;
                    contents_.erase(0, 1);
                    retval = true;
                }
                break;
            case PARCEL_BLOB_WORD:
                retval = getWord(contents_, buffer_);
                break;
            case PARCEL_BLOB_LINE:
                retval = getWord(contents_, buffer_, LINE_SEPARATORS);
                break;
            case PARCEL_BLOB_FILE:
                if (!contents_.empty())
                {
                    buffer_ = contents_;
                    retval = true;
                }
                break;
            case PARCEL_RESULT:
            case PARCEL_TOBJECT:
                break;
            }

            if (retval)
            {
                Blob retBlob("string", Blob::STRING, buffer_);
                blob = retBlob;
            }

            return retval;
        }

    //TODO convert to istrstream
    std::string contents_;
private:
    ParcelType parcelType_;
    std::string buffer_;
};



StringProducer::StringProducer(const std::string& contents, ParcelType parcelType)
    : BaseProducer(0)
    , reader_(new StringReaderImpl(contents, parcelType))
{
    writerDelegate_ = reader_;
}

StringProducer::~StringProducer()
{
    delete reader_;
}

bool StringProducer::read(TObject& object)
{
    return false;
}

bool StringProducer::read(Result& result)
{
    return false;
}

bool StringProducer::read(Blob& blob)
{
    return reader_->getData(blob);
}

bool StringProducer::hasData()
{
    return reader_->hasData() == TYPE_BLOB;
}

bool StringProducer::hasObject(aft::base::ProductType productType)
{
    return reader_->hasData() == productType;
}

void StringProducer::putContents(const std::string& contents, bool overwrite)
{
    if (overwrite)
    {
        reader_->contents_ = contents;
    } else {
        reader_->contents_.append(contents);
    }
}
