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

#include "base/context.h"


namespace aft
{
namespace base
{
// Forward reference
class BaseConsumer;
class BaseProducer;
class Result;
}

namespace core
{
// Forward reference
class RunContextImpl;

/**
 *  Context used to run test cases.
 */
class RunContext : public aft::base::Context
{
public:
    RunContext();
    virtual ~RunContext();
    
    /** Get the singleton global RunContext.
     *
     *  This is used when no local context is provided.
     */
    static RunContext* global();

    void addConsumer(const std::string& name, base::BaseConsumer* consumer);
    void addProducer(const std::string& name, base::BaseProducer* producer);

    base::BaseConsumer* getConsumer(const std::string& name);
    base::BaseProducer* getProducer(const std::string& name);
    
    /** Get the result of the last command */
    base::Result& getLastResult() const;
    /** Set the last result */
    void setLastResult(const base::Result& result);

    void removeConsumer(const std::string& name);
    void removeProducer(const std::string& name);
    
    void setupLogs(const std::string& logConfig);

    // gui, dispositions, procs/prods/cons
    // Need RunVisitor

private:
    RunContextImpl& impl_;
};

} // namespace core
} // namespace aft
