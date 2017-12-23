#pragma once
/*
 *   Copyright © 2015-2017 Andy Warner
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

#include "outlet.h"
#include "base/tobject.h"
#include <string>
#include <vector>

namespace aft {
namespace base {
    // Forward reference
    class Blob;
    class Context;
}

namespace core {

/**
 * Contains a test case that can be run.
 */
class TestCase : public aft::base::TObjectContainer {
public:
    using OutletList = std::vector<Outlet*>;
public:
    TestCase(const std::string& name = std::string());
    virtual ~TestCase();

    /** Open the test case
     *  @return true if testcase was succesfully opened.
     */
    virtual bool open();

    /** Rewind the test case.
     *
     *  It is not always possible to rewind/reset a testcase.
     *  @param context Context used to perform the rewind
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
     *         passed in will likely be modified as a side effect of running.
     *  @return A Result specifying the final outcome of running this test case (TOTrue,
     *          TOFalse or Result::FATAL).
     */
    virtual const base::Result run(base::Context* context);

    /** Close the test case */
    void close();

    bool addOutlet(Outlet* outlet);
    Outlet* getOutlet(const std::string& name) const;
    bool removeOutlet(const std::string& name);

    //TODO Branch(true, false, exception)

    // implement SerializeContract interface
    virtual bool serialize(base::Blob& blob) override;
    virtual bool deserialize(const base::Blob& blob) override;

private:
    OutletList outlets_;
};

} // namespace core
} // namespace aft
