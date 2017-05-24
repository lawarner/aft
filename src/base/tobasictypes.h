#pragma once
/*
 *   Copyright © 2015-2017 Andy Warner
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

#include "result.h"
#include "structureddata.h"
#include "tobject.h"
#include "tobjecttype.h"


namespace aft {
namespace base {

/** Non-templated base class for TOBasicType */
class TOBasicTypeBase : public TObject {
protected:
    TOBasicTypeBase(const TObjectType& type, const std::string& name = std::string());
    virtual ~TOBasicTypeBase();

public:
    //virtual bool isSameType(const TOBasicTypeBase& other);
    virtual Result getValueAsResult() const;

protected:
    Result resultValue_;
};


template<typename T>
class TOBasicType : public TOBasicTypeBase
{
public:
    typedef T value_type;

    /** Construct a TOBasicType with a given value and optional name */
    TOBasicType<T>(T value, const std::string& name = std::string())
    : TOBasicTypeBase(TObjectType::TypeBasicType, name)
    , value_(value) {
        state_ = INITIAL;
    }
    
    /** Construct a TOBasicType with a given value and optional name */
    TOBasicType<T>(const TObjectType& objectType, T value, const std::string& name = std::string())
    : TOBasicTypeBase(objectType, name)
    , value_(value) {
        state_ = INITIAL;
    }
    
    /** Destruct a TOBasicType */
    virtual ~TOBasicType() = default;

    /** Compare this basic object to another.
     *  @return If this object is greater than other returns 1.
     *          If objects are equal returns 0.
     *          If this object is less than other returns -1.
     */
    virtual int compare(const TOBasicType<T>& other) const
    {
        if (this == &other) return 0;
        if (value_ > other.value_) return  1;
        if (value_ < other.value_) return -1;
        return 0;
    }

    virtual Result applyOperation(const Operation& operation)
    {
        Result result(Result::FATAL);
        if (!operation.isValid()) {
            return result;
        }

        // All of the comparison operations take 1 other object
        if (!operation.getObjects().empty()) {
            TObject* other = operation.getObjects().front();
            TOBasicType<T>& otherRef = dynamic_cast<TOBasicType<T> &>(*other);
            int compareResult = compare(otherRef);
            
            switch (operation.getType()) {
                case Operation::OperatorCompare:
                    result = Result(compareResult);
                    break;
                case Operation::OperatorIsLessThan:
                    result = Result(compareResult == -1);
                    break;
                case Operation::OperatorIsLessThanOrEqual:
                    result = Result(compareResult <= 0);
                    break;
                case Operation::OperatorIsEqual:
                    result = Result(compareResult == 0);
                    break;
                case Operation::OperatorIsGreaterThanOrEqual:
                    result = Result(compareResult >= 0);
                    break;
                case Operation::OperatorIsGreaterThan:
                    result = Result(compareResult == 1);
                    break;
                default:
                    result = TObject::applyOperation(operation);
                    break;
            }
        }
        else {
            switch (operation.getType()) {
                case Operation::OperatorIsTrue:
                    result = Result(operator bool());
                    break;
                case Operation::OperatorIsFalse:
                    result = Result(!operator bool());
                    break;
                default:
                    result = TObject::applyOperation(operation);
                    break;
            }
        }

        return result;
    }
    
    virtual bool supportsOperation(const Operation& operation)
    {
        Operation::Type opType = operation.getType();
        bool retval = opType == Operation::OperatorCompare ||
        opType == Operation::OperatorIsLessThan ||
        opType == Operation::OperatorIsLessThanOrEqual ||
        opType == Operation::OperatorIsEqual ||
        opType == Operation::OperatorIsGreaterThanOrEqual ||
        opType == Operation::OperatorIsGreaterThan ||
        opType == Operation::OperatorIsTrue ||
        opType == Operation::OperatorIsFalse;

        return retval || TObject::supportsOperation(operation);
    }

    /** Run in the given context */
    virtual const Result run(Context* context)
    {
        return Result(this);
    }

    /** Test if equal to another TOBasicType<T>.
     *
     *   If the value_ is equal then the 2 TOBasicType's are considered equal.
     */
    virtual bool operator==(const TOBasicType<T>& other) const
    {
        if (this == &other) return true;
        return value_ == other.value_;
    }

    /** Copy contents of this TOBasicType from another. */
    virtual TOBasicType<T> operator=(const TOBasicType<T>& other) {
        if (this != &other) {
            TObject::operator=(other);
            value_ = other.value_;
        }
        return *this;
    }
    
    virtual operator bool() const {
        return false;
    }

    T getValue() const {
        return value_;
    }
    
protected:
    T value_;
};


/**
 * Hold a blob.
 */
class TOBlob : public TOBasicType<Blob *> {

public:
    TOBlob(Blob* value, const std::string& name = std::string());
protected:
    TOBlob(const TObjectType& objType, Blob* value, const std::string& name = std::string());
    
public:
    virtual operator bool() const;
    bool serialize(Blob& blob);
    bool deserialize(const Blob& blob);
};
    
/**
 * Hold a boolean value.
 */
class TOBool : public TOBasicType<bool>
{
public:
    TOBool(bool value, const std::string& name = std::string());

    Result applyOperation(const Operation& operation);
    bool supportsOperation(const Operation& operation);
    
    virtual operator bool() const;
    bool serialize(Blob& blob);
    bool deserialize(const Blob& blob);
};

/**
 * Hold an int value.
 */
class TOInteger : public TOBasicType<int>
{
public:
    TOInteger(int value, const std::string& name = std::string());

    Result applyOperation(const Operation& operation);
    bool supportsOperation(const Operation& operation);

    virtual operator bool() const;
    bool serialize(Blob& blob);
    bool deserialize(const Blob& blob);
};

/**
 * Hold a string value.
 */
class TOString : public TOBasicType<std::string>
{
public:
    TOString(const std::string& value, const std::string& name = std::string());

    virtual operator bool() const;
    bool serialize(Blob& blob);
    bool deserialize(const Blob& blob);
};

/**
 *  Defines some primitive TObject types that are primarily (const) data holders.
 *
 *  TODO add more constants such as TONull
 */
    
/** Global singleton for 'true' */
extern TOBool& TOTrue;
/** Global singleton for 'false' */
extern TOBool& TOFalse;

} // namespace base
} // namespace aft
