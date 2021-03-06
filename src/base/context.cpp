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
using std::string;

Visitor defaultVisitor;

static Context* globalInstance = nullptr;


Context::Context(const std::string& name)
: name_(name)
, env_(*new BasePropertyHandler("Environment"))
, visitor_(defaultVisitor)
{

}

Context::Context(VisitorContract& visitor, const std::string& name)
: name_(name)
, env_(*new BasePropertyHandler("Environment"))
, visitor_(visitor) {

}

Context::~Context() {
    delete &env_;
}

Context* Context::global() {
    if (!globalInstance) {
        globalInstance = new Context("(global)");
    }
    return globalInstance;
}

bool
Context::addProperty(const std::string& propertyName, PropertyHandler* handler) {
    if (!handler || properties_.find(propertyName) != properties_.end()) {
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

BasePropertyHandler&
Context::getEnvironment() const
{
    return env_;
}

bool Context::getEnv(const std::string& name, std::string& value) const
{
    return env_.getValue(name, value);
}

void Context::setEnv(const std::string& name, const std::string& value)
{
    env_.setValue(name, value);
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
    if (it == properties_.end()) return nullptr;

    return it->second;
}

/**
 *  Get a property handler with the given type.
 *  This currently just maps the give propertyType into a string that
 *  is used as the name of the property handler.
 */
PropertyHandler*
Context::handler(HandlerType propertyType) const {
    const std::string& name = PropertyHandler::handlerTypeName(propertyType);
    return handler(name);
}

Result
Context::process(const TObject* tObject)
{
    // visit? tObject->getResult()?
    return Result(true);
}
