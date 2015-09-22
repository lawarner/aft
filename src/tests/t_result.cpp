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

#include <base/result.h>
#include <base/tobject.h>
#include <core/logger.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;

namespace
{

TEST(ResultTest, Boolean)
{
    Result testResult;	// defaults to BOOLEAN
    bool value;
    testResult.setValue(true);
    ASSERT_TRUE(testResult.getValue(value));
    EXPECT_EQ(value, true);
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

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

