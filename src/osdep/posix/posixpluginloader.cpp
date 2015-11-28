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

#include <iostream>
#include <dlfcn.h>
#include "base/factory.h"
#include "osdep/platform.h"

using namespace aft::base;
using namespace aft::osdep;

static const char* DEFAULT_PATH = "/usr/local/lib/aft/plugins";
static const char* PATH_SEPARATOR = "/";
static const char* SO_EXT = ".so";


class aft::osdep::PosixPluginImpl
{
public:
    PosixPluginImpl(const std::string& path = DEFAULT_PATH)
        : handle_(0)
        , factory_(0)
        , path_(path)
        {  }
    void* handle_;
    aft::base::BaseFactory* factory_;
    //TODO keep array of directories in path instead of just one
    std::string path_;
    std::string bundleName_;
};


static std::string
makeSoName(const PosixPluginImpl& impl)
{
    std::string soName = impl.path_ + PATH_SEPARATOR + impl.bundleName_ + SO_EXT;
    return soName;
}


PosixPluginLoader::PosixPluginLoader()
    : impl_(*new PosixPluginImpl)
{
}

PosixPluginLoader::~PosixPluginLoader()
{
    delete &impl_;
}

BaseFactory*
PosixPluginLoader::loadBundle(const std::string& bundleName)
{
    impl_.bundleName_ = bundleName;
    std::string soName = makeSoName(impl_);

    std::cout << "Going to load " << soName << std::endl;
    void* handle = dlopen(soName.c_str(), RTLD_LAZY);
    if (!handle)
    {
        std::cout << "Error loading ld: " << dlerror() << std::endl;
        return 0;
    }

    impl_.handle_ = handle;

    InitializeFunction initialize = (InitializeFunction)dlsym(handle, "initialize");
    if (!initialize)
    {
        std::cout << "Unable to load initialize() function" << std::endl;
        return 0;
    }

    DeinitializeFunction deinitialize = (DeinitializeFunction)dlsym(handle, "deinitialize");
    BaseFactory* factory = initialize();
    if (factory)
    {
        impl_.factory_ = factory;
        factory->setDeinit((void *)deinitialize);
    }
    else
    {
        std::cout << "Error instatiating plug-in factory" << std::endl;
    }

    return factory;
}

void PosixPluginLoader::setPath(const std::string& path)
{
    //TODO expand env variables and tildes
    impl_.path_ = path;
}

void PosixPluginLoader::unloadBundle()
{
    if (impl_.factory_)
    {
        impl_.factory_->deinit();
        delete impl_.factory_;
        impl_.factory_ = 0;
    }

    if (impl_.handle_)
    {
        dlclose(impl_.handle_);
        impl_.handle_ = 0;
    }
}
