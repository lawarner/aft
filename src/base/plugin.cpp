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

#include "factory.h"
#include "plugin.h"
#include "osdep/platform.h"
using namespace aft::base;
using namespace aft::osdep;


BasePlugin::BasePlugin(const std::string& bundleName, const std::string& path)
    : bundleName_(bundleName)
    , path_(path)
    , factory_(0)
    , loader_(new PlatformPluginLoader)
{
}

BasePlugin::~BasePlugin()
{
    delete loader_;
}

bool BasePlugin::loadPlugins()
{
    if (!path_.empty())
    {
        loader_->setPath(path_);
    }
    factory_ = loader_->loadBundle(bundleName_);
    return factory_ != 0;
}

void BasePlugin::unloadPlugins()
{
    loader_->unloadBundle();
    factory_ = 0;
}

TObject* BasePlugin::createInstance(const Blob* blob, const Context* context)
{
    TObject* tobject = 0;
    if (factory_)
    {
        tobject = factory_->construct("", blob, context);
    }
    return tobject;
}

const std::string
BasePlugin::describePlugins() const
{
    if (factory_)
    {
        return "BasePlugin Factory: " + factory_->factoryName()
            + ", Category: " + factory_->category();
    }
    return "(not loaded)";
}

BaseFactory*
BasePlugin::getFactory() const
{
    return factory_;
}

PluginLoaderContract* BasePlugin::getPluginLoader() const
{
    return loader_;
}


