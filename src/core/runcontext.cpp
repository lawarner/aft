/*
 *   Copyright 2015-2017 Andy Warner
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
#include "base/result.h"
#include "loghandler.h"
#include "outlet.h"
#include "runcontext.h"
#include "testcase.h"
using namespace aft;
using namespace aft::core;

static RunContext* globalInstance = nullptr;

static constexpr char* PrefixCons = "cons$";
static constexpr char* PrefixProc = "proc$";
static constexpr char* PrefixProd = "prod$";


class aft::core::RunContextImpl {
public:
    RunContextImpl(LogHandler& logHandler, TestCase* testCase);
    ~RunContextImpl();

    LogHandler& logHandler_;
    TestCase* testCase_;
    base::Result lastResult_;
};

RunContextImpl::RunContextImpl(LogHandler& logHandler, TestCase* testCase)
    : logHandler_(logHandler)
    , testCase_(testCase) {
}

RunContextImpl::~RunContextImpl() {
    delete &logHandler_;
}

RunContext* RunContext::global() {
    if (!globalInstance) {
        globalInstance = new RunContext(new TestCase);
    }
    return globalInstance;
}

RunContext::RunContext(TestCase* testCase)
    : impl_(*new RunContextImpl(*new LogHandler, testCase)) {
}

RunContext::~RunContext() {
    delete &impl_;
}

void RunContext::addOutlet(const std::string& name, Outlet* outlet) {
    impl_.testCase_->addOutlet(outlet);
}

Outlet* RunContext::getOutlet(const std::string& name) {
    return impl_.testCase_->getOutlet(name);
}

void RunContext::removeOutlet(const std::string& name) {
    impl_.testCase_->removeOutlet(name);
}

void RunContext::addConsumer(const std::string& name, base::ConsumerContract* consumer) {
    auto outlet = new Outlet(PrefixCons + name);
    outlet->plugin(consumer);
    impl_.testCase_->addOutlet(outlet);
}

void RunContext::addProducer(const std::string& name, base::ProducerContract* producer) {
    auto outlet = new Outlet(PrefixProd + name);
    outlet->plugin(producer);
    impl_.testCase_->addOutlet(outlet);
}

void RunContext::addProcess(const std::string& name, base::ProcContract* process) {
    auto procOutlet = new Outlet(PrefixProc + name);
    procOutlet->plugin(process);
    impl_.testCase_->addOutlet(procOutlet);
}

base::ConsumerContract*
RunContext::getConsumer(const std::string& name) {
    return impl_.testCase_->getOutlet(PrefixCons + name);
}

base::ProducerContract*
RunContext::getProducer(const std::string& name) {
    return impl_.testCase_->getOutlet(PrefixProd + name);
}

base::ProcContract*
RunContext::getProcess(const std::string& name) {
    return impl_.testCase_->getOutlet(PrefixProc + name);
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

void RunContext::removeConsumer(const std::string& name) {
    impl_.testCase_->removeOutlet(PrefixCons + name);
}

void RunContext::removeProducer(const std::string& name) {
    impl_.testCase_->removeOutlet(PrefixProd + name);
}

void RunContext::removeProcess(const std::string& name) {
    impl_.testCase_->removeOutlet(PrefixProc + name);
}

void RunContext::setupLogs(const std::string& logConfig)
{
    impl_.logHandler_.setup(logConfig);
}

