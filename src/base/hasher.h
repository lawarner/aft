#pragma once
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

#include <map>
#include <vector>

namespace aft
{
namespace base
{

/**
 *  A string to hash converter class so factories can use a switch statement.
 *
 *  Basically matches up:
 *     "cmd name" CMD_NAME 0xhashcode
 *  where CMD_NAME is an enum value.
 
 string -> gethash -> returns int into array of hashes.
 enum Cmd { enumVal1, ...  }
 map<string, enum Cmd>
 int hashes[]  // of size "enum"
 So use as:
 std::string cmd;
 switch (getHashIndex(cmd)) {
   case enumVal1:
 
 */
class Hasher
{
public:
    /** Construct a Hasher.
     *
     *  The corresponding enum values must start with value 0 and follow the
     *  same order as names.
     *  TODO allow for different starting number
     */
    Hasher(const std::vector<std::string>& names);
    /** Construct a Hasher. */
    Hasher(const char* names[], int nameCount = 0);
    ~Hasher();

    typedef unsigned long HashType;
    
    HashType getHash(const std::string& name);
    int getHashIndex(const std::string& name);
    const std::string& getName(int enumCode);
    const std::string& getNameForHash(HashType hash);
    
private:
    bool putHashValue(const std::string& name, int idx);

private:
    std::map<std::string, int> enums_;
    HashType* hashes_;
};
    
}
}
