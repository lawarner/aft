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
using namespace aft::base;

Blob::Blob(const std::string& name, void* data)
    : name_(name)
    , data_(data)
{

}

Blob::Blob(const Blob& other)
    : name_(other.name_)
    , data_(other.data_)
    , members_(other.members_)
{

}

Blob::~Blob()
{

}

Blob& Blob::operator=(const Blob& other)
{
    if (&other != this)
    {
        name_ = other.name_;
        data_ = other.data_;
        members_ = other.members_;
    }

    return *this;
}

bool Blob::addData(void* data)
{
    data_ = data;
    return true;
}

bool Blob::addMember(Blob* blob)
{
    members_.push_back(blob);
    return true;
}

const void*
Blob::getData() const
{
    return data_;
}

const std::vector<Blob*>&
Blob::getMembers() const
{
    return members_;
}

const std::string&
Blob::getName() const
{
    return name_;
}

