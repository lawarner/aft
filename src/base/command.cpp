/*
 *   Copyright 2015, 2016 Andy Warner
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

#include "blob.h"
#include "command.h"
#include "structureddata.h"
#include "tobjecttype.h"

using namespace aft::base;


Command::Command(const std::string& name)
: TObjectContainer(TObjectType::TypeCommand, name)
{

}

Command::Command(const std::string& name, const ParameterList& parameters)
: TObjectContainer(TObjectType::TypeCommand, name)
, parameters_(parameters)
{

}

Command::~Command()
{
}

bool Command::serialize(Blob& blob)
{
    base::StructuredData sd("Command");

    sd.add("name", getName());
    sd.addArray("parameters");
    std::vector<std::string>::const_iterator it;
    for (it = parameters_.begin(); it != parameters_.end(); ++it)
    {
        sd.add("parameters.", *it);
    }

    return sd.serialize(blob);
}

bool Command::deserialize(const Blob& blob)
{
    base::StructuredData sd("Command", blob.getString());
    std::string name;
    std::vector<std::string> parameters;
    if (!sd.get("name", name) || !sd.getArray("parameters", parameters_))
    {
        return false;
    }

    name_ = name;
    return true;
}

Command& Command::operator=(const Command& other)
{
    if (this != &other)
    {
        TObjectContainer::operator=(other);
        parameters_ = other.parameters_;
    }

    return *this;
}
