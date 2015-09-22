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

namespace aft
{
namespace base
{
// Forward reference
template <typename D> class Iterator;


/**
 *  Contract that elements of a Iterator must implement
 */
template <typename D>
class IteratorContract
{
public:
    virtual Iterator<D>& begin() = 0;
    virtual Iterator<D>& end() = 0;
    virtual Iterator<D>& next() = 0;
};

//TODO TreeIteratorContract with hasChildren(), nextSibling()


/**
 *  Base iterator class.
 *  Elements must implement IteratorContract.
 *
 *  Template parameters:
 *   D = Data type of elements
 */
template <typename D>
class Iterator
{
public:
    Iterator<D>() { }
    Iterator<D>(D data)
        : data_(&data)
    {
    }
    Iterator& operator=(const Iterator& other)
    {
        data_ = other.data_;
        return *this;
    }

    bool operator!=(const Iterator& other)
    {
        return data_ != other.data_;
    }

    D& operator*()
    {
        return *data_;
    }

    D* operator->()
    {
        return data_;
    }

    Iterator& operator++()
    {
        return data_->next();
    }

    IteratorContract<D>* data_;
};

} // namespace base
} // namespace aft
