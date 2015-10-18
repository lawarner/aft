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
#include "tobjecttree.h"
using namespace aft::base;

class aft::base::TObjectIteratorImpl
{
public:
    TObjectIteratorImpl(TObjectTree* tree = 0, bool parentIter = true)
        : parentIter_(parentIter)
        {
            if (tree) trees_.push_back(tree);
        }

    /** Convenience type */
    typedef std::vector<TObjectTree*>::iterator TreeIterator;

    /** Iterator is pointing to the root node. */
    bool parentIter_;

    /** Stack of trees being iterated (current tree is always top. */
    std::vector<TObjectTree*> trees_;

    /** Current position in iterator for each tree in stack */
    std::vector<TreeIterator> curr_;
};


TObjectIterator::TObjectIterator(bool atBegin)
    : root_(0)
    , impl_(*new TObjectIteratorImpl(0, atBegin))
{
}

TObjectIterator::TObjectIterator(TObjectTree* root, bool atBegin)
    : root_(root)
    , impl_(*new TObjectIteratorImpl(root, atBegin))
{
}

TObjectIterator::~TObjectIterator()
{
    delete &impl_;
}

TObjectIterator& TObjectIterator::operator=(const TObjectIterator& other)
{
    if (this != &other)
    {
        root_ = other.root_;
        impl_.parentIter_ = other.impl_.parentIter_;
        impl_.trees_ = other.impl_.trees_;
        impl_.curr_ = other.impl_.curr_;
    }
    return *this;
}

bool TObjectIterator::operator==(const TObjectIterator& other)
{
    if (this == &other) return true;
    if (root_ != other.root_) return false;
    if (impl_.parentIter_ && other.impl_.parentIter_) return true;

    if (impl_.curr_.empty() && other.impl_.curr_.empty()) return true;
    if (impl_.trees_.size() != other.impl_.trees_.size()) return false;
    if (impl_.curr_.size() != other.impl_.curr_.size()) return false;

    if (impl_.trees_.back() != other.impl_.trees_.back()) return false;
    return impl_.curr_.back() == other.impl_.curr_.back();
}

bool TObjectIterator::operator!=(const TObjectIterator& other)
{
    return !operator==(other);
}

TObject* TObjectIterator::operator*()
{
    return get();
}

TObject* TObjectIterator::operator->()
{
    return get();
/*
    if (impl_.parentIter_)
    {
        if (data_) return data_->getValue();
        return 0;
    }
    if (*impl_.curr_ == 0) return 0;
    return (*impl_.curr_)->getValue();
    return data_->getValue();
*/
}

TObjectIterator& TObjectIterator::operator++()
{
    if (impl_.parentIter_)
    {
        impl_.trees_.clear();
        impl_.curr_.clear();
        if (root_ && !root_->getChildren().empty())
        {
            impl_.trees_.push_back(root_);
            impl_.curr_.push_back(root_->getChildren().begin());
        }
        impl_.parentIter_ = false;
    } else {
#if 1
        if (impl_.curr_.empty())
        {
            root_ = 0;    // we are done
        } else {
            if (impl_.curr_.back() != impl_.trees_.back()->getChildren().end())
            {
                if (!(*impl_.curr_.back())->getChildren().empty())
                {
                    impl_.trees_.push_back(*impl_.curr_.back());
                    impl_.curr_.push_back((*impl_.curr_.back())->getChildren().begin());
                    return *this;
                }
                ++impl_.curr_.back();
            }
            while (impl_.curr_.back() == impl_.trees_.back()->getChildren().end())
            {
                impl_.trees_.pop_back();
                impl_.curr_.pop_back();
                if (impl_.curr_.empty())
                {
                    root_ = 0;
                    break;
                }
                ++impl_.curr_.back();
            }
        }
#else
        if (!impl_.trees_.empty())
        {
            if (impl_.curr_.back() != impl_.trees_.back()->getChildren().end())
            {
                if (!(*impl_.curr_.back())->getChildren().empty())
                {
                    impl_.trees_.push_back(*impl_.curr_.back());
                    impl_.curr_.push_back((*impl_.curr_.back())->getChildren().begin());
                } else {
                    ++impl_.curr_.back();
                }
            } else {
                impl_.trees_.pop_back();
                impl_.curr_.pop_back();
                if (!impl_.curr_.empty()) ++impl_.curr_.back();
            }
        }
#endif
    }

    return *this;
}

TObject* TObjectIterator::get()
{
    if (impl_.parentIter_)
    {
        if (root_) return root_->getValue();
        return 0;
    }
    if (impl_.curr_.empty()) return 0;
    if (impl_.curr_.back() == impl_.trees_.back()->getChildren().end()) return 0;

    return (*impl_.curr_.back())->getValue();
}
