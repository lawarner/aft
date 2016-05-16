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

#include <vector>
#include "blob.h"
#include "tobject.h"
#include "tobjecttree.h"
#include "visitor.h"


using namespace aft::base;

TObjectTree*
TObjectTree::add(TObject* obj)
{
    if (!obj) return 0;
    
    TObjectTree* wrapper = new TObjectTree(obj);
    children_.push_back(wrapper);
    return wrapper;
}

TObjectTree::Children&
TObjectTree::getChildren()
{
    return children_;
}

const TObjectTree::Children&
TObjectTree::getChildren() const
{
    return children_;
}

TObjectTree*
TObjectTree::find(TObject* obj)
{
    Children::iterator it;
    for (it = children_.begin(); it != children_.end(); ++it)
    {
        if ((*it)->getValue() == obj) return *it;
    }
    return 0;
}

TObject* TObjectTree::getValue() const
{
    return value_;
}

bool TObjectTree::remove(TObject* obj)
{
    Children::iterator it;
    for (it = children_.begin(); it != children_.end(); ++it)
    {
        if ((*it)->getValue() == obj)
        {
            delete *it;
            children_.erase(it);
            return true;
        }
    }
    return false;
}

Result TObjectTree::visit(CVisitor visitor, void* data)
{
    Result retval(true);
    if (value_)
    {
        retval = visitor(value_, data);
        if (!retval)
        {
            return retval;
        }
    }
    Children::iterator it;
    for (it = children_.begin(); it != children_.end(); ++it)
    {
        retval = (*it)->visit(visitor, data);
        if (!retval)
        {
            break;;
        }
    }
    return retval;
}

Result TObjectTree::visit(VisitorContract& visitor, void* data)
{
    bool boolResult;
    if (value_)
    {
        Result result = visitor.visit(value_, data);
        if (result.getValue(boolResult) && !boolResult)
        {
            return Result(value_);
        }
    }
    Children::iterator it;
    for (it = children_.begin(); it != children_.end(); ++it)
    {
        Result result = (*it)->visit(visitor, data);
        if (result.getValue(boolResult) && !boolResult)
        {
            return Result(*it);
        }
    }
    return Result(true);
}

TObjectTree::Children::iterator
TObjectTree::visitUntil(VisitorContract& visitor, void* data)
{
    Children::iterator it;
    for (it = children_.begin(); it != children_.end(); ++it)
    {
        if (visitor.visit((*it)->getValue(), data))
        {
            break;
        }
    }
    return it;
}

TObjectIterator
TObjectTree::begin()
{
    TObjectIterator iterBegin(this);
    return iterBegin;
}

TObjectIterator
TObjectTree::end()
{
    TObjectIterator iterEnd(0, false);
    return iterEnd;
}

TObjectIterator&
TObjectTree::next(TObjectIterator& iter)
{
    return ++iter;
}

bool TObjectTree::serialize(Blob& blob)
{
    return false;
}

bool TObjectTree::deserialize(const Blob& blob)
{
    return false;
}

TObjectTree&
TObjectTree::operator=(const TObjectTree& other)
{
    if (this != &other)
    {
        value_ = other.value_;
        children_ = other.children_;
    }
    return *this;
}
