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

#include <string>
#include "serialize.h"
#include "tobjectiterator.h"
#include "tobjecttree.h"


namespace aft
{
namespace base
{
// Forward reference
class Context;

typedef std::string TObjectKey;


/**
 *  The root of all (evil) testing.
 *  Everything that can appear in a test case is derived from this class.
 */
class TObject : public SerializeContract
{
public:
    enum State
    {
        INVALID = -1,
        UNINITIALIZED,
        INITAL,
        PREPARED,
        RUNNING,
        PAUSED,
        STOPPED,
        FINISHED_BAD,
        FINISHED_GOOD
    };

    TObject(const std::string& name = std::string());

    virtual ~TObject();

    const std::string& getName() const;
    State getState() const;
    virtual TObject& run(Context* context);

    virtual bool operator==(const TObject& other);

    // Implement SerializeContract
    virtual Blob* serialize();
    virtual bool deserialize(const Blob* blob);

protected:
    std::string name_;
    State state_;
};


typedef TObjectTree Children;

/**
 *  Test object that contains other test objects.
 *  It contains a tree of of child test objects and a set of visitors.
 */
class TObjectContainer : public TObject
{
public:

    typedef TObjectIterator iterator;

    bool add(TObject* testObject);
    TObject* find(const TObjectKey& key);
    bool remove(TObject* testObject);


    // Visitors
    virtual TObject& run(Context* context);

    // Override parent SerializeContract
    virtual Blob* serialize();
    virtual bool deserialize(const Blob* blob);

protected:
    TObjectContainer(const std::string& name = std::string());
    virtual ~TObjectContainer();

    Children* children_;
};

} // namespace base
} // namespace aft
