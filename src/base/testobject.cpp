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

#include "testobject.h"

using namespace aft::base;


TestObject::TestObject()
{
}

TestObject::~TestObject()
{
}

Blob*
TestObject::serialize()
{
    return 0;
}

bool
TestObject::deserialize(const Blob* blob)
{
    return false;
}


TestObjectContainer::TestObjectContainer()
{
}

TestObjectContainer::~TestObjectContainer()
{
}


bool TestObjectContainer::add(TestObject* testObject)
{
    children_.push_back(testObject);
    return true;
}

TestObject*
TestObjectContainer::find(const TestObjectKey& key)
{
    return 0;
}

bool TestObjectContainer::remove(TestObject* testObject)
{
    return false;
}


ContainerIterator&
TestObjectContainer::begin()
{
    iterator_.data_ = children_.begin();
    return iterator_;
}

ContainerIterator&
TestObjectContainer::end()
{
    iterator_.data_ = children_.begin();
    return iterator_;
}

ContainerIterator&
TestObjectContainer::next()
{
    ++iterator_.data_;
    return iterator_;
}

Blob*
TestObjectContainer::serialize()
{
    return 0;
}

bool
TestObjectContainer::deserialize(const Blob* blob)
{
    return false;
}
