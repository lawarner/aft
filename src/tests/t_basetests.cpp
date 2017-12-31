/*
 *   Copyright 2015, 2016 Andy Warner
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

#include <cstring>
#include <iostream>

#include <base/blob.h>
#include <base/context.h>
#include <base/entity.h>
#include <base/factory.h>
#include <base/hasher.h>
#include <base/propertyhandler.h>
#include <base/result.h>
#include <base/structureddata.h>
#include <base/tobasictypes.h>
#include <base/tobject.h>
#include <base/tobjecttree.h>
#include <base/tobjecttype.h>
#include <core/logger.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;
using std::string;

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

class EvalVisitor : public VisitorContract
{
public:
    enum VisitType { FirstLeaf, FirstObject, FirstTrue };
    // This visitor returns true if TObject name ends with "Leaf"
    Result visit(TObject* obj, void* data)
    {
        if (!obj) return Result(false);
        VisitType visitType = FirstLeaf;
        if (data)
        {
            visitType = *(VisitType*)data;
        }
        switch (visitType)
        {
            case FirstLeaf:
            {
                std::string name = obj->getName();
                if (name.substr(name.size() - 4, std::string::npos) == "Leaf")
                {
                    return Result(true);
                }
                break;
            }
            case FirstObject:
                return Result(true);
                break;
            case FirstTrue:
                return obj->process();
            default:
                break;
        }
        return Result(false);
    }
};

class SampleContext : public Context
{
public:
    SampleContext(const std::string& name = std::string())
    : Context(evalVisitor_, name)
    { }
private:
    EvalVisitor evalVisitor_;
};

class SamplePropertyHandler : public BasePropertyHandler {
public:
    SamplePropertyHandler()
        : BasePropertyHandler("SampleProperty")
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

class SubTOBlob : public TOBlob {
public:
    SubTOBlob(const TObjectType& blobObjType, Blob* blob)
    : TOBlob(blobObjType, blob, "SubTOBlob") {
        
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

    EXPECT_EQ(handler.getValue("Setting1", ""), "123456");
    EXPECT_EQ(tobj.getName(), refTobj.getName());
    EXPECT_TRUE(tobj.getType() == refTobj.getType());
}

TEST(BasePackageTest, Context)
{
    SamplePropertyHandler handler;
    SampleContext context;
    context.addProperty("Sample", &handler);

    EXPECT_EQ(&handler, context.handler("Sample"));
    TObject tobj("tobj");
    TObject& refTobj = handler.handle(tobj);
    
    EXPECT_EQ(tobj.getName(), refTobj.getName());
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
    const std::string RootName("Root");
    const std::string ChildName("    Child");
    const std::string LeafName("        Leaf");

    SampleTOContainer root(RootName, 0);
    root.run();

    const int SIZE = 6;
    SampleTOContainer* children[SIZE];
    TObjectTree* childTrees[SIZE];
    for (int i = 0; i < SIZE; ++i)
    {
        children[i] = new SampleTOContainer(ChildName, i + 1);
        childTrees[i] = root.add(children[i]);
        SampleTOContainer* leaf = new SampleTOContainer(LeafName, (i + 1) * 10);
        leaf->setResult(Result(true));
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

    it = root.visitUntil();
    TObject* tObj = it.get();
    EXPECT_FALSE(tObj == 0);
    EXPECT_TRUE(tObj->getName() == LeafName);
    std::cout << "visitUnit stopped at: " << tObj->getName() << std::endl;

    SampleContext context("Sample");
    it = root.visitWhile(&context);
    tObj = it.get();
    EXPECT_FALSE(tObj == 0);
    EXPECT_TRUE(tObj->getName() == RootName);
    std::cout << "visitWhile stopped at: " << tObj->getName() << std::endl;

    for (int i = 0; i < SIZE; ++i)
    {
        delete children[i];
    }
}

TEST(BasePackageTest, TObjectTypes)
{
    TObjectType& totBase  = TObjectType::get(TObjectType::NameBase);
    TObjectType& totBase2 = TObjectType::get(TObjectType::NameBase);
    TObjectType& totCommand = TObjectType::get(TObjectType::NameCommand);

    EXPECT_TRUE(totBase == TObjectType::TypeBase);
    EXPECT_TRUE(totCommand == TObjectType::TypeCommand);
    EXPECT_FALSE(totCommand == TObjectType::TypeTestCase);

    EXPECT_TRUE(TObjectType::exists(TObjectType::NameBase));
    EXPECT_FALSE(TObjectType::exists("Hello?"));
    EXPECT_TRUE(totBase.name() == TObjectType::NameBase);
    EXPECT_TRUE(totCommand.name() == TObjectType::NameCommand);
    
    EXPECT_TRUE(totBase == totBase2);
    EXPECT_TRUE(totBase != totCommand);
}

TEST(BasePackageTest, BasicTypes) {
    EXPECT_EQ(TOTrue, TOTrue);
    EXPECT_NE(TOTrue, TOFalse);
    TOBool isTrue(true, "isTrue");
    EXPECT_EQ(isTrue, TOTrue);
    EXPECT_NE(isTrue, TOFalse);

    EXPECT_EQ(TOTrue.compare(TOTrue),   0);
    EXPECT_EQ(TOTrue.compare(TOFalse),  1);
    EXPECT_EQ(TOFalse.compare(TOTrue), -1);
    EXPECT_EQ(TOFalse.compare(TOFalse), 0);

    TOInteger int0(0, "zero");
    TOInteger int1(1, "one");
    TOInteger int2(2, "two");
    TOInteger intNeg(-99999, "(negative)");
    EXPECT_EQ(int1.getValue(), 1);
    EXPECT_EQ(int2.getValue(), 2);
    EXPECT_NE(int1, int2);
    EXPECT_NE(int2, intNeg);

    EXPECT_EQ(int0.compare(int0),    0);
    EXPECT_EQ(int0.compare(int1),   -1);
    EXPECT_EQ(int0.compare(int2),   -1);
    EXPECT_EQ(int0.compare(intNeg),  1);
    EXPECT_EQ(int1.compare(int1),    0);
    EXPECT_EQ(int1.compare(int2),   -1);
    EXPECT_EQ(int1.compare(intNeg),  1);
    EXPECT_EQ(int2.compare(int1),    1);
    EXPECT_EQ(int2.compare(int2),    0);
    EXPECT_EQ(int2.compare(intNeg),  1);
    EXPECT_EQ(intNeg.compare(int1), -1);
    EXPECT_EQ(intNeg.compare(int2), -1);
    EXPECT_EQ(intNeg.compare(intNeg),0);

    TOString hello("Hello");
    EXPECT_EQ(hello.getValue(), "Hello");
    TOString byebye("Bye bye");
    EXPECT_EQ(byebye.getValue(), "Bye bye");

    EXPECT_EQ(hello.compare(hello), 0);
    EXPECT_EQ(hello.compare(byebye), 1);
    EXPECT_EQ(byebye.compare(hello), -1);
    EXPECT_EQ(byebye.compare(byebye), 0);

    byebye = hello;     // Assignment operator
    EXPECT_EQ("Hello", byebye.getValue());
    EXPECT_EQ("Hello", hello.getValue());

    Blob blob("");
    EXPECT_TRUE(isTrue.serialize(blob));
    std::cout << "isTrue serialized: " << blob.getString() << std::endl;
    EXPECT_TRUE(hello.serialize(blob));
    std::cout << "hello serialized: " << blob.getString() << std::endl;
    
    TOBlob serialHello(&blob, "(hello)");
    EXPECT_EQ(serialHello.getValue(), &blob);
    
    Blob blob2("two", Blob::STRING, "Aacme");
    TOBlob toBlob2(&blob2, "(blob2)");
    
    EXPECT_EQ( 0, serialHello.compare(serialHello));
    EXPECT_EQ( 1, serialHello.compare(toBlob2));
    EXPECT_EQ(-1, toBlob2.compare(serialHello));
    EXPECT_EQ( 0, toBlob2.compare(toBlob2));
    
    Result result = hello.getValueAsResult();
    EXPECT_EQ(result.getType(), Result::STRING);
    std::string strval;
    EXPECT_TRUE(result.getValue(strval));
    EXPECT_EQ(strval, "Hello");
}

TEST(BasePackageTest, TOBlobSubClass) {
    Blob blob("subBlob");
    SubTOBlob stb(TObjectType::TypeUiCommand, &blob);
    EXPECT_TRUE(stb.getType() == TObjectType::TypeUiCommand);
    EXPECT_TRUE(TObjectType::TypeUiCommand == stb.getType());
}
    
TEST(BasePackageTest, Blob)
{
    const char* SOMEDATA = "Hello, Blob!";
    const size_t SomeDataLength = strlen(SOMEDATA);

    Blob blob("basicBlob", (void *)SOMEDATA);
    EXPECT_EQ("basicBlob", blob.getName());
    EXPECT_EQ(strncmp(SOMEDATA, (const char *)blob.getData(), SomeDataLength), 0);

    Blob subBlob("subBlob");
    blob.addMember(&subBlob);
    EXPECT_EQ(1, blob.getMembers().size());
    EXPECT_EQ(&subBlob, blob.getMembers().front());

    Blob rawBlob("rawBlob", (void *)SOMEDATA, (int)SomeDataLength + 1);
    EXPECT_EQ(rawBlob.getType(), Blob::RAWDATA);
    EXPECT_EQ(strncmp(SOMEDATA, (const char *)rawBlob.getData(), SomeDataLength), 0);
    std::cout << "Blob data string: " << rawBlob.getString() << std::endl;

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
    StructuredDataName sdParent = sdName.getParent();
    EXPECT_EQ(FullPath, sdParent.getName(true));

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

    // Construct from path and name
    StructuredDataName pathAndName(FullPath, SimpleName);
    EXPECT_EQ(pathAndName.getName(true), FullName);

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
    std::vector<std::string> names;
    EXPECT_TRUE(simpleSd.getMembers(names));
    EXPECT_TRUE(names.empty());
    EXPECT_TRUE(simpleSd.add(member, SomeValue));
    EXPECT_TRUE(simpleSd.getMembers(names));
    EXPECT_EQ(1, names.size());
    EXPECT_EQ("member", names[0]);

    std::string value;
    EXPECT_TRUE(simpleSd.get(member, value));
    EXPECT_EQ(value, SomeValue);

    // Add second member
    EXPECT_TRUE(simpleSd.add(StructuredDataName("another"), "another Value"));
    EXPECT_TRUE(simpleSd.get(StructuredDataName("another"), value));
    EXPECT_EQ(value, "another Value");
    
    EXPECT_TRUE(simpleSd.add("a.b.c", 123));
    int ival;
    EXPECT_TRUE(simpleSd.get("a.b.c", ival));

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
    EXPECT_TRUE(sd.get("id", value));
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
    EXPECT_EQ(blob.getName(), SimpleName);
    std::cout << "serialized json: " << blob.getString() << std::endl;
}
    
TEST(BasePackageTest, Hasher)
{
    enum COMMANDS { First, Second, Third, Fourth };
    const char* cmdStrings[] = { "First", "Second", "Third", "Fourth" };
    size_t numCommands = sizeof(cmdStrings) / sizeof(cmdStrings[0]);
    std::cout << "Load " << numCommands << " commands into Hasher." << std::endl;
    const std::vector<std::string> commands(cmdStrings, cmdStrings + numCommands);
    EXPECT_EQ(commands.size(), numCommands);
    
    Hasher hasher1(commands);
    for (int idx = 0; idx < numCommands; ++idx)
    {
        int hashIdx = hasher1.getHashIndex(commands[idx]);
        switch (hashIdx) {
            case First:
            {
                EXPECT_EQ(idx, 0);
                std::string strCmd = hasher1.getName(hashIdx);
                EXPECT_EQ(strCmd, commands[hashIdx]);
            }
                break;
            case Second:
                EXPECT_EQ(idx, 1);
                break;
            case Third:
                EXPECT_EQ(idx, 2);
                break;
            case Fourth:
                EXPECT_EQ(idx, 3);
                break;
            default:
                std::cout << "Unexpected hash index: " << hashIdx << std::endl;
                EXPECT_TRUE(false);
                break;
        }
    }

    // Test char array constructor
    Hasher hasher2(cmdStrings, (unsigned int) numCommands);
    for (int idx = 0; idx < numCommands; ++idx)
    {
        int hashIdx = hasher2.getHashIndex(commands[idx]);
        switch (hashIdx) {
            case First:
                EXPECT_EQ(idx, 0);
                break;
            case Second:
                EXPECT_EQ(idx, 1);
                break;
            case Third:
                EXPECT_EQ(idx, 2);
                break;
            case Fourth:
                EXPECT_EQ(idx, 3);
                break;
            default:
                std::cout << "Unexpected hash index: " << hashIdx << std::endl;
                EXPECT_TRUE(false);
                break;
        }
    }
}

TEST(BasePackageTest, Entity)
{
    TOString myObject(FullName, "myObject");
    TOString otherObject(FullName, "Other Object");
    Entity byType("basic type", TObjectType::TypeBasicType);
    Entity byName("basic type", TObjectType::TypeBasicType, "myObject");
    
    EXPECT_TRUE(byType.matches(myObject));
    EXPECT_TRUE(byType.matches(otherObject));
    EXPECT_TRUE(byName.matches(myObject));
    EXPECT_FALSE(byName.matches(otherObject));
}
/*
TEST(BasePackageTest, Predicate)
{
    BasePredicate isNull;
    BasePredicate isTrue(&TOTrue);

    EXPECT_TRUE(isNull.evaluate(PredicateContract::PredicateNull));
    EXPECT_FALSE(isTrue.evaluate(PredicateContract::PredicateNull));
    EXPECT_TRUE(isTrue.evaluate(PredicateContract::PredicateTrue));
    EXPECT_FALSE(isTrue.evaluate(PredicateContract::PredicateEqual));

    TOInteger zero(0, "zero");
    TOInteger one(1, "one");
    TOInteger two(2, "two");
    BasePredicate compare(&one, &two);
    EXPECT_FALSE(compare.evaluate(PredicateContract::PredicateNull));
    EXPECT_TRUE(compare.evaluate(PredicateContract::PredicateLessThan));
    EXPECT_TRUE(compare.evaluate(PredicateContract::PredicateLessOrEqual));
    EXPECT_FALSE(compare.evaluate(PredicateContract::PredicateEqual));
    EXPECT_FALSE(compare.evaluate(PredicateContract::PredicateGreaterThan));
    EXPECT_FALSE(compare.evaluate(PredicateContract::PredicateGreaterOrEqual));

    BasePredicate ifThenElse(&TOTrue, &one, &zero);
    EXPECT_TRUE(ifThenElse.evaluate(PredicateContract::PredicateIfThenElse));
    BasePredicate ifNotThenElse(&TOFalse, &one, &TOFalse);
    EXPECT_TRUE(ifNotThenElse.evaluate(PredicateContract::PredicateIfNotThenElse));
    BasePredicate inte2(&TOTrue, &one, &TOFalse);
    EXPECT_TRUE(inte2.evaluate(PredicateContract::PredicateIfNotThenElse));
}
*/

