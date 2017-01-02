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

#include <cassert>
#include <iostream>
#include <sstream>

#include <json/json.h>
#include "structureddataname.h"
using namespace aft::base;


const std::string StructuredDataName::SEPARATOR(".");

////////////////// Helper functions

// Returns path (all but last component)
// precondition: elements is not empty
static std::string
join_(const std::vector<std::string>& elements,
      std::string delimiter = StructuredDataName::SEPARATOR)
{
    std::stringstream ss;
    size_t elems = elements.size() - 1;
    for (size_t i = 0; i < elems; ++i)
    {
        ss << elements[i];
        if (i < elems - 1) ss << delimiter;
    }
    
    return ss.str();
}

static std::vector<std::string>
split_(const std::string& full,
       const std::string& delimiter = StructuredDataName::SEPARATOR)
{
    //TODO check well-formed path.  it should not start or end with SEPARATOR
    std::vector<std::string> ret;
    size_t oldpos = 0;
    size_t pos = full.find(delimiter);
    while (pos != std::string::npos)
    {
        std::string token = full.substr(oldpos, pos - oldpos);
#if 0
        if (token.back() == ']')
        {
        }
#endif
        ret.push_back(token);
        oldpos = pos + 1;
        pos = full.find(delimiter, oldpos);
    }
    ret.push_back(full.substr(oldpos));
    
    return ret;
}

//////////////////

StructuredDataName::StructuredDataName(const std::string& name)
: name_(name) {
    pathFromName();
}

StructuredDataName::StructuredDataName(const char* name)
: name_(name)
{
    pathFromName();
}

StructuredDataName::StructuredDataName(const std::vector<std::string>& namePath)
: path_(namePath)
{
    if (!path_.empty())
    {
        fullPath_ = join_(path_, SEPARATOR);
        name_ = path_.back();
    }
}

StructuredDataName::StructuredDataName(const std::string& path, const std::string& name)
: name_(path + SEPARATOR + name) {
    pathFromName();
}

StructuredDataName::~StructuredDataName()
{
}

void StructuredDataName::pathFromName() {
    size_t pos = name_.rfind(SEPARATOR);
    if (pos != std::string::npos)
    {
        path_ = split_(name_);
        fullPath_ = name_.substr(0, pos);
        //Assert !path_.empty()
        name_ = path_.back();
    } else {
        if (!name_.empty()) {
            path_.push_back(name_);
        }
    }
}

bool StructuredDataName::empty() const
{
    return path_.empty();
}

const std::vector<std::string>&
StructuredDataName::getComponents() const
{
    return path_;
}

std::string
StructuredDataName::getName(bool fullName) const
{
    switch (path_.size())
    {
        case 0:
            return std::string();
            break;
        case 1:
            return name_;
            break;
    }
    
    return fullName ? fullPath_ + SEPARATOR + name_ : name_;
}

const std::string&
StructuredDataName::getPath(bool fullPath) const
{
    if (fullPath) return fullPath_;
    
    if (path_.size() >= 2) return path_[path_.size() - 2];
    
    return path_.back();
}

bool StructuredDataName::operator==(const StructuredDataName& other)
{
    return fullPath_ == other.fullPath_ && name_ == other.name_
                     && path_.size() == other.path_.size();
}
