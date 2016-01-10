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

#include <sstream>

#include "base/blob.h"
#include "base/consumer.h"
#include "base/context.h"
#include "base/structureddata.h"
#include "basiccommands.h"
#include "fileconsumer.h"
#include "fileproducer.h"
#include "logger.h"
#include "runcontext.h"
using namespace aft;
using namespace aft::core;


LogCommand::LogCommand(const std::string& message, const std::string& type)
    : Command("Log")
    , message_(message)
    , type_(type)
{
    parameters_.push_back(message_);
    parameters_.push_back(type_);
}

const base::Result
LogCommand::process(base::Context* context)
{
    if (type_.empty() && !message_.empty())     // The most common case
    {
        aftlog << "Command logger: " << message_ << std::endl;
    } else if (type_ == "context")
    {
        base::Context* ctx = context ? context : RunContext::global();
        aftlog << "Command logger: Context=" << ctx->getName() << std::endl;
    } else if (type_ == "result")
    {
        RunContext* ctx = context ? dynamic_cast<RunContext *>(context) : RunContext::global();
        base::Result& lastResult = ctx->getLastResult();
        std::ostringstream oss("Command logger: Result type=");
        oss.seekp(0, std::ostringstream::end);
        oss << lastResult.asString() << std::endl << " - value: ";
        switch (lastResult.getType())
        {
            case base::Result::FATAL:
                oss << "FATAL";
                break;
            case base::Result::BLOB:
            {
                base::Blob* blob = 0;
                if (lastResult.getValue(blob))
                {
                    if (blob->getType() == base::Blob::STRING)
                    {
                        oss << "String = " << blob->getString();
                    }
                }
            }
                break;
            case base::Result::BOOLEAN:
            {
                bool flag = false;
                lastResult.getValue(flag);
                oss << (flag ? "True" : "False");
            }
                break;
            case base::Result::COMMAND:
                oss << "Command";
                break;
            case base::Result::ITERATOR:
                oss << "Iterator";
                break;
            case base::Result::TOBJECT:
                oss << "TObject";
                break;
                
            case base::Result::UNKNOWN:     // fall thru
            default:
                oss << "(Unknown)";
                break;
        }
        aftlog << oss.str() << std::endl;
    }

    return base::Result(true);
}

const base::Result
LogCommand::setup(base::Context* context, const base::Blob* parameters)
{
//    RunContext* runcontext = dynamic_cast<RunContext *>(context);
    //TODO get and set 
    return base::Result(true);
}

///////////////////////////////////////////////////////////////////////////

ConsCommand::ConsCommand(const std::string& type, const std::string& name)
    : Command("Cons")
    , type_(type)
    , consumer_(0)
{
    parameters_.push_back(type);
    if (!name.empty())
    {
        parameters_.push_back(name);
    }
}

ConsCommand::~ConsCommand()
{
    if (consumer_) delete consumer_;
}

const base::Result
ConsCommand::process(base::Context* context)
{
    bool retval = false;

    if (parameters_.size() < 2)
    {
        aftlog << loglevel(Error) << "ConsCommand: Not enough parameters to process" << std::endl;
        return base::Result(retval);
    }

    RunContext* ctx = context ? dynamic_cast<RunContext *>(context) : RunContext::global();
    std::string consName = parameters_[1];
    if (type_ == "open" || type_ == "openw")
    {
        if (consumer_)
        {
            aftlog << loglevel(Warning) << "ConsCommand deleting existing consumer" << std::endl;
            delete consumer_;
        }
        consumer_ = new FileConsumer(consName, type_ == "openw");
        ctx->addConsumer(consName, consumer_);
    }
    else if (type_ == "close")
    {
        ctx->removeConsumer(consName);
    }
    else if (type_ == "needsData")
    {
        aftlog << "ConsCommand needsData" << std::endl;
        base::BaseConsumer* consumer = ctx->getConsumer(consName);
        if (!consumer)
        {
            aftlog << loglevel(Error) << "ConsCommand: " << consName << " not yet open" << std::endl;
            return base::Result(retval);
        }
        return base::Result(consumer->needsData());
    }
    else if (type_ == "write")
    {
        aftlog << "ConsCommand write" << std::endl;
        base::BaseConsumer* consumer = ctx->getConsumer(consName);
        if (!consumer)
        {
            aftlog << loglevel(Error) << "ConsCommand: " << consName << " not yet open" << std::endl;
            return base::Result(retval);
        }
        if (parameters_.size() < 3)
        {
            aftlog << loglevel(Error) << "ConsCommand: No data to write" << std::endl;
            return base::Result(retval);
        }
        if (!parameters_[2].empty() && consumer->needsData())
        {
            base::Blob blobData("", base::Blob::STRING, parameters_[2]);
            //aftlog << "- write string blob: " << blobData.getString() << std::endl;
            retval = consumer->write(blobData);
        }
    }

    return base::Result(retval);
}

const base::Result
ConsCommand::setup(base::Context* context, const base::Blob* parameters)
{
    if (!parameters) return base::Result(false);
    
    parameters_.push_back(parameters->getString());
    
    //TODO get and set
    return base::Result(true);
}

///////////////////////////////////////////////////////////////////////////

ProdCommand::ProdCommand(const std::string& type, const std::string& name)
: Command("Prod")
, type_(type)
, buffer_(new base::Blob(""))
, producer_(0)
{
    parameters_.push_back(type);
    if (!name.empty())
    {
        parameters_.push_back(name);
    }
}

ProdCommand::~ProdCommand()
{
    delete buffer_;
    if (producer_) delete producer_;
}


const base::Result
ProdCommand::process(base::Context* context)
{
    bool retval = false;
    
    if (parameters_.size() < 2)
    {
        aftlog << loglevel(Error) << "ProdCommand: Not enough parameters to process" << std::endl;
        return base::Result(retval);
    }
    
    RunContext* ctx = context ? dynamic_cast<RunContext *>(context) : RunContext::global();
    std::string prodName = parameters_[1];
    if (type_ == "open")
    {
        if (producer_)
        {
            aftlog << loglevel(Warning) << "ProdCommand deleting existing consumer" << std::endl;
            delete producer_;
        }
        producer_ = new FileProducer(prodName);
        ctx->addProducer(prodName, producer_);
    }
    else if (type_ == "close")
    {
        ctx->removeProducer(prodName);
    }
    else if (type_ == "hasData")
    {
        aftlog << "ProdCommand hasData" << std::endl;
        base::BaseProducer* producer = ctx->getProducer(prodName);
        if (!producer)
        {
            aftlog << loglevel(Error) << "ProdCommand: " << prodName << " not yet open" << std::endl;
            return base::Result(retval);
        }
        return base::Result(producer->hasData());
    }
    else if (type_ == "read")
    {
        aftlog << "ProdCommand read" << std::endl;
        base::BaseProducer* producer = ctx->getProducer(prodName);
        if (!producer)
        {
            aftlog << loglevel(Error) << "ProdCommand: " << prodName << " not yet open" << std::endl;
            return base::Result(retval);
        }
        if (producer->hasData())
        {
            if (producer->read(*buffer_))
            {
                aftlog << "- read: " << buffer_->getString();
                base::Result result(buffer_);
                ctx->setLastResult(result);     //TODO this should be done by the runVisitor for all commands
                return result;
            }
        }
    }

    return base::Result(retval);
}

const base::Result
ProdCommand::setup(base::Context* context, const base::Blob* parameters)
{
    if (!parameters) return base::Result(false);
    
    parameters_.push_back(parameters->getString());
    
    //TODO get and set
    return base::Result(true);
}
