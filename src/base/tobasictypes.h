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

#include "structureddata.h"
#include "tobject.h"
#include "tobjecttype.h"


namespace aft
{
namespace base
{
// Forward reference


template<typename T>
class TOBasicType : public TObject
{
public:
    typedef T value_type;

    /** Construct a TOBasicType with a given value and optional name */
    TOBasicType<T>(T value, const std::string& name = std::string())
    : TObject(TObjectType::TypeBasicType, name)
    , value_(value)
    {
        state_ = INITIAL;
    }
    
    /** Destruct a TOBool */
    virtual ~TOBasicType()
    {
        
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
        return value_ == other.value_;
    }

    /** Copy contents of this TOBasicType from another. */
    virtual TOBasicType<T> operator=(const TOBasicType<T>& other)
    {
        if (this != &other)
        {
            TObject::operator=(other);
            value_ = other.value_;
        }
        return *this;
    }
    
    T getValue() const
    {
        return value_;
    }
    
protected:
    T value_;
};

/**
 * Hold a boolean value.
 */
class TOBool : public TOBasicType<bool>
{
public:
    TOBool(bool value, const std::string& name = std::string())
    : TOBasicType<bool>(value, name)
    {
        
    }
    bool serialize(Blob& blob)
    {
        if (!TObject::serialize(blob))
        {
            return false;
        }
        
        StructuredData sd(TObjectType::NameBasicType, blob);
        sd.add("value", value_);
        
        return sd.serialize(blob);
    }
    bool deserialize(const Blob& blob)
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
};

/**
 * Hold a string value.
 */
class TOString : public TOBasicType<std::string>
{
public:
    TOString(const std::string& value, const std::string& name = std::string())
    : TOBasicType<std::string>(value, name)
    {
        
    }
    bool serialize(Blob& blob)
    {
        if (!TObject::serialize(blob))
        {
            return false;
        }
        
        StructuredData sd(TObjectType::NameBasicType, blob);
        sd.add("value", value_);
        
        return sd.serialize(blob);
    }
    bool deserialize(const Blob& blob)
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
};
    

/**
 * Hold a blob.
 * TODO override (de)serialize
 */
typedef TOBasicType<Blob *> TOBlob;


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
