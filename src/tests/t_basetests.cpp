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
#include <base/blob.h>
#include <base/context.h>
#include <base/factory.h>
#include <base/propertyhandler.h>
#include <base/result.h>
#include <base/structureddata.h>
#include <base/tobasictypes.h>
#include <base/tobject.h>
#include <base/tobjecttree.h>
#include <core/logger.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;

const std::string FullName("one.two.three.four");
const std::string FullPath("one.two.three");
const std::string SimpleName("four");

class RunVisitor : public VisitorContract
{
public:
    virtual Result visit(TObject* obj, void* data)
    {
        if (!obj || !data) return Result(false);

        TOString* message = (TOString *) data;
        std::cout << obj->getName() << ": " << message->getValue() << std::endl;
        return Result(true);
    }

};

static Result runVisitor(TObject* obj, void* data)
{
    if (!obj || !data) return false;

    TOString* message = (TOString *) data;
    std::cout << obj->getName() << ": " << message->getValue() << std::endl;
    return Result(true);
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

    std::cout << "Visit using new style visitor class." << std::endl;
    RunVisitor rv;
    tree.visit(rv, &message);
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

    Blob rawBlob("rawBlob", (void *)SOMEDATA);
    EXPECT_EQ(rawBlob.getType(), Blob::RAWDATA);
    EXPECT_EQ(strncmp(SOMEDATA, (const char *)rawBlob.getData(), 0), 0);

    std::string aString("This is another string.");
    Blob stringBlob("stringBlob", Blob::STRING, aString);
    EXPECT_EQ(stringBlob.getType(), Blob::STRING);
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

    MecFactory* mec = MecFactory::instance();
    mec->addFactory(&factory);
    TObject* tobj2 = mec->construct(categoryName, objectName);
    EXPECT_TRUE(tobj2 != 0);
    EXPECT_EQ(tobj2->getName(), objectName);

    mec->removeFactory(&factory);
    TObject* tobj0 = mec->construct(categoryName, "another");
    EXPECT_TRUE(tobj0 == 0);

    free(tobj);
    free(tobj2);
    factory.deinit();    //TODO test setDeinit

}

TEST(BasePackageTest, StructuredDataName)
{
    // Construct simple, one-component name from string
    StructuredDataName simpleName(SimpleName);
    EXPECT_EQ(SimpleName, simpleName.getName(true));
    EXPECT_EQ(SimpleName, simpleName.getName());
    EXPECT_EQ("", simpleName.getPath());
    EXPECT_EQ(simpleName.getComponents().size(), 1);

    // Construct full name from string
    StructuredDataName sdName(FullName);
    EXPECT_EQ(FullName,   sdName.getName(true));
    EXPECT_EQ(SimpleName, sdName.getName());
    EXPECT_EQ(FullPath,   sdName.getPath());

    const std::vector<std::string>& path = sdName.getComponents();
    EXPECT_EQ(path.size(), 4);
    std::vector<std::string>::const_iterator it;
    for (it = path.begin(); it != path.end(); ++it)
    {
        std::cout << "path: " << *it << std::endl;
    }

    // Construct from vector
    StructuredDataName otherName(path);
    EXPECT_EQ(sdName.getName(), otherName.getName());
    EXPECT_EQ(sdName.getName(true), otherName.getName(true));
    EXPECT_TRUE(sdName == otherName);

    // Construct empty name from string
    StructuredDataName emptyName("");
    EXPECT_EQ(emptyName.getName(), "");
    EXPECT_EQ(emptyName.getName(true), "");
    EXPECT_EQ(emptyName.getComponents().size(), 0);

    // Construct special names that start/end with SEPARATOR
    StructuredDataName specialName(".starts.with.dot");
    std::cout << "Special name: " << specialName.getName() << std::endl;
    std::cout << "Special full name: " << specialName.getName(true) << std::endl;
    std::cout << "Special path size = " << specialName.getComponents().size() << std::endl;
    EXPECT_TRUE(specialName.getComponents().front().empty());

    StructuredDataName arrayName("ends.with.dot.");
    std::cout << "Array name: " << arrayName.getName() << std::endl;
    std::cout << "Array full name: " << arrayName.getName(true) << std::endl;
    std::cout << "Array path size = " << arrayName.getComponents().size() << std::endl;
    EXPECT_TRUE(arrayName.getComponents().back().empty());
}

