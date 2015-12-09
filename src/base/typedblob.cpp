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

#include "typedblob.h"
using namespace aft::base;


TypedBlob::TypedBlob(const std::string& name, void* data)
    : Blob(name, data)
    , type_(RAWDATA)
{
}

TypedBlob::TypedBlob(const std::string& name, TypedBlob::Type type,
                     const std::string& stringData)
    : Blob(name)
    , type_(type)
    , stringData_(stringData)
{
    //TODO assert type not UNKNOWN or RAWDATA
}

TypedBlob::TypedBlob(const TypedBlob& other)
    : Blob(other)
    , type_(other.type_)
    , stringData_(other.stringData_)
{

}

TypedBlob::~TypedBlob()
{
}

TypedBlob& TypedBlob::operator=(const TypedBlob& other)
{
    if (&other != this)
    {
        Blob::operator=(other);
        type_ = other.type_;
        stringData_ = other.stringData_;
    }

    return *this;
}

const std::string&
TypedBlob::getString() const
{
    return stringData_;
}

TypedBlob::Type
TypedBlob::getType() const
{
    return type_;
}


