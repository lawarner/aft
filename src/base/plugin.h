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

namespace aft
{
namespace base
{
// Forward reference
class BaseFactory;
class Blob;
class Context;
class TObject;


/**
 *  Any class supporting plugins must implement the PluginContract.
 *
 *  The primary extension mechanism in AFT is the plugin.  These are contained in
 *  dynamically loadable bundles that adhere to the FactoryContract interface.
 *  
 */
class PluginContract
{
public:
    /** Return the factory, if loaded */
    virtual BaseFactory* getFactory() const = 0;

    /** Check if the plug-ins are loaded.. */
    virtual bool isLoaded() const = 0;

    /** Load the plug-ins. */
    virtual bool loadPlugins() = 0;
    /** Unload the plug-ins. */
    virtual void unloadPlugins() = 0;

    virtual TObject* createInstance(const Blob* blob = 0, const Context* context = 0) = 0;

    virtual const std::string describePlugins() const = 0;
};


/**
 *  Plug-in loader interface.
 *
 *  Note that loadable plugins have the following structure:
 *
 *  BaseFactory* initialize()
 *  void deinitialize()
 *  The rest of the .so file contains various TObject implementations that
 *  are instantiated by the returned factory.
 */
class PluginLoaderContract
{
public:
    /** Destruct a plugin loader */
    virtual ~PluginLoaderContract() { }

    virtual BaseFactory* loadBundle(const std::string& bundleName) = 0;
    virtual void setPath(const std::string& path) = 0;
    virtual void unloadBundle() = 0;
};


/**
 *  Base implementation of PluginContract.
 *
 *  When implementing a Plugin capable class it is often useful to sub-class from
 *  BasePlugin instead of implementing the PluginContract interface.
 */
class BasePlugin : public PluginContract
{
public:
    BasePlugin(const std::string& bundleName, const std::string& path = std::string());

    virtual ~BasePlugin();

    virtual bool isLoaded() const;
    virtual bool loadPlugins();
    virtual void unloadPlugins();

    virtual TObject* createInstance(const Blob* blob = 0, const Context* context = 0);

    virtual const std::string describePlugins() const;

    virtual BaseFactory* getFactory() const;
    virtual PluginLoaderContract* getPluginLoader() const;

protected:
    const std::string bundleName_;
    const std::string path_;
    BaseFactory* factory_;
    PluginLoaderContract* loader_;
};

} // namespace base
} // namespace aft