TEST(BasePackageTest, StructuredData)
{
    const std::string SomeValue("someValue");
    StructuredDataName member("member");
    StructuredDataName simpleSdName(SimpleName);
    StructuredData simpleSd(simpleSdName);
    EXPECT_TRUE(simpleSd.add(member, SomeValue));

    std::string value;
    EXPECT_TRUE(simpleSd.get(member, value));
    EXPECT_EQ(value, SomeValue);

    // Add second member
    EXPECT_TRUE(simpleSd.add(StructuredDataName("another"), "another Value"));
    EXPECT_TRUE(simpleSd.get(StructuredDataName("another"), value));
    EXPECT_EQ(value, "another Value");

    // Compound structure
    StructuredDataName compoundName("Compound");
    StructuredData compound(compoundName);
    EXPECT_TRUE(compound.add(member, simpleSd));
    StructuredData sd(StructuredDataName(""));
    EXPECT_TRUE(compound.get(member, sd));
    EXPECT_TRUE(sd.get(member, value));
    EXPECT_EQ(value, SomeValue);

    // Array
    EXPECT_TRUE(simpleSd.addArray(StructuredDataName("myarray")));
    EXPECT_TRUE(simpleSd.isArray(StructuredDataName("myarray")));

    EXPECT_TRUE(simpleSd.add(StructuredDataName("myarray."), "first"));
    EXPECT_TRUE(simpleSd.add(StructuredDataName("myarray."), "second"));
    EXPECT_TRUE(simpleSd.add(StructuredDataName("myarray."), 3));
    EXPECT_TRUE(simpleSd.add(StructuredDataName("myarray."), compound));

    EXPECT_TRUE(simpleSd.get("myarray.1", value));
    EXPECT_EQ(value, "second");

    std::vector<std::string> values;
    EXPECT_TRUE(simpleSd.getArray("myarray", values));
    std::vector<std::string>::iterator it;
    std::cout << " CONTENTS OF ARRAY:" << std::endl;
    for (it = values.begin(); it != values.end(); ++it)
    {
        std::cout << "+Value: " << *it << std::endl;
    }

    Blob blob("");
    EXPECT_TRUE(simpleSd.serialize(blob));
    std::cout << "Array object:  " << blob.getString() << std::endl;
}

TEST(BasePackageTest, StructuredDataParse)
{
    const std::string jsonStr("{ \"id\": \"simple name\",\n"
                              "  \"member\": {\n"
                              "              \"first\": 54321,\n"
                              "              \"another\": \"anotherValue\"\n"
                              "} }\n");
    StructuredData sd(StructuredDataName(SimpleName), jsonStr);
    StructuredDataName first("first");
    StructuredDataName another("another");
    StructuredDataName member("member");

    std::string value;
    EXPECT_TRUE(sd.get(StructuredDataName("id"), value));
    EXPECT_EQ(value, "simple name");

    StructuredData subSd(StructuredDataName(""));
    EXPECT_TRUE(sd.get(member, subSd));
    EXPECT_TRUE(subSd.get(another, value));
    EXPECT_EQ(value, "anotherValue");
    int ivalue;
    EXPECT_TRUE(subSd.get(first, ivalue));
    EXPECT_EQ(ivalue, 54321);

    Blob blob("");
    EXPECT_TRUE(sd.serialize(blob));

    EXPECT_TRUE(blob.getType() == Blob::STRING);
    EXPECT_EQ(blob.getName(), "");
    std::cout << "serialized json: " << blob.getString() << std::endl;
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
