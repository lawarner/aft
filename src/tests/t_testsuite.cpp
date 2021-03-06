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

#include <memory>
#include <string>
#include <vector>

#include <base/context.h>
#include <core/basiccommands.h>
#include <core/logger.h>
#include <core/testcase.h>
#include <core/testsuite.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;


class SampleContext : public Context {
public:
    SampleContext(const std::string& name)
        : Context(name) { }
    SampleContext(VisitorContract& visitor, const std::string& name)
        : Context(visitor, name) { }
};


namespace
{

class TestSuiteTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        runVisitor_ = std::make_unique<ProcessVisitor>();
        context_ = std::make_unique<SampleContext>(*runVisitor_, "sample context");
    }
    virtual void TearDown() {

    }

    void createSimple(const std::string& name, bool withVisitor = false) {
        testCase_.setName(name + " case");
        if (withVisitor) {
            context_.reset(new SampleContext(*runVisitor_, name + " context"));
            //  = new SampleContext(*runVisitor_, name + " context");
        } else {
            context_.reset(new SampleContext(name + " context"));
        }
        testCase_.add(new LogCommand("Message that is first."));
        testCase_.add(new LogCommand("Message number 2."));
        
        testSuite_.setName(name + " suite");
        testSuite_.add(&testCase_);
    }

    void createTwoCases(const std::string& name, bool withVisitor = false) {
        createSimple(name, withVisitor);
        
        testCase2_.setName(name + " case 2");
        if (withVisitor) {
            context_.reset(new SampleContext(*runVisitor_, name + " context"));
            //  = new SampleContext(*runVisitor_, name + " context");
        } else {
            context_.reset(new SampleContext(name + " context"));
        }
        testCase2_.add(new LogCommand("Test case 2."));
        testCase2_.add(new LogCommand("Test case 2 end."));

        testSuite_.add(&testCase2_);
    }

protected:
    std::unique_ptr<ProcessVisitor> runVisitor_;
    std::unique_ptr<SampleContext> context_;
    TestSuite testSuite_;
    TestCase  testCase_;
    TestCase  testCase2_;
};

TEST_F(TestSuiteTest, EmptyTestCase)
{
    testCase_.setName("empty test case");
    EXPECT_TRUE(testCase_.open());
    aftlog << "Going to start test case." << std::endl;
    Result result = testCase_.run(0);
    EXPECT_FALSE(!result);
    testCase_.close();
}

TEST_F(TestSuiteTest, EmptyTestSuite)
{
    testSuite_.setName("empty test suite");
    EXPECT_TRUE(testSuite_.open());

    Result result = testSuite_.run(0);
    EXPECT_FALSE(!result);
    testSuite_.close();
}

TEST_F(TestSuiteTest, SimpleTestCase)
{
    createSimple("simple test");
    EXPECT_TRUE(testCase_.open());
    aftlog << "Going to start test case." << std::endl;
    Result result = testCase_.run(context_.get());
    EXPECT_FALSE(!result);
    testCase_.close();
}

TEST_F(TestSuiteTest, SimpleTestCaseVisitor)
{
    createSimple("test with visitor", true);
    EXPECT_TRUE(testCase_.open());
    aftlog << "Going to start test case." << std::endl;
    Result result = testCase_.run(context_.get());
    EXPECT_FALSE(!result);
    testCase_.close();
}

TEST_F(TestSuiteTest, SimpleTestSuite) {
    createSimple("simple test");
    EXPECT_TRUE(testSuite_.open());
    aftlog << "Going to start test suite." << std::endl;
    Result result = testSuite_.run(context_.get());
    EXPECT_FALSE(!result);
    testSuite_.close();
}

TEST_F(TestSuiteTest, SimpleTestSuiteVisitor) {
    createSimple("simple test with visitor", true);
    EXPECT_TRUE(testSuite_.open());
    aftlog << "Going to start test suite." << std::endl;
    Result result = testSuite_.run(context_.get());
    EXPECT_FALSE(!result);
    testSuite_.close();
}

TEST_F(TestSuiteTest, TwoCaseTestSuite) {
    createTwoCases("two test cases test");
    EXPECT_TRUE(testSuite_.open());
    aftlog << "Going to start test suite." << std::endl;
    Result result = testSuite_.run(context_.get());
    EXPECT_FALSE(!result);
    testSuite_.close();
}
    
TEST_F(TestSuiteTest, TwoCaseTestSuiteVisitor) {
    createTwoCases("two test cases test with visitor", true);
    EXPECT_TRUE(testSuite_.open());
    aftlog << "Going to start test suite." << std::endl;
    Result result = testSuite_.run(context_.get());
    EXPECT_FALSE(!result);
    testSuite_.close();
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
