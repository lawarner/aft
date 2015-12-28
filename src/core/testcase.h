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

#include <string>
#include "base/tobject.h"

namespace aft
{
// Forward reference
namespace base
{
    class Blob;
    class Context;
}

namespace core
{

/**
 * Contains a test case that can be run.
 */
class TestCase : public aft::base::TObjectContainer
{
public:
    TestCase(const std::string& name = std::string());
    virtual ~TestCase();

    /** Open the test case */
    virtual bool open();

    /** Rewind the test case.
     *
     *  It is not always possible to rewind/reset a testcase.
     *  @return true if testcase was succesfully rewound.
     */
    virtual bool rewind(base::Context* context);

    /**
     *  Run the test case within a context.
     *
     *  The test case is run by iterating the objects in this container
     *  and executing them in the context.
     *
     *  @param context The context (environment) where objects are run.  The Context
     *         past in will likely be modified as a side effect of running.
     *  @return A TObject, typically a Result specifying the final outcome of running
     *          this test case.
     */
    virtual const base::Result run(base::Context* context);

    /** Close the test case */
    void close();

    //TODO Branch(true, false, exception)

    // implement SerializeContract interface
    virtual bool serialize(base::Blob& blob);
    virtual bool deserialize(const base::Blob& blob);

private:

};

} // namespace core
} // namespace aft
