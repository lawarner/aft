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

#include <sstream>

#include "blob.h"
#include "command.h"
#include "result.h"
using namespace aft::base;


Result::Result(ResultType type)
: type_(type)
, isValueSet_(type == FATAL)
{

}

Result::Result(Blob* blob)
: type_(BLOB)
{
    setValue(blob);
}

Result::Result(bool value)
: type_(BOOLEAN)
{
    setValue(value);
}

Result::Result(Command* command)
: type_(COMMAND)
{
    setValue(command);
}

Result::Result(int value)
: type_(INTEGER)
{
    setValue(value);
}

Result::Result(const std::string& strValue)
: type_(STRING)
{
    setValue(strValue);
}

Result::Result(TObject* object)
: type_(TOBJECT)
{
    setValue(object);
}

Result::~Result()
{

}

std::string
Result::asString() const
{
    if (!isValueSet_) return "(unset)";

    switch (type_)
    {
        case UNKNOWN:
            break;
        case FATAL:
            return "(FATAL)";
        case BLOB:
            return value_.blob_->getString();
        case BOOLEAN:
            return (value_.flag_ ? "true" : "false");
        case COMMAND:
            return value_.command_->getName();
        case INTEGER:
        {
            std::ostringstream oss;
            oss << value_.integer_;
            return oss.str();
        }
        case ITERATOR:
            return "(Iterator)";
        case STRING:
            return *value_.string_;
        case TOBJECT:
            return value_.object_->getName();
        default:
            break;
    }
    return "";  // fall trhu
}


Result::ResultType
Result::getType() const
{
    return type_;
}

std::string Result::getTypeName() const
{
    switch (type_)
    {
        case UNKNOWN:
            return "(unknown)";
        case FATAL:
            return "FATAL";
        case BLOB:
            return "Blob";
        case BOOLEAN:
            return "Boolean";
        case COMMAND:
            return "Command";
        case INTEGER:
            return "Integer";
        case ITERATOR:
            return "Iterator";
        case STRING:
            return "String";
        case TOBJECT:
            return "TObject";
        default:
            return "(bad result type)";
    }
    return "";  // not reached
}

bool Result::getValue(Blob*& blob) const
{
    if (!isValueSet_ || type_ != BLOB) return false;

    blob = value_.blob_;
    return true;
}

bool Result::getValue(bool& value) const
{
    if (!isValueSet_ || type_ != BOOLEAN) return false;
    
    value = value_.flag_;
    return true;
}

bool Result::getValue(Command*& command) const
{
    if (!isValueSet_ || type_ != COMMAND) return false;

    command = value_.command_;
    return true;
}

bool Result::getValue(int& value) const
{
    if (!isValueSet_ || type_ != INTEGER) return false;

    value = value_.integer_;
    return true;
}

bool Result::getValue(std::string& strValue) const
{
    if (!isValueSet_ || type_ != STRING) return false;

    strValue = *value_.string_;
    return true;
}

bool Result::getValue(TObject*& object) const
{
    if (!isValueSet_ || type_ != TOBJECT) return false;

    object = value_.object_;
    return true;
}

void Result::setValue(Blob* blob)
{
    if (blob)
    {
        type_ = BLOB;
        value_.blob_ = blob;
        isValueSet_ = true;
    }
}

void Result::setValue(bool value)
{
    type_ = BOOLEAN;
    value_.flag_ = value;
    isValueSet_ = true;
}

void Result::setValue(Command* command)
{
    type_ = COMMAND;
    value_.command_ = command;
    isValueSet_ = true;
}

void Result::setValue(int value)
{
    type_ = INTEGER;
    value_.integer_ = value;
    isValueSet_ = true;
}

void Result::setValue(const std::string& strValue)
{
    type_ = STRING;
    value_.string_ = new std::string(strValue);
    isValueSet_ = true;
}

void Result::setValue(TObject* object)
{
    type_ = TOBJECT;
    value_.object_ = object;
    isValueSet_ = true;
}

int Result::compare(const Result& other) const
{
    if (type_ != other.type_) return -2;
    if (!isValueSet_ || !other.isValueSet_)
    {
        return -2;
    }
    if (operator==(other)) return 0;

    switch (type_)
    {
        case UNKNOWN:
            return -2;
        case FATAL:
            return -2;
        case BLOB:
            return -2;
        case BOOLEAN:
            if (value_.flag_ < other.value_.flag_) return -1;
        case COMMAND:
            return -2;
        case INTEGER:
            if (value_.integer_ < other.value_.integer_) return -1;
        case ITERATOR:
            return -2;
        case STRING:
            if (value_.string_ < other.value_.string_) return -1;
        case TOBJECT:
            return -2;
        default:
            return -2;
    }
    
    return 1;
}

bool Result::operator==(const Result& other) const
{
    if (this == &other) return true;
    if (type_ != other.type_) return false;
    if (!isValueSet_ || !other.isValueSet_)
    {
        return isValueSet_ == other.isValueSet_;
    }
    
    switch (type_)
    {
        case UNKNOWN:
            return true;
        case FATAL:
            return true;
        case BLOB:
            return *value_.blob_ == *other.value_.blob_;
        case BOOLEAN:
            return value_.flag_ == other.value_.flag_;
        case COMMAND:
            return value_.command_ == other.value_.command_;
        case INTEGER:
            return value_.integer_ == other.value_.integer_;
        case ITERATOR:
            return value_.iterator_ == other.value_.iterator_;
        case STRING:
            return value_.string_ == other.value_.string_;
        case TOBJECT:
            return value_.object_ == other.value_.object_;
        default:
            break;
    }

    return false;
}

bool Result::operator!=(const Result& other) const
{
    return !operator==(other);
}

bool Result::operator!() const
{
    return !operator bool();
}

Result::operator bool() const
{
    if (!isValueSet_ || type_ == FATAL || (type_ == BOOLEAN && !value_.flag_))
    {
        return false;
    }

    return true;
}
