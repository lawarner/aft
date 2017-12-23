#pragma once
/*
 *   Copyright © 2015-2017 Andy Warner
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

#include <vector>

namespace aft {
namespace base {
// Forward reference
class Context;
class PropertyContainer;
class TObject;


/**
 * Handle TObject's based on a given property or functionality.
 * A TObject can be handled either contextually or non-contextually.
 * A set of name/value pairs is also available for handlers to use.
 *
 * An example usage could be a serializer handler that reads property
 * "filename", opens the file and writes the contents of the give TObject.
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

    /** Return a list of all of the property names. */
    void getPropertyNames(std::vector<std::string>& names) const;

    /** Get the value of a property.
     *  @param name Name of value to retrieve.
     *  @param value Reference to a string where the value will be copied to.
     *  @return true if value was retrieved, otherwise false.
     */
    bool getValue(const std::string& name, std::string& value) const;

    /** Get the value of a property.
     *  @return value associated with name, or empty string if no value is
     *          associated.
     */
    const std::string& getValue(const std::string& name) const;

    /** Associate (bind) a named value */
    void setValue(const std::string& name, const std::string& value);
    
    /** Unassoicate a named value */
    bool unsetValue(const std::string& name);

    /** Apply the property to a TObject and return the result.
     *  Sub-classes are expected to implement specific property-specific behavior.
     *  The base class method just returns the given TObject (pass-thru).
     */
    virtual TObject& handle(const TObject& tObject);
    /** Apply the property to a TObject using context and return the result. */
    virtual TObject& handle(Context* context, const TObject& tObject);

private:
    std::string handlerName_;
    std::unique_ptr<PropertyContainer> container_;
};

} // namespace base
} // namespace aft
