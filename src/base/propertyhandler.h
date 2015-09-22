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
class Context;
class PropertyContainer;
class TObject;


/**
 * Handle TObject's based on a given property or functionality.
 * A TObject can be handled either contextually or non-contextually.
 * A set of name/value pairs is also available for handlers to use.
 */
class PropertyHandler
{
public:
    /** Construct a handler with a given name */
    PropertyHandler(const std::string& handlerName);
    /** Destruct a handler */
    virtual ~PropertyHandler();

    /** Return the name of the handler */
    const std::string& getName() const { return handlerName_; }
    /** Get the value of a property.
     *  @return value associated with name, or empty string if no value is
     *          associated.
     */
    const std::string& getValue(const std::string& name) const;
    /** Associate (bind) a named value */
    void setValue(const std::string& name, const std::string& value);

    virtual TObject& handle(const TObject& testObject) = 0;
    virtual TObject& handle(Context* context, const TObject& testObject);

private:
    std::string handlerName_;
    PropertyContainer* container_;
};

} // namespace base
} // namespace aft
