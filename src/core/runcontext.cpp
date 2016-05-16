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

#include <map>

#include "base/consumer.h"
#include "base/producer.h"
#include "base/result.h"
#include "loghandler.h"
#include "runcontext.h"
using namespace aft;
using namespace aft::core;


static RunContext* globalInstance = 0;

class aft::core::RunContextImpl
{
public:
    RunContextImpl(LogHandler& logHandler);
    ~RunContextImpl();

    LogHandler& logHandler_;
    std::map<std::string, base::BaseConsumer*> consumers_;
    std::map<std::string, base::BaseProducer*> producers_;
    
    base::Result lastResult_;
};

RunContextImpl::RunContextImpl(LogHandler& logHandler)
    : logHandler_(logHandler)
{
}

RunContextImpl::~RunContextImpl()
{
    delete &logHandler_;
}

RunContext* RunContext::global()
{
    if (!globalInstance)
    {
        globalInstance = new RunContext;
    }
    return globalInstance;
}

RunContext::RunContext()
: impl_(*new RunContextImpl(*new LogHandler))
{
}

RunContext::~RunContext()
{
    delete &impl_;
}

void RunContext::addConsumer(const std::string& name, base::BaseConsumer* consumer)
{
    impl_.consumers_[name] = consumer;
}

void RunContext::addProducer(const std::string& name, base::BaseProducer* producer)
{
    impl_.producers_[name] = producer;
}

base::BaseConsumer*
RunContext::getConsumer(const std::string& name)
{
    std::map<std::string,base::BaseConsumer*>::iterator it = impl_.consumers_.find(name);
    if (it == impl_.consumers_.end()) return 0;

    return it->second;
}

base::BaseProducer*
RunContext::getProducer(const std::string& name)
{
    std::map<std::string,base::BaseProducer*>::iterator it = impl_.producers_.find(name);
    if (it == impl_.producers_.end()) return 0;

    return it->second;
}

base::Result&
RunContext::getLastResult() const
{
    return impl_.lastResult_;
}

void RunContext::setLastResult(const base::Result& result)
{
    impl_.lastResult_ = result;
}

void RunContext::removeConsumer(const std::string& name)
{
    std::map<std::string,base::BaseConsumer*>::iterator it = impl_.consumers_.find(name);
    if (it != impl_.consumers_.end())
    {
        impl_.consumers_.erase(it);
    }
}

void RunContext::removeProducer(const std::string& name)
{
    std::map<std::string,base::BaseProducer*>::iterator it = impl_.producers_.find(name);
    if (it != impl_.producers_.end())
    {
        impl_.producers_.erase(it);
    }
}

void RunContext::setupLogs(const std::string& logConfig)
{
    impl_.logHandler_.setup(logConfig);
}

