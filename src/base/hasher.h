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
 *  A string to hash converter class so factories can use a switch statement on an enum.
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
     *  TODO allow for different starting number and sparse enum values
     *  @param vector list of strings to associate with enum values 0, 1, ...
     */
    Hasher(const std::vector<std::string>& names);
    /** Construct a Hasher.
     *
     *  The corresponding enum values must start with value 0 and follow the
     *  same order as names.
     *  @param names array of strings to associate with enums. If nameCount is 0 then the 
     *               last entry of this array must be 0.
     *  @param nameCount The number of elements in the names array. If 0 then names must have
     *               an additional 0 (null) entry.
     */
    Hasher(const char* names[], int nameCount = 0);
    /** Destruct a Hasher. */
    ~Hasher();

    typedef unsigned long HashType;
    
    HashType getHash(const std::string& name);
    /** Get the sequence number of a name within the names of this Hasher.
     *
     *  @param name Name to lookup with the array of names
     *  @return The index of name into the array of names. Returns -1 if name is not found in names.
     */
    int getHashIndex(const std::string& name);
    /** Used for debugging, make be removed later. */
    const std::string& getName(int enumCode) const;
    /** Used for debugging, make be removed later. */
    const std::string& getNameForHash(HashType hash) const;
    
private:
    bool putHashValue(const std::string& name, int idx);

private:
    std::map<std::string, int> enums_;
    HashType* hashes_;
};
    
}
}
