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

#include "base/blob.h"
#include "base/context.h"
#include "base/result.h"
#include "base/structureddata.h"
#include "base/tobjecttree.h"
#include "base/tobjecttype.h"
#include "core/logger.h"
#include "core/runcontext.h"
#include "core/testcase.h"
#include "core/testsuite.h"
using namespace aft;
using namespace aft::core;


TestSuite::TestSuite(const std::string& name)
: TObjectContainer(base::TObjectType::TypeTestSuite, name) {
    state_ = INITIAL;
}

void TestSuite::copyEnv(RunContext* runContext) const
{
    for (auto it = environment_.begin(); it != environment_.end(); ++it)
    {
        runContext->setEnv(it->first, it->second);
    }
}

bool TestSuite::getEnv(const std::string& name, std::string& value) const
{
    auto it = environment_.find(name);
    if (it == environment_.cend())  return false;

    value = it->second;
    return true;
}

void TestSuite::setEnv(const std::string& name, const std::string& value)
{
    environment_[name] = value;
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
    //TODO copy environment
    return TObjectContainer::rewind(context);
}

const base::Result
TestSuite::run(base::Context* context, bool stopOnError)
{
    base::Result result(true);
    if (state_ == PREPARED && children_)
    {
        aftlog << "Running test suite \"" << getName() << "\"" << std::endl;
        RunContext* runContext = context ? dynamic_cast<RunContext *>(context) : RunContext::global();
        copyEnv(runContext);
        
        int ranGood = 0;
        int ranBad  = 0;
        state_ = RUNNING;
        base::TObjectTree::Children& testcases = children_->getChildren();
        base::TObjectTree::Children::iterator iter;
        for (iter = testcases.begin(); iter != testcases.end(); ++iter)
        {
            TestCase* testcase = dynamic_cast<TestCase *>((*iter)->getValue());
            if (!testcase) continue;
            const std::string testcaseName = "test case \"" + testcase->getName() + "\"";

            aftlog << " - Running " << testcaseName << std::endl;
            if (!testcase->open())
            {
                aftlog << " - Error: cannot open " << testcaseName << std::endl;
                continue;
            }
            result = testcase->run(context);
            testcase->close();
            if (!result) {
                aftlog << " - FAILED " << testcaseName << std::endl;
                ++ranBad;
                if (stopOnError || result.getType() == base::Result::FATAL) {
                    break;
                }
            }
            else {
                aftlog << " - SUCCESS " << testcaseName << std::endl;
                ++ranGood;
            }
        }
        
        aftlog << "Finished test suite: " << ranGood << " test cases succeeded, "
               << ranBad << " test cases failed." << std::endl;
        
        if (ranBad > 0) {
            result = base::Result(false);
            state_ = FINISHED_BAD;
        }
        else {
            state_ = FINISHED_GOOD;
        }
    }
    else {
        if (children_) {
            aftlog << "Error: test suite \"" << getName() << "\" not opened." << std::endl;
            result = base::Result(false);
        }
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


bool TestSuite::serialize(base::Blob& blob) {
    if (!TObject::serialize(blob)) {
        return false;
    }

    base::StructuredData sd(base::TObjectType::NameTestSuite, blob);

    sd.addArray("environment");
    for (const auto& envvar : environment_) {
        base::StructuredData sdEnvar(envvar.first, envvar.second);
        sd.add("environment.", sdEnvar);
    }

    sd.addArray("testcases");
    base::TObjectTree::Children& testcases = children_->getChildren();
    for (const auto testcase : testcases) {
        TObject* tObj = testcase->getValue();
        if (tObj) {
            base::Blob tcBlob("");
            tObj->serialize(tcBlob);
            base::StructuredData sdtc("testcase", tcBlob.getString());
            sd.add("testcases.", sdtc);
        }
    }
    
    return sd.serialize(blob);
}

bool TestSuite::deserialize(const base::Blob& blob)
{
    if (!TObject::deserialize(blob))
    {
        return false;
    }

    base::StructuredData sd("");
    if (!sd.deserialize(blob))
    {
        aftlog << loglevel(Error) << "Cannot deserialize testsuite" << std::endl;
        return false;
    }

    // environment
    std::vector<std::string> env;
    if (sd.getArray("environment", env))
    {
        std::vector<std::string>::const_iterator it;
        for (it = env.cbegin(); it != env.cend(); ++it)
        {
            const std::string name = *it;
            const std::string envVar("environment." + name);
            std::string value = sd.get(envVar);
            setEnv(name, value);
        }
    }
    
    // testcases
    std::vector<std::string> testcases;
    if (!sd.getArray("testcases", testcases))
    {
        return false;
    }
    
    std::vector<std::string>::const_iterator it;
    for (it = testcases.begin(); it != testcases.end(); ++it)
    {
        base::StructuredData sdParams("", *it);
        std::string testcaseName = sdParams.get("name");
        base::Blob params(testcaseName, base::Blob::STRING, *it);
        base::TObject* tobj = new TestCase(testcaseName);
        if (!tobj->deserialize(params))
        {
            aftlog << loglevel(Error) << "Cannot deserialize testcase" << std::endl;
            delete tobj;
            return false;
        }
        add(tobj);
    }
    
    return true;
}
