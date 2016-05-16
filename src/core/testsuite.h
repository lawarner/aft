#pragma once
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

#include <map>

#include "base/tobject.h"

namespace aft
{
namespace base
{
    class Context;
}

namespace core
{
class RunContext;

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

    /** Get the value of an environment variable.
     *   @return true if value was successfully set, otherwise false.
     */
    bool getEnv(const std::string& name, std::string& value) const;

    /** Set the value of an environment variable. */
    void setEnv(const std::string& name, const std::string& value);

    /** Open test suite and prepare to run. */
    bool open();

    /** Rewind test suite and prepare to run again, if possble. */
    bool rewind(base::Context* context);

    /**
     *  Run test suite using context.
     *
     *  @param context Context to run test suite.
     *  @param stopOnError if true then stops when a test case fails.
     *  @return result of running test cases (summary of run)
     */
    const base::Result run(base::Context* context, bool stopOnError = false);

    /** Close the testcase. */
    void close();

    // implement SerializeContract interface
    virtual bool serialize(base::Blob& blob);
    virtual bool deserialize(const base::Blob& blob);

private:
    void copyEnv(RunContext* runContext) const;

private:
    /** Global environment for the test suite which is copied to the RunContext
     *  used for running the testcases.
     *  This may change to a PropertyHandler instead. Or maybe allow testsuites to 
     *  own a Context so parts of it can be de/serialized.
     */
    std::map<std::string,std::string> environment_;
};

} // namespace core
} // namespace aft
