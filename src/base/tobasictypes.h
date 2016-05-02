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

#include "tobject.h"


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
    : TObject(name)
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
    
    // Implement SerializeContract
    virtual bool serialize(Blob& blob)
    {
        return TObject::serialize(blob);
    }
    virtual bool deserialize(const Blob& blob)
    {
        return TObject::deserialize(blob);
    }

    T getValue() const
    {
        return value_;
    }
    
private:
    T value_;
};

/**
 * Hold a boolean value.
 */
typedef TOBasicType<bool> TOBool;

/**
 * Hold a string value.
 */
typedef TOBasicType<std::string> TOString;

/**
 * Hold a blob
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
