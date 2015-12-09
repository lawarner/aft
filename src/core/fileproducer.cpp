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
#include <strings.h>

#include "base/consumer.h"
#include "base/result.h"
#include "base/tobject.h"
#include "base/typedblob.h"
#include "fileproducer.h"

using namespace aft::base;
using namespace aft::core;


static const char* WORD_SEPERATORS = " \t\n\r;:()/#*";

// Utility function for extracting the next word token from file.
static bool getWord(std::ifstream& ifile, std::string& strWord)
{
    strWord.clear();
    int ch;
    while ((ch = ifile.get()) != EOF)
    {
        if (index(WORD_SEPERATORS, ch)) break;

        strWord.push_back(ch);
    }

    return !strWord.empty();
}


class aft::core::FileReaderImpl : public aft::base::WriterContract
{
public:
    FileReaderImpl(const std::string& fileName, FileProducer::FileProduct fileProduct)
        : fileName_(fileName)
        , fileProduct_(fileProduct)
        {   }
    virtual ~FileReaderImpl() { }

    /** Returns the type of product this writer has ready to write. */
    virtual ProductType hasData()
        {
            if (infile_.peek() == EOF) return TYPE_NONE;

            return TYPE_TYPEDBLOB;
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

            switch (fileProduct_)
            {
            case FileProducer::BLOB_CHARACTER:
            {
                int ch = infile_.get();
                if (ch != EOF)
                {
                    buffer_ = ch;
                    retval = true;
                }
            }
                break;
            case FileProducer::BLOB_WORD:
                retval = getWord(infile_, buffer_);
                break;
            case FileProducer::BLOB_LINE:
            {
                char lineBuf[1024];
                infile_.getline(lineBuf, 1024);
                if (infile_.gcount() > 0)
                {
                    buffer_ = lineBuf;
                    retval = true;
                }
            }
                break;
            case FileProducer::BLOB_FILE:
            {
                if (infile_.peek() != EOF)
                {
                    buffer_ = std::string(std::istreambuf_iterator<char>(infile_),
                                          std::istreambuf_iterator<char>());
                    retval = true;
                }
            }
                break;
            case FileProducer::RESULT:
            case FileProducer::TOBJECT:
                break;
            }

            if (retval)
            {
                TypedBlob retBlob("file", TypedBlob::STRING, buffer_);
                dynamic_cast<TypedBlob&>(blob) = retBlob;
            }

            return retval;
        }
    bool open()
        {
            infile_.open(fileName_.c_str());
            return infile_.is_open();
        }
    void close()
        {
            if (infile_.is_open())
            {
                infile_.close();
            }
        }

private:
    std::string fileName_;
    FileProducer::FileProduct fileProduct_;
    std::ifstream infile_;
    std::string buffer_;
};



//TODO : BaseProducer(new FileReaderImpl ...)
FileProducer::FileProducer(const std::string& fileName, FileProduct fileProduct)
    : BaseProducer(0)
    , reader_(new FileReaderImpl(fileName, fileProduct))
{
    writerDelegate_ = reader_;
    reader_->open();
}

FileProducer::~FileProducer()
{
    reader_->close();
    delete reader_;
}

bool FileProducer::read(TObject& object)
{
    return false;
}

bool FileProducer::read(Result& result)
{
    return false;
}

bool FileProducer::read(Blob& blob)
{
    return reader_->getData(blob);
}

bool FileProducer::hasData()
{
    return reader_->hasData() == TYPE_TYPEDBLOB;
}

bool FileProducer::hasObject(aft::base::ProductType productType)
{
    return reader_->hasData() == productType;
}
