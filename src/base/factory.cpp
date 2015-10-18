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

#include <algorithm>
#include "factory.h"
#include "tobasictypes.h"
using namespace aft::base;


BaseFactory::BaseFactory(const std::string& category, const std::string& name)
    : category_(category)
    , factoryName_(name)
    , deinitPointer_(0)
{
}

BaseFactory::~BaseFactory()
{
    deinit();
}

const std::string&
BaseFactory::category() const
{
    return category_;
}

const std::string&
BaseFactory::factoryName() const
{
    return factoryName_;
}

TObject* BaseFactory::construct(const std::string& name, const Blob* blob,
                               const Context* context)
{
    TObject* tObject = 0;
    if (blob)
    {
        tObject = new TOBlob(blob, name);
    } else {
        tObject = new TObject(name);
    }

    return tObject;
}

bool BaseFactory::deinit()
{
    if (deinitPointer_)
    {
        typedef bool (*DeInit)();
        return (*(DeInit)deinitPointer_)();
    }
    return false;
}

void BaseFactory::setDeinit(void* deinit)
{
    deinitPointer_ = deinit;
}


MecFactory::MecFactory()
{
}

MecFactory::~MecFactory()
{
}

FactoryContract*
MecFactory::loadBundle(const std::string& bundleName)
{
    //TODO use delegate
    return 0;
}

bool MecFactory::unloadBundle(FactoryContract* factoryClass)
{
    return false;
}

void MecFactory::addFactory(const FactoryContract* factoryClass)
{
    if (!factoryClass) return;

    FactoryList& factoryList = factories_[factoryClass->category()];
    factoryList.push_back(const_cast<FactoryContract*>(factoryClass));
}

void MecFactory::removeFactory(const FactoryContract* factoryClass)
{
    FactoryList& factoryList = factories_[factoryClass->category()];
    FactoryList::iterator it;
    it = std::find(factoryList.begin(), factoryList.end(), factoryClass);
    if (it != factoryList.end())
    {
        factoryList.erase(it);
    }
}

TObject* MecFactory::construct(const std::string& category, const std::string& name,
                               const Blob* parameters, const Context* context)
{
    TObject* result = 0;
    FactoryList& factoryList = factories_[category];
    FactoryList::iterator it;
    for (it = factoryList.begin(); it != factoryList.end(); ++it)
    {
        result = (*it)->construct(name, parameters, context);
        if (result) break;
    }

    return result;
}

