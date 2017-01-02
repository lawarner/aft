#pragma once
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

#include "base/consumer.h"

namespace aft
{
namespace base
{
// Forward reference
class Blob;
class Result;
class ReaderContract;
class TObject;
class WriterContract;
}

namespace core
{
// Forward reference
class StringWriterImpl;

/**
 *  Consumer that writes objects to a string.
 *
 */
class StringConsumer : public aft::base::BaseConsumer
{
public:
    /** Construct a StringConsumer with an internal string buffer. */
    StringConsumer();

    /** Construct a StringConsumer
     *
     *  @param contents  reference to string where data is written.
     *  @param overwrite if true will clear contents of the string,
     *                   if false will append to exisitng data.
     */
    StringConsumer(std::string& contents, bool overwrite = false);
    virtual ~StringConsumer();

    /** Returns true if write can be called on this consumer without blocking */
    virtual bool canAcceptData(bool isRequired = false);

    virtual bool write(const aft::base::TObject& object);
    virtual bool write(const aft::base::Result& result);
    virtual bool write(const aft::base::Blob& blob);

    /** Clear the contents of the string. */
    void clear();

    /** Copy the contents of the string. */
    std::string getContents() const;

protected:
    std::unique_ptr<StringWriterImpl> writer_;
};

} // namespace base
} // namespace aft
