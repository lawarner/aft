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
 *  Interface used by the producer to call when data is available.
 */
class ReaderContract
{
public:
    virtual void dataAvailable(TObject& object) = 0;
    virtual void dataAvailable(Blob& blob) = 0;
};

/**
 *  Interface that classes must implement that produce data.
 */
class ProducerContract
{
public:
    virtual bool read(TObject& object) = 0;
    virtual bool read(Blob& blob) = 0;
    virtual bool hasData() = 0;

    virtual bool registerDataCallback(const ReaderContract* reader);
    virtual bool unregisterDataCallback(const ReaderContract* reader);

protected:
    std::vector<ReaderContract*> readers_;
};

} // namespace base
} // namespace aft
