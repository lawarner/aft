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
#include "base/producer.h"


namespace aft {
namespace base {

class ReaderWriterContract : public ReaderContract, public WriterContract
{

};

/**
 *  Interface that proc classes must implement.
 */
class ProcContract : public ProducerContract, public ConsumerContract
{

};

/**
 *  Base implementation of the ProducerContract/ConsumerContract interface.
 */
class BaseProc : public ProcContract
{
public:
    BaseProc(WriterContract* writerDelegate = nullptr, ReaderContract* readerDelegate = nullptr);
    virtual ~BaseProc() = default;

    virtual Result read(TObject& object) override;
    virtual Result read(Result& result) override;
    virtual Result read(Blob& blob) override;
    virtual bool hasData() override;
    virtual bool hasObject(ProductType productType) override;
    
    /** Register to receive a callback when data is available. */
    virtual bool registerDataCallback(const ReaderContract* reader) override;
    /** Unregister callback from receiving any more data. */
    virtual bool unregisterDataCallback(const ReaderContract* reader) override;
    
    /** Start loop reading from the writer delegate and writing to the readers.
     *
     *  TODO This method may need to either be overloaded or templatize.
     */
    virtual void flowData();

    /** Returns true if write can be called on this consumer without blocking */
    virtual bool canAcceptData() override;
    
    virtual Result write(const TObject& object) override;
    virtual Result write(const Result& result) override;
    virtual Result write(const Blob& blob) override;
    /** Returns the number of objects written. */
    virtual int write(const std::vector<TObject>& objects) override;
    virtual int write(const std::vector<Result>& results) override;
    virtual int write(const std::vector<Blob>& blobs) override;
    
    /** Register as a data writer for this consumer. */
    virtual bool registerWriteCallback(const WriterContract* writer) override;
    /** Unregister as a data writer for this consumer. */
    virtual bool unregisterWriteCallback(const WriterContract* writer) override;
private:
    BaseProducer producer_;
    BaseConsumer consumer_;
};

} // namespace base
} // namespace aft
