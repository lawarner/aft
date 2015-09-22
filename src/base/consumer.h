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

#include <vector>

namespace aft
{
namespace base
{
// Forward reference
class Blob;
class TObject;

/**
 *  Interface used by the consumer to call writers when it needs data
 */
class WriterContract
{
public:
    /** Return true if data was written */
    virtual bool writeData(TObject& object) = 0;
    virtual bool writeData(Blob& blob) = 0;
};

/**
 *  Interface that classes must implement that consume data.
 */
class ConsumerContract
{
public:
    virtual bool write(const TObject& object) = 0;
    virtual bool write(const Blob& blob) = 0;
    /** Returns true if can write to this consumer without blocking */
    virtual bool needsData() = 0;

    //TODO write multiple objects (array/vector)

    virtual bool registerWriteCallback(const WriterContract* writer);
    virtual bool unregisterWriteCallback(const WriterContract* writer);

protected:
    std::vector<WriterContract*> writers_;
};

} // namespace base
} // namespace aft
