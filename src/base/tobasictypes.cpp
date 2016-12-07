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

#include <sstream>

#include "blob.h"
#include "operation.h"
#include "tobasictypes.h"
using namespace aft::base;


TOBasicTypeBase::TOBasicTypeBase(const TObjectType& type, const std::string& name)
: TObject(type, name)
{
    
}
TOBasicTypeBase::~TOBasicTypeBase()
{
    
}

Result TOBasicTypeBase::getValueAsResult() const
{
    return resultValue_;
}

// Specializations

TOBlob::TOBlob(Blob *value, const std::string& name)
: TOBasicType<Blob *>(value, name)
{
    resultValue_.setValue(value);
}

TOBlob::operator bool() const
{
    return value_;
}

bool TOBlob::serialize(Blob& blob)
{
    if (!TObject::serialize(blob))
    {
        return false;
    }

    StructuredData sd(TObjectType::NameBasicType, blob);
    sd.add("blobtype", value_->getType());
    sd.add("value", value_->getString());

    return sd.serialize(blob);
}

bool TOBlob::deserialize(const Blob& blob)
{
    if (!TObject::deserialize(blob))
    {
        return false;
    }
    
    StructuredData sd("");
    if (!sd.deserialize(blob))
    {
        return false;
    }

    int intVal;
    if (!sd.get("blobtype", intVal))
    {
        return false;
    }
    std::string strValue;
    if (!sd.get("value", strValue))
    {
        return false;
    }

    Blob::Type blobType = static_cast<Blob::Type>(intVal);
    
    //TODO check blobType == RAWDATA

    value_ = new Blob(name_, blobType, strValue);
    return true;
}

TOBool::TOBool(bool value, const std::string& name)
: TOBasicType<bool>(value, name)
{
    resultValue_.setValue(value);
}

Result TOBool::applyOperation(const Operation& operation)
{
    Result result = Result(true);
    switch (operation.getType())
    {
        case Operation::OperatorSet:
        {
            const std::vector<TObject *>& operands = operation.getObjects();
            const std::vector<std::string>& parameters = operation.getParameters();
            if (!operands.empty())
            {
                TOBool* other = dynamic_cast<TOBool *>(operands.back());
                value_ = other->getValue();
            }
            else if (!parameters.empty())
            {
                int otherValue = 0;
                std::istringstream iss(parameters.back());
                iss >> otherValue;
                value_ = otherValue;
            }
        }
            break;
        case Operation::OperatorZero:
            value_ = false;
            break;
        default:
            result = TOBasicType<bool>::applyOperation(operation);
            break;
    }
    
    return result;
}

bool TOBool::supportsOperation(const Operation& operation)
{
    Operation::Type operationType = operation.getType();
    bool retval = operationType == Operation::OperatorZero ||
                  operationType == Operation::OperatorSet;
    
    return retval || TOBasicType<bool>::supportsOperation(operation);
}

TOBool::operator bool() const
{
    return value_;
}

bool TOBool::serialize(Blob& blob)
{
    if (!TObject::serialize(blob))
    {
        return false;
    }
    
    StructuredData sd(TObjectType::NameBasicType, blob);
    sd.add("value", value_);
    
    return sd.serialize(blob);
}

bool TOBool::deserialize(const Blob& blob)
{
    if (!TObject::deserialize(blob))
    {
        return false;
    }
    
    StructuredData sd("");
    if (!sd.deserialize(blob))
    {
        return false;
    }
    
    int intValue;
    if (!sd.get("value", intValue))
    {
        return false;
    }
    value_ = intValue != 0;
    
    return true;
}


TOInteger::TOInteger(int value, const std::string& name)
: TOBasicType<int>(value, name)
{
    resultValue_.setValue(value);
}

Result TOInteger::applyOperation(const Operation& operation)
{
    Result result = Result(true);
    switch (operation.getType())
    {
        case Operation::OperatorAdd:
        {
            const std::vector<TObject *>& operands = operation.getObjects();
            const std::vector<std::string>& parameters = operation.getParameters();
            if (!operands.empty())
            {
                std::vector<TObject *>::const_iterator it;
                for (it = operands.begin(); it != operands.end(); ++it)
                {
                    TOInteger* other = dynamic_cast<TOInteger *>(*it);
                    value_ += other->getValue();
                }
            }
            else if (!parameters.empty())
            {
                std::vector<std::string>::const_iterator it;
                for (it = parameters.begin(); it != parameters.end(); ++it)
                {
                    int otherValue = 0;
                    std::istringstream iss(*it);
                    iss >> otherValue;
                    value_ += otherValue;
                }
            }
        }
            break;
        case Operation::OperatorSet:
        {
            const std::vector<TObject *>& operands = operation.getObjects();
            const std::vector<std::string>& parameters = operation.getParameters();
            if (!operands.empty())
            {
                TOInteger* other = dynamic_cast<TOInteger *>(operands.back());
                value_ = other->getValue();
            }
            else if (!parameters.empty())
            {
                int otherValue = 0;
                std::istringstream iss(parameters.back());
                iss >> otherValue;
                value_ = otherValue;
            }
        }
            break;
        case Operation::OperatorZero:
            value_ = 0;
            break;
        case Operation::OperatorIsTrue:
            if (!operator bool())
            {
                result = Result(false);
            }
            break;
        case Operation::OperatorIsFalse:
            if (operator bool())
            {
                result = Result(false);
            }
            break;
        default:
            result = TOBasicType<int>::applyOperation(operation);
            break;
    }

    return result;
}

bool TOInteger::supportsOperation(const Operation& operation)
{
    Operation::Type operationType = operation.getType();
    bool retval = operationType == Operation::OperatorAdd ||
                  operationType == Operation::OperatorZero ||
                  operationType == Operation::OperatorSet ||
                  operationType == Operation::OperatorIsTrue ||
                  operationType == Operation::OperatorIsFalse;

    return retval || TOBasicType<int>::supportsOperation(operation);
}

TOInteger::operator bool() const
{
    return value_;
}

bool TOInteger::serialize(Blob& blob)
{
    if (!TObject::serialize(blob))
    {
        return false;
    }
    
    StructuredData sd(TObjectType::NameBasicType, blob);
    sd.add("value", value_);
    
    return sd.serialize(blob);
}

bool TOInteger::deserialize(const Blob& blob)
{
    if (!TObject::deserialize(blob))
    {
        return false;
    }
    
    StructuredData sd("");
    if (!sd.deserialize(blob))
    {
        return false;
    }
    
    if (!sd.get("value", value_))
    {
        return false;
    }
    
    return true;
}


TOString::TOString(const std::string& value, const std::string& name)
: TOBasicType<std::string>(value, name)
{
    resultValue_.setValue(value);
}

TOString::operator bool() const
{
    return !value_.empty();
}

bool TOString::serialize(Blob& blob)
{
    if (!TObject::serialize(blob))
    {
        return false;
    }
    
    StructuredData sd(TObjectType::NameBasicType, blob);
    sd.add("value", value_);
    
    return sd.serialize(blob);
}

bool TOString::deserialize(const Blob& blob)
{
    if (!TObject::deserialize(blob))
    {
        return false;
    }
    
    StructuredData sd("");
    if (!sd.deserialize(blob))
    {
        return false;
    }
    
    if (!sd.get("value", value_))
    {
        return false;
    }
    
    return true;
}
