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

#include "propertymap.h"
#include <unordered_map>
#include <string>


namespace aft {
namespace base {
// Forward reference
class Context;
class TObject;

/** Some common PropertyHandler types.
 *  Note that subclasses could be defined that does not fit these common types.
 *  In that case, the subclass can use the Extended type.
 *  Also note that there can be multiple PropertyHandlers with the same type.
 */
enum class HandlerType {
    Unknown,
    Base,
    Command,
    Logging,
    String = Base,
    TObject,
    UI,
    Run,
    Extended
};

/**
 * Handle TObject's based on a given property or functionality.
 * A TObject can be handled either contextually or non-contextually.
 * A set of name/value pairs is also available for handlers to use.
 *
 * An example usage could be a serializer handler that reads property
 * "filename", opens the file and writes the contents of the give TObject.
 */
class PropertyHandler {
public:
    /** Construct a handler with a given name */
    PropertyHandler(const std::string& handlerName, HandlerType type = HandlerType::Unknown);
    /** Destruct a handler */
    virtual ~PropertyHandler() = default;

    /** Return the name of the handler */
    const std::string& getName() const { return handlerName_; }
    /** Return the type of the handler */
    HandlerType getType() const { return type_; }

    /** Apply the property to a TObject and return the result.
     *  Sub-classes are expected to implement specific property-specific behavior.
     *  The base class method just returns the given TObject (pass-thru).
     */
    virtual TObject& handle(const TObject& tObject);
    
    /** Apply the property to a TObject using context and return the result. */
    virtual TObject& handle(Context* context, const TObject& tObject);

    static const std::string& handlerTypeName(HandlerType type);
    
private:
    const std::string handlerName_;
    const HandlerType type_;
};
    
/** Base implementation for string property values.
 *  Probably a more descriptive name for this class is StringPropertyHandler.
 */
class BasePropertyHandler : public PropertyHandler, public PropertyMap<std::string> {
public:
    BasePropertyHandler(const std::string& handlerName);
    virtual ~BasePropertyHandler() = default;
private:
    
};

} // namespace base
} // namespace aft
