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
#include "visitor.h"


namespace aft
{
namespace base
{
// Forward reference

/**
 *  A simple tree based on a recursive std::vector.
 *
 *  It provides a depth-first visitor pattern, a depth-first iterator interface
 *  and is serializable.
 *  This class has no ownership of the TObjects that it holds, thus allowing
 *  easy shared pooling of TObjects.
 *  Trees can be empty, hold a single "root" TObject and/or hold a list of children.
 *  Note that the root object is optional.
 */
class TObjectTree : public SerializeContract, public TObjectIteratorContract
{
public:
    /** Construct an empty TObjectTree */
    TObjectTree()
        : value_(0)
    { }
    /** Construct a TObjectTree with a root TObjectTree */
    TObjectTree(TObject* value)
        : value_(value)
    { }
    /** Destruct TObjectTree */
    virtual ~TObjectTree() { }

public:
    /** Type of underlying storage for TObject elements */
    typedef typename std::vector<TObjectTree*> Children;
    typedef TObjectIterator iterator;

    /** Append a TObject to the list of children. */
    TObjectTree* add(TObject* obj)
    {
        if (!obj) return 0;

        TObjectTree* wrapper = new TObjectTree(obj);
        children_.push_back(wrapper);
        return wrapper;
    }

    /** Get underlying storage of the list of children.
     *  This call should not be necessary (outside of core classes) and
     *  its use is discouraged.
     *  It may be deprecated or removed in the future. */
    Children& getChildren()
    {
        return children_;
    }

    /** Get underlying storage of the list of children as a const.
     *  This call should not be necessary (outside of core classes) and
     *  its use is discouraged.
     *  It may be deprecated or removed in the future. */
    const Children& getChildren() const
    {
        return children_;
    }

    /** Find the child tree wrapper of the object among children */
    TObjectTree* find(TObject* obj)
    {
        Children::iterator it;
        for (it = children_.begin(); it != children_.end(); ++it)
        {
            if ((*it)->getValue() == obj) return *it;
        }
        return 0;
    }

    /** Return the TObject wrapped by this tree.  It may be null. */
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
     *  Visits this tree's value followed by each childs value, recursively.
     *
     *  Child nodes in the tree are visited in a depth-first order, after first
     *  visting the root node.  Visiting stops if the visitor returns false.
     *
     *  This method will likely be deprecated in the future.
     *
     *  @param visitor Pointer to C function visitor
     *  @param data Extra opaque data past on each element visited
     *  @return false when the vistor returns false for any child, otherwise true.
     */
    Result visit(CVisitor visitor, void* data)
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

    /**
     *  Interface-based visitor.
     *  Returns true after visiting all children.  If any child TObject returns a false
     *  Result, then the TObject is returned wrapped in a Result.
     */
    Result visit(VisitorContract& visitor, void* data)
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

    /**
     *  Visits this tree's children and stops on true.
     *
     *  Only direct child nodes in the tree are visited.  This method does
     *  not vist the root node.
     *
     *  @param visitor Pointer to C function visitor
     *  @param data Extra opaque data past on each element visited
     *  @return the iterator pointing to the child that returns true
     */
    Children::iterator visitUntil(BVisitor visitor, void* data)
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
    virtual bool serialize(Blob& blob)
    {
        return false;
    }

    virtual bool deserialize(const Blob& blob)
    {
        return false;
    }

protected:
    /** The tree root value (optional). */
    TObject* value_;
    /** List of children. */
    Children children_;
};

} // namespace base
} // namespace aft
