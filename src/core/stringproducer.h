#pragma once
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

#include "base/producer.h"

namespace aft
{
namespace base
{
// Forward reference
class Blob;
class Result;
class TObject;
}

namespace core
{
// Forward reference
class StringReaderImpl;

/**
 *  Producer that generates objects from a string.
 */
class StringProducer : public aft::base::BaseProducer
{
public:
    /** Construct a StringProducer
     *  @param contents Initial contents of string.
     *  @param parcelType How to parse the string for content.
     */
    StringProducer(const std::string& contents,
                   aft::base::ParcelType parcelType = aft::base::PARCEL_BLOB_FILE);
    virtual ~StringProducer();

    /** Not yet implemented. */
    virtual bool read(aft::base::TObject& object);
    /** Not yet implemented. */
    virtual bool read(aft::base::Result& result);
    /** Read blobs from the file.
     *
     *  The data in the blob is either a character, word, text line or the whole file,
     *  depending on this StringProducer's ParcelType.
     *  @param blob Reference of a blob object where the file contents is copied.
     *  @return true if data is copied, otherwise false
     */
    virtual bool read(aft::base::Blob& blob);
    virtual bool hasData();
    virtual bool hasObject(aft::base::ProductType productType);

    /** Put contents to the string. */
    void putContents(const std::string& contents, bool overwrite = false);

protected:
    // This is the same as writerDelegate_, but sub-classed
    StringReaderImpl* reader_;
};

} // namespace core
} // namespace aft
