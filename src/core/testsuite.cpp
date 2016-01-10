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
#include "base/result.h"
#include "base/tobjecttree.h"
#include "core/logger.h"
#include "core/testcase.h"
#include "core/testsuite.h"
using namespace aft;
using namespace aft::core;


TestSuite::TestSuite(const std::string& name)
    : TObjectContainer(name)
{
    state_ = INITIAL;
}

TestSuite::~TestSuite()
{
}


bool
TestSuite::open()
{
    if (state_ == INITIAL)
    {
        state_ = PREPARED;
        return true;
    }

    return false;
}

bool
TestSuite::rewind(base::Context* context)
{
    return TObjectContainer::rewind(context);
}

const base::Result
TestSuite::run(base::Context* context, bool stopOnError)
{
    base::Result result(false);
    if (state_ == PREPARED && children_)
    {
        aftlog << "Running test suite \"" << getName() << "\"" << std::endl;
        base::TObjectTree::Children& testcases = children_->getChildren();
        base::TObjectTree::Children::iterator iter;
        for (iter = testcases.begin(); iter != testcases.end(); ++iter)
        {
            TestCase* testcase = dynamic_cast<TestCase *>((*iter)->getValue());
            if (!testcase) continue;
            const std::string testcaseName = "test case \"" + testcase->getName() + "\"";

            aftlog << " - Running test case " << testcaseName << std::endl;
            if (!testcase->open())
            {
                aftlog << " - Error: cannot open " << testcaseName << std::endl;
                continue;
            }
            result = testcase->run(context);
            testcase->close();
            if (!result)
            {
                aftlog << " - FAILED test case " << testcase->getName() << std::endl;
                if (stopOnError) break;
            } else {
                aftlog << " - SUCCESS test case " << testcase->getName() << std::endl;
            }
        }
    }
    else
    {
        if (children_) aftlog << "Error: test suite \"" << getName() << "\" not opened." << std::endl;
    }

    return result;
}

void
TestSuite::close()
{
    //TODO check other states, i.e., if running then stop.
    if (state_ != INVALID && state_ != UNINITIALIZED)
    {
        state_ = INITIAL;
    }
}
