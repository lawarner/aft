
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
class TObject;
class TObjectIterator;
class TObjectIteratorImpl;
class TObjectTree;


/**
 *  Contract that elements of a Iterator must implement
 */
class TObjectIteratorContract
{
public:
    virtual TObjectIterator begin() = 0;
    virtual TObjectIterator end() = 0;
    virtual TObjectIterator& next(TObjectIterator& iter) = 0;
};

/**
 *  Iterator class for TObjects.
 *
 *  This may be templatized later, but I doubt it.
 *  The implementation of the iterator is hidden in the TObjectIteratorImpl
 *  opaque inner class.
 */
class TObjectIterator
{
public:
    /**
     *  Construct TObject iterator.
     *
     *  @param root Starts iterator pointing to root of tree.
     *  @param atBegin If true then iterator starts pointing to first child,
     *                 otherwise points to end().
     */
    TObjectIterator(TObjectTree* root = 0, bool atBegin = true);
    /** Destruct a TObject iterator. */
    ~TObjectIterator();

    TObject* get();

    TObjectIterator& operator=(const TObjectIterator& other);

    bool operator==(const TObjectIterator& other);
    bool operator!=(const TObjectIterator& other);

    TObject* operator*();

    TObject* operator->();

    TObjectIterator& operator++();

private:
    TObjectTree* root_;
    TObjectIteratorImpl& impl_;
};

} // namespace base
} // namespace aft
