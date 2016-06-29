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
class FileWriterImpl;

/**
 *  Consumer that writes objects to a file.
 *
 *  Similar to the comment in fileproducer.h, a hint for the "type" of file would
 *  be helpful.  By "type" I mean is the file raw, TObjects only, Results, Blobs, string,
 *  serialized data, json, etc.
 *  This class uses std::ofstream to write the file.
 */
class FileConsumer : public aft::base::BaseConsumer
{
public:
    /** Construct a FileConsumer
     *
     *  @param fileName name with optional path of file to write.
     *  @param overwrite if true will overwrite an existing file and
     *                   if false will not overwrite an existing file.
     */
    FileConsumer(const std::string& fileName, bool overwrite = false);
    virtual ~FileConsumer();

    virtual bool write(const aft::base::TObject& object);
    virtual bool write(const aft::base::Result& result);
    virtual bool write(const aft::base::Blob& blob);
    /** Returns true if write can be called on this consumer without blocking */
    virtual bool canAcceptData(bool isRequired = false);

protected:
    FileWriterImpl* writer_;
};

} // namespace base
} // namespace aft