TEST(BasePackageTest, Operation)
{
    TObject empty("empty");
    Operation isNull(Operation::OperatorIsNull);
    Operation isTrue(Operation::OperatorIsTrue);
    EXPECT_TRUE(empty.supportsOperation(isNull));
    EXPECT_FALSE(empty.supportsOperation(isTrue));

    EXPECT_TRUE(empty.applyOperation(isNull));
    EXPECT_FALSE(empty.applyOperation(isTrue));

    TOInteger anInt(112, "anInt");
    TOInteger another(112, "another");
    TOInteger one(1, "one");
    Operation isEqual(Operation::OperatorIsEqual);
    isEqual.addObject(&another);
    Operation isGreater(Operation::OperatorIsGreaterThan);
    isGreater.addObject(&another);
    Operation increment(Operation::OperatorAdd);
    increment.addObject(&one);
    Operation setTo123(Operation::OperatorSet);
    setTo123.addParameter("123");

    EXPECT_FALSE(anInt.applyOperation(isNull));
    EXPECT_TRUE(anInt.applyOperation(isTrue));

    EXPECT_TRUE(anInt.supportsOperation(isEqual));
    EXPECT_TRUE(anInt.supportsOperation(isGreater));
    EXPECT_TRUE(anInt.supportsOperation(increment));

    EXPECT_TRUE(anInt.applyOperation(isEqual));
    EXPECT_FALSE(anInt.applyOperation(isGreater));

    EXPECT_EQ(anInt.getValue(), 112);
    EXPECT_TRUE(anInt.applyOperation(increment));
    EXPECT_EQ(anInt.getValue(), 113);
    EXPECT_FALSE(anInt.applyOperation(isEqual));
    EXPECT_TRUE(anInt.applyOperation(isGreater));

    EXPECT_TRUE(anInt.supportsOperation(setTo123));
    EXPECT_TRUE(anInt.applyOperation(setTo123));
    EXPECT_EQ(anInt.getValue(), 123);
}
    
} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
