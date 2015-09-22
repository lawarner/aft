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
#include "iterator.h"
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
template <typename T>
class Tree : public SerializeContract, public IteratorContract<Tree<T> >
{
public:
    Tree() { }
    Tree(T value) : value_(value) { }
    virtual ~Tree() { }

public:
    /** Type of underlying storage for elements */
    typedef std::vector<T> Children;
    typedef Iterator<Tree<T> > TreeIterator;
    typedef TreeIterator iterator;
    /** Define a vistor function for each type T */
    typedef bool (*Visitor)(T& obj, void* data);

    bool add(T obj)
    {
        children_.push_back(obj);
        return true;
    }
    Children& getChildren() const
    {
        return children_;
    }
    bool remove(T obj)
    {
        typename Children::iterator it;
        for (it = children_.begin(); it != children_.end(); ++it)
        {
            if (*it == obj)
            {
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
        typename Children::iterator it;
        for (it = children_.begin(); it != children_.end(); ++it)
        {
            if (!visitor(*it, data))
            {
                return false;
            }
        }
        return true;
    }

    // Implement IteratorContract
    TreeIterator& begin()
    {
        curr_ = TreeIterator(this);
        return TreeIterator(*curr_);
    }

    TreeIterator& end()
    {
        curr_ = children_.end();
        return TreeIterator(*curr_);
    }

    TreeIterator& next()
    {
        if (*curr_ == this)
        {
            curr_ = children_.begin();
        } else {
            ++curr_;
        }
        return TreeIterator(*curr_);
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
    T value_;
    Children children_;
    /** Current position in iterator */
    typename Children::iterator curr_;		// This is bad and will be moved ...
};

} // namespace base
} // namespace aft
