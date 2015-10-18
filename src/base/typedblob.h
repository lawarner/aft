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

#include "blob.h"


namespace aft
{
namespace base
{

/**
 *  A "typed" Blob
 *
 *  This class holds the data is serialize by any class that implements the
 *  SerializeContract interface.  Each Blob holds data, members (other Blobs)
 *  or both data and blobs.
 *  Blobs can optionally be named.
 */
class TypedBlob : public Blob
{
public:
    enum Type
    {
        UNKNOWN = -1,
        RAWDATA,
        STRING,
        JSON,
        COMMAND,
        URL
    };

public:
    TypedBlob(const std::string& name, void* data = 0);
    TypedBlob(const std::string& name, Type type, const std::string& stringData);
    virtual ~TypedBlob();

    const std::string& getString() const;
    Type getType() const;

protected:
    Type type_;
    std::string stringData_;
};

} // namespace base
} // namespace aft
