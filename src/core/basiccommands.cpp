/*
 *   Copyright 2015, 2016 Andy Warner
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
#include "base/propertyhandler.h"
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

LogCommand::LogCommand(const std::vector<std::string>& parameters)
    : Command("Log", parameters)
    , message_(parameters[0])
    , type_(parameters[1])
{
    //aftlog << "DEBUG: create Log " << message_ << ", Type=" << type_ << std::endl;
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
        std::ostringstream oss("Command logger: Result Type=");
        oss.seekp(0, std::ostringstream::end);
        oss << lastResult.getTypeName() << ",  Value=" << lastResult.asString();
        aftlog << oss.str() << std::endl;
    }

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
    else if (type_ == "canAcceptData")
    {
        aftlog << "ConsCommand canAcceptData" << std::endl;
        base::BaseConsumer* consumer = ctx->getConsumer(consName);
        if (!consumer)
        {
            aftlog << loglevel(Error) << "ConsCommand: " << consName << " not yet open" << std::endl;
            return base::Result(retval);
        }
        return base::Result(consumer->canAcceptData());
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
        if (!parameters_[2].empty() && consumer->canAcceptData())
        {
            base::Blob blobData("", base::Blob::STRING, parameters_[2]);
            aftlog << loglevel(Debug) << "- write string blob: " << blobData.getString() << std::endl;
            retval = consumer->write(blobData);
        }
    }

    return base::Result(retval);
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

///////////////////////////////////////////////////////////////////////////

/*  This command handles get, set (global,local), unset, list */

EnvCommand::EnvCommand(const std::string& type, const std::string& name,
               const std::string& value)
: Command("Env")
, type_(type)
{
    parameters_.push_back(type);
    parameters_.push_back(name);
    if (!value.empty())
    {
        parameters_.push_back(value);
    }
}

EnvCommand::~EnvCommand()
{

}

const base::Result
EnvCommand::process(base::Context* context)
{
    bool retval = false;
    RunContext* ctx = context ? dynamic_cast<RunContext *>(context) : RunContext::global();
    if (type_ == "set")
    {
        if (parameters_.size() < 3 || parameters_[1].empty())
        {
            aftlog << loglevel(Error) << "EnvCommand set: missing value" << std::endl;
        }
        else
        {
            base::PropertyHandler& env = ctx->getEnvironment();
            env.setValue(parameters_[1], parameters_[2]);
            retval = true;
        }
    }
    else if (type_ == "get")
    {
        if (parameters_.size() < 2 || parameters_[1].empty())
        {
            aftlog << loglevel(Error) << "EnvCommand get: missing name" << std::endl;
        }
        else
        {
            base::PropertyHandler& env = ctx->getEnvironment();
            std::string value;
            if (env.getValue(parameters_[1], value))
            {
                result_ = base::Result(value);
                ctx->setLastResult(result_);
                return result_;
            }
        }
    }
    else if (type_ == "unset") {
        if (parameters_.size() < 2 || parameters_[1].empty()) {
            aftlog << loglevel(Error) << "EnvCommand unset: missing name" << std::endl;
        }
        else {
            base::PropertyHandler& env = ctx->getEnvironment();
            retval = env.unsetValue(parameters_[1]);
        }
    }
    else if (type_ == "list") {
        std::vector<std::string> names;
        base::PropertyHandler& env = ctx->getEnvironment();
        env.getPropertyNames(names);
        aftlog << loglevel(Info) << "Environment variables:" << std::endl;
        std::ostringstream oss;
        for (const auto& name : names) {
            std::string value;
            if (env.getValue(name, value)) {
                aftlog << loglevel(Info) << name << "=" << value << std::endl;
                oss << name << '=' << value << std::endl;
            } else {
                aftlog << loglevel(Info) << name << " (no value found)" << std::endl;
                oss << name << " (no value found)" << std::endl;
            }
        }
        result_ = base::Result(oss.str());
        ctx->setLastResult(result_);
        return result_;
    }

    return base::Result(retval);
}


GroupCommand::GroupCommand(const std::string& name)
    : Command("Group")
{
    parameters_.push_back(name);
}

GroupCommand::~GroupCommand()
{
    
}

aft::base::TObjectTree*
GroupCommand::add(aft::base::TObject* tObject, aft::base::TObjectTree* tObjWrapper)
{
    //TODO check type of tObject.
    return TObjectContainer::add(tObject, tObjWrapper);
}

const base::Result
GroupCommand::process(base::Context* context)
{
    return result_;
}


IfCommand::IfCommand(const std::string& name, const base::Operation& condition,
          base::Command* trueCommand, base::Command* falseCommand)
: Command("If")
, condition_(condition)
, trueCommand_(trueCommand)
, falseCommand_(falseCommand)
{
    
}

IfCommand::~IfCommand()
{
    
}

const base::Result IfCommand::process(base::Context* context)
{
    //TODO Command needs to override Operations so they apply to children, not the command itself
    //     Or if Operation contains an extra TObject then use that.
    if (supportsOperation(condition_))
    {
        if (applyOperation(condition_))
        {
            if (trueCommand_) return trueCommand_->process(context);
        }
        else
        {
            if (falseCommand_) return falseCommand_->process(context);
        }
    }

    return base::Result(base::Result::FATAL);
}

