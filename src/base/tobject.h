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
#include "result.h"
#include "serialize.h"
#include "tobjectiterator.h"


namespace aft
{
namespace base
{
// Forward reference
class Callback;
class Context;
class TObjectTree;
class TObjectType;


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
        INITIAL,
        PREPARED,
        RUNNING,
        PAUSED,
        STOPPED,
        FINISHED_BAD,
        FINISHED_GOOD
    };

    /** Construct a TObject with a given, optional name */
    TObject(const std::string& name = std::string());
protected:
    /** Construct a TObject with a given type and optional name.
     *  Used by subclasses
     */
    TObject(const TObjectType& type, const std::string& name = std::string());

public:
    /** Destruct a TObject */
    virtual ~TObject();

    /** Get the name of this TObject */
    virtual const std::string& getName() const;
    /** Get the current result of this TObject */
    const Result& getResult() const;
    
    /** Get the current state of this TObject */
    State getState() const;

    /** Get the type of this TObject */
    const TObjectType& getType() const;
    TObjectType& getType();

    /** Set the name of this TObject */
    void setName(const std::string& name);

    /** Set the state of this TObject.
     *  @param state the new state for this TObject
     *  @return the previous state
     */
    State setState(State state);

    //TODO change to return a TOBool instead of bool
    /** Rewind to initial state, if possible.
     *  @param context Context of run state.  If 0 then goes to default,
     *                 non-contextual run state.
     *  @return false if could not successfully rewind,
     *          otherwise returns true.
     */
    virtual bool rewind(Context* context);

    /**
     *  Processing logic of this TObject, if any.
     */
    virtual const Result process(Context* context = 0);

    /** Run in the given context.
     *
     *  Subclasses must implement specific behavior.  This base method is a
     *  pass-thru.  The returned result is a wrapper for 'this'.
     */
    virtual const Result run(Context* context = 0);

    /** Start this TObject in the given context asynchronously.
     *
     *  Subclasses must implement specific behavior.  This base method spawns
     *  this TObject in a ThreadHandler and returns the result from the thread.
     *  If the context is 0 then starts non-contextually.
     *
     *  Typically a sub-class would return the result of starting the thread,
     *  either a TOBool or perhaps a TOThread, etc.  Then when the thread finishes
     *  the final result is delivered to the callback.
     */
    virtual const Result start(Context* context = 0, Callback* callback = 0);

    /** Stop this TObject if it is running in a thread.
     *
     *  @param force if true then the thread will be forcibly terminated.
     *  @return true if stop was attempted on the thread running this object.
     */
    virtual bool stop(bool force = false);

    /** Test if equal to another TObject */
    virtual bool operator==(const TObject& other) const;

    /** Test if not equal to another TObject */
    virtual bool operator!=(const TObject& other) const;
    
    /** Copy from another TObject. */
    virtual TObject& operator=(const TObject& other);

    // Implement SerializeContract
    virtual bool serialize(Blob& blob);
    virtual bool deserialize(const Blob& blob);

    //TODO implement pluggable contract
    //TODO cast operators for frequent autoconversions:
    //     i.e., if (tobj == false) // false is TOBool(false) is TOFalse which is singleton
    //TODO usage counter

protected:
    TObjectType& type_;
    std::string name_;
    State state_;
    Result result_;

    //TODO a public dictionary of TObject's
    // bool isTemp; // not stored in dictionary
};


typedef TObjectTree Children;

/**
 *  A TObject that contains other TObjects.
 *
 *  It contains a tree of child TObjects and a set of visitors.
 *  Note that the parent SerializeContract is not overrideen since each TObjectContainer subclass
 *  handles their children differently.
 */
class TObjectContainer : public TObject
{
public:

    typedef TObjectIterator iterator;

    /** Add an object to the list of children objects.
     *  @param tObjWrapper the tree wrapper to use.  If not specified then a new
     *                     wrapper is allocated.
     *  @return the child's TObjectTree wrapper.
     */
    virtual TObjectTree* add(TObject* tObject, TObjectTree* tObjWrapper = 0);

    /** Find the object with given name among children */
    TObject* find(const TObjectKey& key);

    /** Remove a child object from children list. */
    bool remove(TObject* tObject);

    Children* getChildren() const;

    // Visitors
    virtual const Result run(Context* context = 0);
    
    virtual const TObjectIterator& visitUntil(Context* context = 0);

    /** Copy from another TObjectContainer. */
    virtual TObjectContainer& operator=(const TObjectContainer& other);

protected:
    /** Construct a TObjectContainer with a given optional name */
    TObjectContainer(const std::string& name = std::string());

    /** Construct a TObjectContainer with a given type and optional name */
    TObjectContainer(const TObjectType& type, const std::string& name = std::string());

    /** Destruct a TObjectContainer */
    virtual ~TObjectContainer();

protected:
    /** List of children objects. */
    Children* children_;

    /** Saved iterator for this container */
    TObjectIterator iterator_;
};

} // namespace base
} // namespace aft
