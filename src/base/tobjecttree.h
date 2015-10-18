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

#include <vector>
#include "tobject.h"
#include "tobjectiterator.h"
#include "serialize.h"


namespace aft
{
namespace base
{
// Forward reference

/**
 *  A simple tree based on a recursive std::vector.
 *  It provides a depth-first visitor pattern and is serializable.
 */
class TObjectTree : public SerializeContract, public TObjectIteratorContract
{
public:
    TObjectTree()
        : value_(0)
    { }
    TObjectTree(TObject* value)
        : value_(value)
    { }
    virtual ~TObjectTree() { }

public:
    /** Type of underlying storage for TObject elements */
    typedef typename std::vector<TObjectTree*> Children;
    typedef TObjectIterator iterator;

    /** Define a vistor function for TObject */
    typedef bool (*Visitor)(TObject* obj, void* data);

    TObjectTree* add(TObject* obj)
    {
        if (!obj) return 0;

        TObjectTree* wrapper = new TObjectTree(obj);
        children_.push_back(wrapper);
        return wrapper;
    }
    Children& getChildren()
    {
        return children_;
    }
    const Children& getChildren() const
    {
        return children_;
    }

    /** Find the child tree of the object among children */
    TObjectTree* find(TObject* obj)
    {
        Children::iterator it;
        for (it = children_.begin(); it != children_.end(); ++it)
        {
            if ((*it)->getValue() == obj) return *it;
        }
        return 0;
    }

    TObject* getValue() const { return value_; }

    /** Remove tree wrapper of object from children.
     *  The object itself is not deleted. */
    bool remove(TObject* obj)
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

    /**
     *  Visits this tree's value followed by each childs value.
     *
     *  @param visitor Function pointer to visitor
     *  @param data Extra opaque data past on each element visited
     */
    bool visit(Visitor visitor, void* data)
    {
        if (value_ && !visitor(value_, data))
        {
            return false;
        }
        Children::iterator it;
        for (it = children_.begin(); it != children_.end(); ++it)
        {
            if (!(*it)->visit(visitor, data))
            {
                return false;
            }
        }
        return true;
    }

    /**
     *  Visits this tree's children and stops on true.
     *
     *  @param visitor Function pointer to visitor
     *  @param data Extra opaque data past on each element visited
     */
    Children::iterator visitUntil(Visitor visitor, void* data)
    {
        Children::iterator it;
        for (it = children_.begin(); it != children_.end(); ++it)
        {
            if (visitor((*it)->getValue(), data))
            {
                break;
            }
        }
        return it;
    }

    // Implement TObjectIteratorContract
    TObjectIterator begin()
    {
        TObjectIterator iterBegin(this);
        return iterBegin;
    }

    TObjectIterator end()
    {
        TObjectIterator iterEnd(0, false);
        return iterEnd;
    }

    TObjectIterator& next(TObjectIterator& iter)
    {
        return ++iter;
    }

    // Implement SerializeContract
    virtual Blob* serialize()
    {
        return 0;
    }

    virtual bool deserialize(const Blob* blob)
    {
        return false;
    }

protected:
    /** This nodes value */
    TObject* value_;
    Children children_;
};

} // namespace base
} // namespace aft
