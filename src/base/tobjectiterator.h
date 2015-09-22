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
class TObjectTree;

/**
 *  Contract that elements of a Iterator must implement
 */
class TObjectIteratorContract
{
public:
    virtual TObjectIterator& begin() = 0;
    virtual TObjectIterator& end() = 0;
    virtual TObjectIterator& next() = 0;
};

/**
 *  Iterator class for TObjects.
 *
 *  This may be templatized later, but I doubt it.
 */
class TObjectIterator
{
public:
    TObjectIterator();
    TObjectIterator(TObjectTree* data);

    TObjectIterator& operator=(const TObjectIterator& other);

    bool operator!=(const TObjectIterator& other);

    TObject& operator*();

    TObject* operator->();

    TObjectIterator& operator++();

    TObjectTree* data_;
};

} // namespace base
} // namespace aft
