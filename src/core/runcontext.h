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

namespace core {
// Forward reference
class RunContextImpl;
class TestCase;

/**
 *  Context used to run test cases.
 *  The constructor for this class adds RunPropertyHandler and other
 *  properties.
 */
class RunContext : public aft::base::Context {
public:
    RunContext(const std::string& name, TestCase* testCase);
    virtual ~RunContext();
    
    /** Get the singleton global RunContext.
     *
     *  This is used when no local context is provided.
     */
    static RunContext* global();

    //TODO Need RunVisitor

private:
    RunContextImpl& impl_;
};

} // namespace core
} // namespace aft
