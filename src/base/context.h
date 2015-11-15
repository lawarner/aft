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

#include <map>
#include <string>
#include "visitor.h"


namespace aft
{
namespace base
{
// Forward reference
class PropertyHandler;
class TObject;


/**
 *  Properties held by Context.
 *  The key is a unique property name and the value is an instance of a 
 *  PropertyHandler.
 */
typedef std::map<std::string, PropertyHandler*> Properties;

/**
 *  Context and environment runtime information.
 *
 *  Contexts carry runtime information and are passed loosely between objects.
 *  This base class carries a dictionary of properties that subclasses can use.
 *  These properties
 *  store items such as the current test case, transports, ui, logs, etc.
 */
class Context
{
public:
    virtual ~Context();

public:
    /** Apply this context to test object and return the result. */
    TObject& apply(const TObject& testObject);

protected:
    /** Construct a context with default visitor */
    Context();

    /** Construct a context with a given visitor */
    Context(VisitorContract& visitor);

public:
    bool addProperty(const std::string& propertyName, PropertyHandler* handler);

    VisitorContract& getVisitor() const;

    PropertyHandler* handler(const std::string& propertyName) const;

protected:
    Properties properties_;

    /** Default visitor */
    VisitorContract& visitor_;
};

} // namespace base
} // namespace aft
