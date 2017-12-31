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
#include "runpropertyhandler.h"
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
    RunContextImpl(TestCase* testCase)
        : propHandler("RunContextImpl", testCase) { }
    ~RunContextImpl() = default;

    RunPropertyHandler propHandler;
};

/////////////////////////////////////////////////////////////////////////////////////////
RunContext* RunContext::global() {
    if (!globalInstance) {
        globalInstance = new RunContext("Run Context (global)", new TestCase);
    }
    return globalInstance;
}

RunContext::RunContext(const std::string& name, TestCase* testCase)
    : base::Context(name)
    , impl_(*new RunContextImpl(testCase)) {
    addProperty(base::PropertyHandler::handlerTypeName(base::HandlerType::Run),
                &impl_.propHandler);
}

RunContext::~RunContext() {
    delete &impl_;
}
