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
#include <base/factory.h>
#include <base/propertyhandler.h>
#include <base/result.h>
#include <base/tobasictypes.h>
#include <base/tobject.h>
#include <base/tobjecttree.h>
#include <base/typedblob.h>
#include <core/logger.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;

static bool runVisitor(TObject* obj, void* data)
{
    if (!obj || !data) return false;

    TOString* message = (TOString *) data;
    std::cout << obj->getName() << ": " << message->getValue() << std::endl;
    return true;
}

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

    virtual TObject& handle(const TObject& tObject)
    {
        std::cout << getName() << "::handle TObject=" << tObject.getName() << std::endl;
        return const_cast<TObject&>(tObject);
    }
};

class SampleTOContainer : public TObjectContainer
{
public:
    SampleTOContainer(const std::string& name, int ordinal)
        : TObjectContainer(name)
        , ordinal_(ordinal)
        { }
    virtual ~SampleTOContainer() { }
/*
    virtual const Result run(Context* context = 0)
        {
            std::cout << ordinal() << ": " << getName() << std::endl;
            return TObjectContainer::run(context);
        }
*/
    int ordinal() const { return ordinal_; }
private:
    int ordinal_;
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

TEST(BasePackageTest, TObjectTree)
{
    TObject root("    Root");
    TObject child("        Child");
    TObject leaf("            Leaf");
    TOString message("Message from visitor.");

    TObjectTree empty;
    std::cout << "Visit empty tree" << std::endl;
    empty.visit(runVisitor, &message);
    TObjectTree tree(&root);
    std::cout << "Visit single node tree" << std::endl;
    tree.visit(runVisitor, &message);

    const int SIZE = 3;
    for (int i = 0; i < SIZE; ++i)
    {
        TObjectTree* childTree = tree.add(&child);
        if ((i % 2) == 1) continue;
        for (int j = 0; j < SIZE; ++j)
        {
            childTree->add(&leaf);
        }
    }
    std::cout << "Visit populated tree" << std::endl;
    tree.visit(runVisitor, &message);
}

TEST(BasePackageTest, TObjectContainer)
{
    SampleTOContainer root("Root", 0);
    root.run();

    const int SIZE = 6;
    SampleTOContainer* children[SIZE];
    TObjectTree* childTrees[SIZE];
    for (int i = 0; i < SIZE; ++i)
    {
        children[i] = new SampleTOContainer("    Child", i + 1);
        childTrees[i] = root.add(children[i]);
        SampleTOContainer* leaf = new SampleTOContainer("        Leaf", (i + 1) * 10);
        children[0]->add(leaf, childTrees[0]);
        if (i >= 2) children[2]->add(leaf, childTrees[2]);
    }
    root.run();

    TObjectIterator it;
    Children* childTree = root.getChildren();
    EXPECT_TRUE(childTree != 0);
    for (it = childTree->begin(); it != childTree->end(); it = childTree->next(it))
    {
        TObject* tObj = it.get();
        //TObject* tObj = *it;
        if (tObj)
        {
            std::cout << "Got object: " << tObj->getName() << std::endl;
        } else {
            std::cout << "Iterator points to null object." << std::endl;
        }
    }

    for (int i = 0; i < SIZE; ++i)
    {
        delete children[i];
    }
}

TEST(BasePackageTest, BasicTypes)
{
    EXPECT_EQ(TOTrue, TOTrue);
    EXPECT_NE(TOTrue, TOFalse);
    TOBool isTrue(true);
    EXPECT_EQ(isTrue, TOTrue);
    EXPECT_NE(isTrue, TOFalse);

    TOString hello("Hello");
    EXPECT_EQ(hello.getValue(), "Hello");
}

TEST(BasePackageTest, Blob)
{
    const char* SOMEDATA = "Hello, Blob!";
    Blob blob("basicBlob", (void *)SOMEDATA);
    EXPECT_EQ("basicBlob", blob.getName());
    EXPECT_EQ(strncmp(SOMEDATA, (const char *)blob.getData(), 0), 0);

    Blob subBlob("subBlob");
    blob.addMember(&subBlob);
    EXPECT_EQ(1, blob.getMembers().size());
    EXPECT_EQ(&subBlob, blob.getMembers().front());

    TypedBlob rawBlob("rawBlob", (void *)SOMEDATA);
    EXPECT_EQ(rawBlob.getType(), TypedBlob::RAWDATA);
    EXPECT_EQ(strncmp(SOMEDATA, (const char *)rawBlob.getData(), 0), 0);

    std::string aString("This is another string.");
    TypedBlob stringBlob("stringBlob", TypedBlob::STRING, aString);
    EXPECT_EQ(stringBlob.getType(), TypedBlob::STRING);
    EXPECT_EQ(aString, stringBlob.getString());
}

TEST(BasePackageTest, Factory)
{
    const std::string categoryName("Base");
    const std::string factoryName("base factory");
    const std::string objectName("anObject");
    BaseFactory factory(categoryName, factoryName);
    EXPECT_EQ(factory.category(), categoryName);
    EXPECT_EQ(factory.factoryName(), factoryName);
    TObject* tobj = factory.construct(objectName);
    EXPECT_TRUE(tobj != 0);
    EXPECT_EQ(tobj->getName(), objectName);

    MecFactory mec;   // probably will become a singleton interface
    mec.addFactory(&factory);
    TObject* tobj2 = mec.construct(categoryName, objectName);
    EXPECT_TRUE(tobj2 != 0);
    EXPECT_EQ(tobj2->getName(), objectName);

    mec.removeFactory(&factory);
    TObject* tobj0 = mec.construct(categoryName, "another");
    EXPECT_TRUE(tobj0 == 0);

    free(tobj);
    free(tobj2);
    factory.deinit();    //TODO test setDeinit

}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

