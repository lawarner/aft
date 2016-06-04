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

#include <iomanip>
#include <sstream>

#include "blob.h"
using namespace aft::base;


Blob::Blob(const std::string& name, void* data, int dataLength)
: name_(name)
, type_(RAWDATA)
{
    addData(data, dataLength);
}

Blob::Blob(const std::string& name, Blob::Type type, const std::string& stringData)
: name_(name)
, data_(0)
, dataLength_(0)
, type_(type)
, stringData_(stringData)
{

}

Blob::Blob(const Blob& other)
: name_(other.name_)
, data_(other.data_)
, dataLength_(other.dataLength_)
, members_(other.members_)
, type_(other.type_)
, stringData_(other.stringData_)
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
        dataLength_ = other.dataLength_;
        members_ = other.members_;
        type_ = other.type_;
        stringData_ = other.stringData_;
    }

    return *this;
}

bool Blob::addData(void* data, int dataLength)
{
    data_ = data;
    dataLength_ = dataLength;

    if (data_)
    {
        std::ostringstream oss;
        if (dataLength_ < 0)
        {
            oss << "<pointer>";
        }
        else
        {
            unsigned char* charData = (unsigned char *)data_;
            for (int idx = 0; idx < dataLength_; ++idx)
            {
                unsigned int ch = charData[idx];
                oss << std::hex << std::setw(2) << std::setfill('0') << ch << " ";
                if (idx == (dataLength_ - 1) || (idx % 16) == (dataLength_ - 1))
                {
                    oss << std::endl;
                }
            }
        }
        stringData_ = oss.str();
    }
    else
    {
        stringData_ = "<null>";
    }

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

const std::string&
Blob::getString() const
{
    return stringData_;
}

Blob::Type
Blob::getType() const
{
    return type_;
}

bool Blob::operator==(const Blob& other)
{
    if (this == &other) return true;
    if (type_ != other.type_ ||
        name_ != other.name_)
    {
        return false;
    }
    
    if (members_.size() != other.members_.size()) return false; //TODO more strict testing of members

    switch (type_)
    {
        case UNKNOWN:
            return true;
        case RAWDATA:
            return data_ == other.data_;
        case STRING:
            return stringData_ == other.stringData_;
        case JSON:
            return stringData_ == other.stringData_;
        case COMMAND:
            return false;   //TODO figure out if this is even used
        case URL:
            return stringData_ == other.stringData_;
        default:
            break;
    }
    return false;
}

bool Blob::operator!=(const Blob& other)
{
    return !operator==(other);
}
