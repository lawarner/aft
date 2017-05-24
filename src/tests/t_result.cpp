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

#include <base/command.h>
#include <base/result.h>
#include <base/tobject.h>
#include <core/logger.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;

namespace {

const std::string sampleText("The quick brown fox jumps over the lazy dog.");
    
class SetCommand : public Command {
public:
    SetCommand(int value)
    : Command("Set")
    , value_(value) {
        
    }
    virtual ~SetCommand() = default;
    virtual const Result process(Context* context = nullptr) {
        std::cout << "Processing Set command" << std::endl;
        return Result(value_);
    }
private:
    int value_;
};


TEST(ResultTest, Boolean) {
    Result testResult;	// defaults to BOOLEAN
    bool value;
    testResult.setValue(true);
    ASSERT_TRUE(testResult.getValue(value));
    ASSERT_EQ(Result::BOOLEAN, testResult.getType());
    ASSERT_TRUE(value);
    ASSERT_TRUE(testResult);
}

TEST(ResultTest, String)
{
    Result strResult(sampleText);
    ASSERT_TRUE(strResult.getType() == Result::STRING);
    ASSERT_EQ(sampleText, strResult.asString());
}

TEST(ResultTest, TObject)
{
    Result testResult(Result::TOBJECT);
    TObject obj("Test Object");
    testResult.setValue(&obj);

    TObject* objPtr;
    ASSERT_TRUE(testResult.getValue(objPtr));
    EXPECT_EQ(&obj, objPtr);
}

TEST(ResultTest, Command) {
    Result cmdResult(Result::COMMAND);
    SetCommand setter(12345);
    cmdResult.setValue(&setter);

    Command* cmdPtr;
    ASSERT_TRUE(cmdResult.getValue(cmdPtr));
    EXPECT_EQ(&setter, cmdPtr);
    cmdPtr->setState(TObject::PREPARED);
    Result ival = cmdPtr->run();
    int ii;
    EXPECT_TRUE(ival.getValue(ii));
    EXPECT_EQ(12345, ii);
}

TEST(ResultTest, NotOperator)
{
    Result testResult;	// defaults to BOOLEAN
    bool value;
    testResult.setValue(true);
    ASSERT_FALSE(!testResult);
    testResult.getValue(value);
    EXPECT_TRUE(value);

    testResult.setValue(false);
    ASSERT_TRUE(!testResult);
    testResult.getValue(value);
    EXPECT_FALSE(value);
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

