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
        , tObjectIter_(this)
        , parentIter_(true)
    { }
    TObjectTree(TObject* value)
        : value_(value)
        , tObjectIter_(this)
        , parentIter_(true)
    { }
    virtual ~TObjectTree() { }

public:
    /** Type of underlying storage for TObject elements */
    typedef typename std::vector<TObjectTree*> Children;
    typedef TObjectIterator iterator;

    /** Define a vistor function for TObject */
    typedef bool (*Visitor)(TObject* obj, void* data);

    bool add(TObject* obj)
    {
        children_.push_back(new TObjectTree(obj));
        return true;
    }
    const Children& getChildren() const
    {
        return children_;
    }

    TObject* getValue() const { return value_; }

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
        if (!visitor(value_, data))
        {
            return false;
        }
        Children::iterator it;
        for (it = children_.begin(); it != children_.end(); ++it)
        {
            if (!visitor((*it)->getValue(), data))
            {
                return false;
            }
        }
        return true;
    }

    // Implement TObjectIteratorContract
    TObjectIterator& begin()
    {
        parentIter_ = true;
        return tObjectIter_;
    }

    TObjectIterator& end()
    {
        parentIter_ = false;
        curr_ = children_.end();
        return tObjectIter_;
    }

    TObjectIterator& next()
    {
        if (parentIter_ == true)
        {
            parentIter_ = false;
            curr_ = children_.begin();
        } else {
            ++curr_;
        }
        return tObjectIter_;
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
    /** Current position in iterator */
    Children::iterator curr_;		// This is bad and will be moved ...
    TObjectIterator tObjectIter_;
    bool parentIter_;
};

} // namespace base
} // namespace aft
