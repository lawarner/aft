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

#include "context.h"
#include "propertyhandler.h"
#include "tobject.h"
using namespace aft::base;

Visitor defaultVisitor;


Context::Context(const std::string& name)
: name_(name)
, env_(*new PropertyHandler("Environment"))
, visitor_(defaultVisitor)
{

}

Context::Context(VisitorContract& visitor, const std::string& name)
: name_(name)
, env_(*new PropertyHandler("Environment"))
, visitor_(visitor)
{

}


Context::~Context()
{
    delete &env_;
}

bool
Context::addProperty(const std::string& propertyName, PropertyHandler* handler)
{
    if (!handler || properties_.find(propertyName) != properties_.end())
    {
        return false;
    }
    properties_[propertyName] = handler;
    return true;
}

TObject&
Context::apply(const TObject& tObject)
{
    return const_cast<TObject&>(tObject);
}

PropertyHandler&
Context::getEnvironment() const
{
    return env_;
}

const std::string&
Context::getName() const
{
    return name_;
}

VisitorContract&
Context::getVisitor() const
{
    return visitor_;
}

PropertyHandler*
Context::handler(const std::string& propertyName) const
{
    Properties::const_iterator it = properties_.find(propertyName);
    if (it == properties_.end()) return 0;

    return it->second;
}
