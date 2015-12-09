#pragma once
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

#include <string>
#include <vector>


namespace aft
{
namespace base
{

/**
 *  Serialized representation of a class instance.
 *
 *  This class holds the data that is serialize by any class that implements the
 *  SerializeContract interface.  Each Blob holds data, members (other Blobs)
 *  or both data and blobs.
 *  Blobs can optionally be named.
 */
class Blob
{
public:
    Blob(const std::string& name, void* data = 0);
    Blob(const Blob& other);
    virtual ~Blob();

    Blob& operator=(const Blob& other);

    //TODO a general read/write interface?

    bool addData(void* data);
    bool addMember(Blob* blob);
    const void* getData() const;
    const std::vector<Blob*>& getMembers() const;
    const std::string& getName() const;

protected:
    std::string name_;
    void* data_;
    std::vector<Blob*> members_;
};

} // namespace base
} // namespace aft
