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
#include "base/producer.h"


namespace aft
{
namespace base
{
// Forward reference

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
class BaseProc : public BaseProducer, public BaseConsumer
{
public:
    BaseProc(WriterContract* writerDelegate = 0, ReaderContract* readerDelegate = 0);
    virtual ~BaseProc();
    
};

} // namespace base
} // namespace aft
