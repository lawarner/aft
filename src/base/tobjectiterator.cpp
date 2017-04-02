/*
 *   Copyright 2015-2017 Andy Warner
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
    /** Convenience type */
    using  TreeIterator = std::vector<TObjectTree*>::iterator;
    
    TObjectIteratorImpl(TObjectTree* tree = nullptr, bool parentIter = true)
        : root_(tree)
        , parentIter_(parentIter) {
        if (tree) trees_.push_back(tree);
    }

    TObject* get() const;
    
    void increment(std::size_t num = 1);
    
    bool isEqual(TObjectIteratorImpl& other) const;
    
    TObjectTree* root_;

    /** Indicate if iterator is pointing to the root node. */
    bool parentIter_;

    /** Stack of trees being iterated (current tree is always top. */
    std::vector<TObjectTree*> trees_;

    /** Current position in iterator for each tree in stack */
    std::vector<TreeIterator> curr_;
};

TObject*
TObjectIteratorImpl::get() const {
    if (parentIter_) {
        if (root_) return root_->getValue();
        return nullptr;
    }
    if (curr_.empty() ||
        curr_.back() == trees_.back()->getChildren().end()) {
        return nullptr;
    }
    
    return (*curr_.back())->getValue();
}

void TObjectIteratorImpl::increment(std::size_t num) {
    for (size_t idx = 0; idx < num && root_ != nullptr; ++idx) {
        if (parentIter_) {
            trees_.clear();
            curr_.clear();
            if (!root_->getChildren().empty()) {
                trees_.push_back(root_);
                curr_.push_back(root_->getChildren().begin());
            }
            parentIter_ = false;
        } else {
            if (curr_.empty()) {
                root_ = 0;    // we are done
            } else {
                if (curr_.back() != trees_.back()->getChildren().end()) {
                    if (!(*curr_.back())->getChildren().empty()) {
                        trees_.push_back(*curr_.back());
                        curr_.push_back((*curr_.back())->getChildren().begin());
                        return;
                    }
                    ++curr_.back();
                }
                while (curr_.back() == trees_.back()->getChildren().end()) {
                    trees_.pop_back();
                    curr_.pop_back();
                    if (curr_.empty()) {
                        root_ = 0;
                        break;
                    }
                    ++curr_.back();
                }
            }
        }
    }
}

bool TObjectIteratorImpl::isEqual(TObjectIteratorImpl& other) const {
    if (root_ != other.root_) return false;
    if (parentIter_ && other.parentIter_) return true;
    
    if (curr_.empty() && other.curr_.empty()) return true;
    if (trees_.size() != other.trees_.size()) return false;
    if (curr_.size() != other.curr_.size()) return false;
    
    if (trees_.back() != other.trees_.back()) return false;
    return curr_.back() == other.curr_.back();
}

//////////////////////////////////////////////////////////////////

TObjectIterator::TObjectIterator(TObjectTree* root, bool atBegin)
    : impl_(std::make_unique<TObjectIteratorImpl>(root, atBegin)) {
}

TObjectIterator::TObjectIterator(const TObjectIterator& other)
: impl_(nullptr) {
    if (other.impl_) {
        impl_ = std::make_unique<TObjectIteratorImpl>(*other.impl_);
    }
}

TObjectIterator::~TObjectIterator() {
}

TObjectIterator&
TObjectIterator::operator=(const TObjectIterator& other)
{
    if (this != &other) {
        if (other.impl_ == nullptr) {
            impl_.reset();
        }
        else {
            *impl_ = *other.impl_;
        }
    }
    return *this;
}

bool TObjectIterator::operator==(const TObjectIterator& other)
{
    if (this == &other) return true;
    return impl_->isEqual(*other.impl_);
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

TObjectIterator&
TObjectIterator::operator++() {
    impl_->increment(1);
    return *this;
}

TObject* TObjectIterator::get() {
    return impl_->get();
}
