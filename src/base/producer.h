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
class TObject;
class WriterContract;

/**
 *  Interface that the producer calls when data is available.
 */
class ReaderContract
{
public:
    /** Called by the Producer to deliver a TObject to readers.
     *  @return true if the reader consumed the data, otherwise false
     */
    virtual bool dataAvailable(const TObject& object) = 0;
    /** Called by the Producer to deliver a Result to readers.
     *  @return true if the reader consumed the data, otherwise false
     */
    virtual bool dataAvailable(const Result& result) = 0;
    /** Called by the Producer to deliver a Blob to readers.
     *  @return true if the reader consumed the data, otherwise false
     */
    virtual bool dataAvailable(const Blob& blob) = 0;
    
    /** Indicate if the reader can accept data */
    virtual bool roomForData() const = 0;
    virtual bool roomForObject(ProductType productType) const = 0;
};

/**
 *  Interface that classes must implement that produce data.
 */
class ProducerContract
{
public:
    /** Read a TObject from this producer.
     *  @param object Reference to TObject that receives the produced data.
     *  @return true if the object was read.
     */
    virtual bool read(TObject& object) = 0;
    /** Read a Result from this producer.
     *  @param object Reference to Result that receives the produced data.
     *  @return true if the result was read.
     */
    virtual bool read(Result& result) = 0;
    /** Read a Blob from this producer.
     *  @param object Reference to Blob that receives the produced data.
     *  @return true if the blob was read.
     */
    virtual bool read(Blob& blob) = 0;
    virtual bool hasData() = 0;
    virtual bool hasObject(ProductType productType) = 0;

    /** Register to receive a callback when data is available. */
    virtual bool registerDataCallback(const ReaderContract* reader) = 0;
    /** Unregister callback from receiving any more data. */
    virtual bool unregisterDataCallback(const ReaderContract* reader) = 0;
};

/**
 *  Base implementation of the ProducerContract interface.
 */
class BaseProducer : public ProducerContract
{
public:
    BaseProducer(WriterContract* writerDelegate = 0);
    virtual ~BaseProducer();

    virtual bool read(TObject& object);
    virtual bool read(Result& result);
    virtual bool read(Blob& blob);
    virtual bool hasData();
    virtual bool hasObject(ProductType productType);

    /** Register to receive a callback when data is available. */
    virtual bool registerDataCallback(const ReaderContract* reader);
    /** Unregister callback from receiving any more data. */
    virtual bool unregisterDataCallback(const ReaderContract* reader);

    /** Start loop reading from the writer delegate and writing to the readers.
     *
     *  TODO This method may need to either be overloaded or templatize.
     */
    virtual void flowData();

protected:
    WriterContract* writerDelegate_;
    std::vector<ReaderContract*> readers_;
};

} // namespace base
} // namespace aft
