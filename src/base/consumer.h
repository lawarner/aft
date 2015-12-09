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
#include "producttype.h"

namespace aft
{
namespace base
{
// Forward reference
class Blob;
class Result;
class ReaderContract;
class TObject;

/**
 *  Interface that the consumer calls when it needs data.
 */
class WriterContract
{
public:
    /** Returns the type of product this writer has ready to write. */
    virtual ProductType hasData() = 0;

    // Note: Using references requires class to have proper copy semantics.
    /** Return true if data was written. */
    virtual bool getData(TObject& object) = 0;
    virtual bool getData(Result& result) = 0;
    virtual bool getData(Blob& blob) = 0;
};

/**
 *  Interface that classes must implement that consume data.
 */
class ConsumerContract
{
public:
    virtual bool write(const TObject& object) = 0;
    virtual bool write(const Result& result) = 0;
    virtual bool write(const Blob& blob) = 0;
    /** Returns true if write can be called on this consumer without blocking */
    virtual bool needsData() = 0;

    //TODO write multiple objects (array/vector)

    /** Register as a data writer for this consumer. */
    virtual bool registerWriteCallback(const WriterContract* writer) = 0;
    /** Unregister as a data writer for this consumer. */
    virtual bool unregisterWriteCallback(const WriterContract* writer) = 0;
};

/**
 *  Base implementation of the ProducerContract interface.
 */
class BaseConsumer
{
public:
    BaseConsumer(ReaderContract* readerDelegate = 0);
    virtual ~BaseConsumer();

    virtual bool write(const TObject& object);
    virtual bool write(const Result& result);
    virtual bool write(const Blob& blob);
    /** Returns true if write can be called on this consumer without blocking */
    virtual bool needsData();

    //TODO write multiple objects (array/vector)

    /** Register as a data writer for this consumer. */
    virtual bool registerWriteCallback(const WriterContract* writer);
    /** Unregister as a data writer for this consumer. */
    virtual bool unregisterWriteCallback(const WriterContract* writer);

    /** Start loop reading from writers and writing to the reader delegate. */
    virtual void flowData();

protected:
    ReaderContract* readerDelegate_;
    std::vector<WriterContract*> writers_;
};

} // namespace base
} // namespace aft
