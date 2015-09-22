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

#include <iostream>
#include <base/context.h>
#include <base/propertyhandler.h>
#include <base/tobject.h>
#include <core/logger.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;

class SampleContext : public Context
{
public:
    SampleContext() { }
};

class SamplePropertyHandler : public PropertyHandler
{
public:
    SamplePropertyHandler()
        : PropertyHandler("SampleProperty")
    { }
    virtual ~SamplePropertyHandler() { }

    virtual TObject& handle(const TObject& testObject)
    {
        std::cout << getName() << "::handle TObject=" << testObject.getName() << std::endl;
        return const_cast<TObject&>(testObject);
    }
};



namespace
{
TEST(BasePackageTest, PropertyHandler)
{
    SamplePropertyHandler handler;
    handler.setValue("Setting1", "123456");
    TObject tobj("tobj");
    TObject& refTobj = handler.handle(tobj);

    EXPECT_EQ(handler.getValue("Setting1"), "123456");
    EXPECT_EQ(tobj.getName(), refTobj.getName());
}

TEST(BasePackageTest, Context)
{
    SamplePropertyHandler handler;
    SampleContext context;
    context.addProperty("Sample", &handler);

    EXPECT_EQ(&handler, context.handler("Sample"));
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

