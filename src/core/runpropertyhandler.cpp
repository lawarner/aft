/*
 *  runpropertyhandler.cpp
 *  libaft
 *
 *  Copyright © 2017 Andy Warner. All rights reserved.
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

#include "base/result.h"
#include "loghandler.h"
#include "outlet.h"
#include "runpropertyhandler.h"
#include "testcase.h"
using namespace aft;
using namespace aft::core;

static RunPropertyHandler* globalInstance = nullptr;

static constexpr char PrefixCons[] { "cons$" };
static constexpr char PrefixProc[] { "proc$" };
static constexpr char PrefixProd[] { "prod$" };

class aft::core::RunPropertyHandlerImpl {
public:
    RunPropertyHandlerImpl(LogHandler& logHandler)
    : logHandler_(logHandler)
    , testCase_(nullptr) { }
    ~RunPropertyHandlerImpl() { delete &logHandler_; }
    
    LogHandler& logHandler_;
    TestCase* testCase_;
    base::Result lastResult_;
};

//////////////////////////////////////////////////////////////////////////////////

RunPropertyHandler::RunPropertyHandler(const std::string& name, TestCase* testCase)
    : PropertyHandler(name, base::HandlerType::Run)
    , impl_(std::make_unique<RunPropertyHandlerImpl>(*new LogHandler)) {
    if (nullptr == testCase) {
        impl_->testCase_ = new TestCase;
    } else {
        impl_->testCase_ = testCase;
    }
}

RunPropertyHandler::~RunPropertyHandler() {
}

RunPropertyHandler* RunPropertyHandler::global() {
    if (!globalInstance) {
        globalInstance = new RunPropertyHandler("_");
    }
    return globalInstance;
}

void RunPropertyHandler::setTestCase(TestCase* testCase) {
    impl_->testCase_ = testCase;
}

void RunPropertyHandler::addOutlet(const std::string& name, Outlet* outlet) {
    impl_->testCase_->addOutlet(outlet);
}

Outlet* RunPropertyHandler::getOutlet(const std::string& name) {
    return impl_->testCase_->getOutlet(name);
}

void RunPropertyHandler::removeOutlet(const std::string& name) {
    impl_->testCase_->removeOutlet(name);
}

void RunPropertyHandler::addConsumer(const std::string& name, base::ConsumerContract* consumer) {
    auto outlet = new Outlet(PrefixCons + name);
    outlet->plugin(consumer);
    impl_->testCase_->addOutlet(outlet);
}

void RunPropertyHandler::addProducer(const std::string& name, base::ProducerContract* producer) {
    auto outlet = new Outlet(PrefixProd + name);
    outlet->plugin(producer);
    impl_->testCase_->addOutlet(outlet);
}

void RunPropertyHandler::addProcess(const std::string& name, base::ProcContract* process) {
    auto procOutlet = new Outlet(PrefixProc + name);
    procOutlet->plugin(process);
    impl_->testCase_->addOutlet(procOutlet);
}

base::ConsumerContract*
RunPropertyHandler::getConsumer(const std::string& name) {
    return impl_->testCase_->getOutlet(PrefixCons + name);
}

base::ProducerContract*
RunPropertyHandler::getProducer(const std::string& name) {
    return impl_->testCase_->getOutlet(PrefixProd + name);
}

base::ProcContract*
RunPropertyHandler::getProcess(const std::string& name) {
    return impl_->testCase_->getOutlet(PrefixProc + name);
}

base::Result&
RunPropertyHandler::getLastResult() const {
    return impl_->lastResult_;
}

void RunPropertyHandler::setLastResult(const base::Result& result) {
    impl_->lastResult_ = result;
}

void RunPropertyHandler::removeConsumer(const std::string& name) {
    impl_->testCase_->removeOutlet(PrefixCons + name);
}

void RunPropertyHandler::removeProducer(const std::string& name) {
    impl_->testCase_->removeOutlet(PrefixProd + name);
}

void RunPropertyHandler::removeProcess(const std::string& name) {
    impl_->testCase_->removeOutlet(PrefixProc + name);
}

void RunPropertyHandler::setupLogs(const std::string& logConfig) {
    impl_->logHandler_.setup(logConfig);
}
