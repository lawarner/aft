#pragma once
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

#include "base/context.h"

namespace aft {
namespace base {
// Forward reference
class ConsumerContract;
class ProcContract;
class ProducerContract;
class Result;
}

namespace core {
// Forward reference
class Outlet;
class RunContextImpl;
class TestCase;

/**
 *  Context used to run test cases.
 */
class RunContext : public aft::base::Context {
public:
    RunContext(TestCase* testCase);
    virtual ~RunContext();
    
    /** Get the singleton global RunContext.
     *
     *  This is used when no local context is provided.
     */
    static RunContext* global();

    void addOutlet(const std::string& name, Outlet* outlet);
    Outlet* getOutlet(const std::string& name);
    void removeOutlet(const std::string& name);

    void addConsumer(const std::string& name, base::ConsumerContract* consumer);
    void addProducer(const std::string& name, base::ProducerContract* producer);
    void addProcess(const std::string& name, base::ProcContract* process);

    base::ConsumerContract* getConsumer(const std::string& name);
    base::ProducerContract* getProducer(const std::string& name);
    base::ProcContract* getProcess(const std::string& name);

    /** Get the result of the last command */
    base::Result& getLastResult() const;
    /** Set the last result */
    void setLastResult(const base::Result& result);

    void removeConsumer(const std::string& name);
    void removeProducer(const std::string& name);
    void removeProcess(const std::string& name);

    void setupLogs(const std::string& logConfig);

    // gui, dispositions
    // Need RunVisitor

private:
    RunContextImpl& impl_;
};

} // namespace core
} // namespace aft
