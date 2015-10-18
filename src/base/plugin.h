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
    virtual bool loadPlugins() = 0;
    virtual void unloadPlugins() = 0;

    virtual TObject* createInstance(const Blob* blob = 0, const Context* context = 0) = 0;

    virtual static const std::string& describe() const = 0;
    virtual bool activate() = 0;
    virtual bool deactive() = 0;
};

} // namespace base
} // namespace aft
