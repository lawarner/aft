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

#include "context.h"
#include "propertyhandler.h"

using namespace aft::base;
using std::string;

static const string strPropertyNames[] = {
    "Unknown",
    "Base",
    "Command",
    "Logging",
    "String",
    "TObject",
    "UI",
    "Run",
    "Extended"
};


PropertyHandler::PropertyHandler(const std::string& handlerName, HandlerType type)
    : handlerName_(handlerName)
    , type_(type) {

}

TObject& PropertyHandler::handle(const TObject& tObject) {
    return const_cast<TObject&>(tObject);
}

TObject& PropertyHandler::handle(Context* context, const TObject& tObject) {
    if (nullptr != context) {
        return context->apply(tObject);
    }
    return const_cast<TObject&>(tObject);
}

const std::string&
PropertyHandler::handlerTypeName(HandlerType type) {
    size_t idx = 0;
    if (HandlerType::Unknown <= type && HandlerType::Extended >= type) {
        idx = static_cast<size_t>(type);
    }
    return strPropertyNames[idx];
}


BasePropertyHandler::BasePropertyHandler(const std::string& handlerName)
    : PropertyHandler(handlerName) {

}
