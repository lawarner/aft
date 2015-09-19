#pragma once
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

#include <string>
#include <vector>
#include "iterator.h"
#include "serialize.h"


namespace aft
{
namespace base
{
// Forward reference
class TestObject;

typedef std::string TestObjectKey;
typedef std::vector<TestObject*> Children;
typedef Iterator<Children::iterator> ContainerIterator;


/**
 *  The root of all (evil) testing.
 *  Everything that can appear in a test case is derived from this class.
 */
class TestObject : public SerializeContract
{
public:
    // Implement SerializeContract
    virtual Blob* serialize();
    virtual bool deserialize(const Blob* blob);

protected:
    TestObject();
    virtual ~TestObject();
};


/**
 *  Test object that contains other test objects.
 *  It contains a tree of of child test objects and a set of iterators and visitors.
 */
class TestObjectContainer : public TestObject, public IteratorContract<Children::iterator>
{
public:

    typedef ContainerIterator iterator;

    bool add(TestObject* testObject);
    TestObject* find(const TestObjectKey& key);
    bool remove(TestObject* testObject);

    // Implement IteratorContract
    ContainerIterator& begin();
    ContainerIterator& end();
    ContainerIterator& next();

    // Implement SerializeContract
    virtual Blob* serialize();
    virtual bool deserialize(const Blob* blob);

protected:
    TestObjectContainer();
    virtual ~TestObjectContainer();

    Children children_;
    iterator iterator_;
};

} // namespace base
} // namespace aft
