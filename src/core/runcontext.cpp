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
#include "loghandler.h"
#include "runcontext.h"
using namespace aft;
using namespace aft::core;


class aft::core::RunContextImpl
{
public:
    RunContextImpl(LogHandler& logHandler);
    ~RunContextImpl();

    LogHandler& logHandler_;
    std::map<std::string, base::Consumer*> consumers_;
    std::map<std::string, base::Producer*> producers_;
};

RunContextImpl::RunContextImpl(LogHandler& logHandler)
    : logHandler_(logHandler)
{
}

RunContextImpl::~RunContextImpl()
{
    delete &logHandler_;
}


RunContext::RunContext()
    : impl_(*new RunContextImpl(*new LogHandler))
{
}

RunContext::~RunContext()
{
    delete &impl_;
}

void RunContext::addConsumer(const std::string& name, base::Consumer* consumer)
{
    impl_.consumers_[name] = consumer;
}

void RunContext::addProducer(const std::string& name, base::Producer* producer)
{
    impl_.producers_[name] = producer;
}

base::Consumer*
RunContext::getConsumer(const std::string& name)
{
    std::map<std::string,base::Consumer*>::iterator it = impl_.consumers_.find(name);
    if (it == impl_.consumers_.end()) return 0;

    return it->second;
}

base::Producer*
RunContext::getProducer(const std::string& name)
{
    std::map<std::string,base::Producer*>::iterator it = impl_.producers_.find(name);
    if (it == impl_.producers_.end()) return 0;

    return it->second;
}

void RunContext::setupLogs(const std::string& logConfig)
{
    impl_.logHandler_.setup(logConfig);
}


