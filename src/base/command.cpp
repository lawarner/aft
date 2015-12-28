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

#include "blob.h"
#include "command.h"
#include "structureddata.h"

using namespace aft::base;

Command::Command(const std::string& name)
{
    name_ = name;
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
    if (sd.get("name", name))
    {
        name_ = name;
    } else {
        return false;
    }

    std::vector<std::string> parameters;
    if (sd.getArray("parameters", parameters))
    {
        std::vector<std::string>::const_iterator it;
        for (it = parameters.begin(); it != parameters.end(); ++it)
        {
            parameters_.push_back(*it);
        }
    } else {
        return false;
    }

    return true;
}

