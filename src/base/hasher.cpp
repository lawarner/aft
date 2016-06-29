/*
 *   Copyright 2016 Andy Warner
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
//
//  hasher.cpp
//  libaft
//
#include <functional>
#include <string>

#include "hasher.h"

static const std::string emptyString;

using namespace aft::base;


Hasher::Hasher(const std::vector<std::string>& names)
: hashes_(new HashType[names.size()])
{
    for (unsigned int idx = 0; idx < names.size(); ++idx)
    {
        putHashValue(names[idx], idx);
    }
}

Hasher::Hasher(const char* names[], int nameCount)
: hashes_(nameCount > 0 ? new HashType[nameCount] : 0)
{
    if (nameCount == 0)
    {
        while (names[nameCount]) nameCount++;  // do nothing but count
        hashes_ = new HashType[nameCount];
    }
    
    for (int idx = 0; idx < nameCount; ++idx)
    {
        putHashValue(names[idx], idx);
    }
}

Hasher::~Hasher()
{
    delete [] hashes_;
}

bool
Hasher::putHashValue(const std::string& name, int idx)
{
    std::hash<std::string> strHash;
    hashes_[idx] = strHash(name);
    //TODO check if hash zero or already in map
    enums_[name] = idx;

    return true;
}

Hasher::HashType
Hasher::getHash(const std::string& name)
{
    int hashIndex = getHashIndex(name);
    if (hashIndex < 0)
    {
        return 0;
    }
    
    return hashes_[hashIndex];
}

int Hasher::getHashIndex(const std::string& name)
{
    std::map<std::string, int>::const_iterator it = enums_.find(name);
    if (it == enums_.end())  return -1;
    
    return it->second;
}

const std::string&
Hasher::getName(int enumCode) const
{
    std::map<std::string, int>::const_iterator it;
    for (it = enums_.cbegin(); it != enums_.cend(); ++it)
    {
        if (it->second == enumCode)
        {
            return it->first;
        }
    }
    
    return emptyString;
}

const std::string&
Hasher::getNameForHash(HashType hash) const
{
    for (unsigned int idx = 0; idx < enums_.size(); ++idx)
    {
        if (hash == hashes_[idx])
        {
            return getName(idx);
        }
    }

    return emptyString;
}

