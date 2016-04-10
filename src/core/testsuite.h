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

#include "base/tobject.h"

namespace aft
{
namespace base
{
    class Context;
}

namespace core
{
/**
 *  Test suite is a collection of test cases.
 *
 *  Each test case is opened, run and closed in order.
 *
 * TODO mode: run, edit, view
 * TODO outlets, variables
 */
class TestSuite : public aft::base::TObjectContainer
{
public:
    /** Construct test suite with an optional name. */
    TestSuite(const std::string& name = std::string());
    /** Destruct test suite */
    ~TestSuite();

    /** Open test suite and prepare to run. */
    bool open();

    bool rewind(base::Context* context);
    /**
     *  Run test suite using context.
     *
     *  @param context Context to run test suite.
     *  @param stopOnError if true then stops when a test case fails.
     *  @return result of running test cases (summary of run)
     */
    const base::Result run(base::Context* context, bool stopOnError = false);
    void close();
};

} // namespace core
} // namespace aft
