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
#include <map>
#include <vector>


namespace aft
{
namespace base
{
// Forward reference
class Blob;
class Context;
class TObject;


/**
 *  Contract that factory classes must implement.
 */
class FactoryContract
{
public:
    /** The category of this factory. */
    virtual const std::string& category() const = 0;
    /** The name of this factory. */
    virtual const std::string& factoryName() const = 0;

    /**
     *  Construct an object with a given name.
     *
     *  @param name Name of object to create
     *  @param blob Parameters that are used to create the object.
     *           If not specified a generic object is created, if possible.
     *  @param context The context to apply when creating the object.  Note that 
     *           some TObject's are not context aware, so specifying a context
     *           may not have an effect.
     */
    virtual TObject* construct(const std::string& name, const Blob* blob = 0,
                               const Context* context = 0) = 0;

    /** Deinitialize the factory.
     *  This is primarily used when unloading a dynamically loaded (bundle) factory.
     */
    virtual bool deinit() = 0;
    /** Set the Deinit pointer called from deinit() */
    virtual void setDeinit(void* deinit) = 0;
};


/**
 *  Base implementation of FactoryContract.
 *
 *  When implementing a Factory it is often useful to sub-class from BaseFactory
 *  instead of implementing the FactoryContract interface.
 */
class BaseFactory : public FactoryContract
{
public:
    /** Create a BaseFactory with given category and name. */
    BaseFactory(const std::string& category, const std::string& name);
    /** Destruct a BaseFactory. */
    virtual ~BaseFactory();

    /** The category of this factory. */
    virtual const std::string& category() const;
    /** The name of this factory. */
    virtual const std::string& factoryName() const;

    virtual TObject* construct(const std::string& name, const Blob* blob = 0,
                               const Context* context = 0);

    virtual bool deinit();
    virtual void setDeinit(void* deinit);

protected:
    std::string category_;
    std::string factoryName_;
    void* deinitPointer_;
};

/**
 *  The MEC factory is a parent of other factories.
 *
 *  The factories are arranged in a list by category so specific sub-types of TObject
 *  can be constructed.  Factories are either added manually or loaded dynmaically
 *  from a bundle.
 *
 *  When a TObject is constructed, each factory in the given category is called and
 *  given a turn to construct the object.  The object is constructed when the first
 *  factory returns a value.  The factories per category are kept in the order that
 *  they were added to the MecFactory.
 */
class MecFactory
{
public:
    /** Type of per-category factory list. */
    typedef std::vector<FactoryContract*> FactoryList;

public:
    /** Construct a MecFactory.  This may change to a singleton. */
    MecFactory();
    /** Destruct a MecFactory. */
    virtual ~MecFactory();

    /** Load a factory from a bundle.
     *
     *  The factory will be added to its category.
     *  @param bundleName Past to the delegate in order to load the bundle.
     */
    FactoryContract* loadBundle(const std::string& bundleName);
    /** Unload a factory bundle. */
    bool unloadBundle(FactoryContract* factoryClass);

    /** Add a factory to its category */
    void addFactory(const FactoryContract* factoryClass);
    /** Remove a factory from its category */
    void removeFactory(const FactoryContract* factoryClass);

    /**
     *  Construct an object with a given name in the given category.
     *
     *  The factories that have been added to the category actually construct
     *  the object.  This method simply calls the category factories.
     *
     *  @param category Category
     *  @param name Name of object to create
     *  @param parameters Parameters that a factory uses to create the object.
     *           If not specified a generic object is created, if possible.
     *  @param context The context to apply when creating the object.  Note that 
     *           some TObject's are not context aware, so specifying a context
     *           may not have an effect.
     */
    TObject* construct(const std::string& category, const std::string& name,
                       const Blob* parameters = 0, const Context* context = 0);

protected:
    std::map<std::string, FactoryList > factories_;
    //TODO loader delegate
};

} // namespace base
} // namespace aft
