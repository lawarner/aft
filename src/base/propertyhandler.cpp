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
#include "context.h"
#include "propertyhandler.h"

using namespace aft::base;
using namespace std;


static const std::string emptyString;

class aft::base::PropertyContainer
{
public:
    map<string, string> nameValues;
};


PropertyHandler::PropertyHandler(const std::string& handlerName)
    : handlerName_(handlerName)
    , container_(new PropertyContainer)
{

}

PropertyHandler::~PropertyHandler()
{
    delete container_;
}

bool
PropertyHandler::getValue(const std::string& name, std::string& value) const
{
    map<string,string>::const_iterator it = container_->nameValues.find(name);
    if (it == container_->nameValues.end())  return false;

    value = it->second;
    return true;
}

const std::string&
PropertyHandler::getValue(const std::string& name,
                          const std::string& defValue) const
{
    map<string,string>::const_iterator it = container_->nameValues.find(name);
    if (it != container_->nameValues.end())  return it->second;

    return defValue.empty() ? emptyString : defValue;
}

void
PropertyHandler::setValue(const std::string& name, const std::string& value)
{
    container_->nameValues[name] = value;
}

TObject&
PropertyHandler::handle(const TObject& tObject)
{
    return const_cast<TObject&>(tObject);
}

TObject&
PropertyHandler::handle(Context* context, const TObject& tObject)
{
    if (context) return context->apply(tObject);

    return const_cast<TObject&>(tObject);
}
