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

#include "base/plugin.h"

extern "C" {
// Function pointer prototypes for the .so plugin file
typedef aft::base::BaseFactory* (*InitializeFunction)();
typedef void (*DeinitializeFunction)();
}

namespace aft
{
namespace base
{
// Forward reference
class BaseFactory;
}

namespace osdep
{
// Forward reference
class NativePluginImpl;

/**
 *  Dynamic library plug-in loader.
 *
 *  Note that loadable plugins have the following structure:
 *
 *  BaseFactory* initialize()
 *  void deinitialize()
 *  The rest of the .so file contains various TObject implementations that
 *  are instantiated by the returned factory.
 */
class NativePluginLoader : public aft::base::PluginLoaderContract
{
public:
    NativePluginLoader();
    virtual ~NativePluginLoader();

    aft::base::BaseFactory* loadBundle(const std::string& bundleName);
    void setPath(const std::string& path);
    void unloadBundle();

private:
    NativePluginImpl& impl_;
};

} // namespace osdep
} // namespace aft
