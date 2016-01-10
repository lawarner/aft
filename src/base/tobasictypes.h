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


/**
 *  Defines some primitive TObject types that are primarily (const) data holders.
 *
 *  TODO this is a prime candidate for some templating/specialization refactor.
 */

/**
 * Hold a boolean value.
 */
class TOBool : public TObject
{
public:
    /** Construct a TOBool with a given value and optional name */
    TOBool(bool value, const std::string& name = std::string());

    /** Destruct a TOBool */
    virtual ~TOBool();

    /** Run in the given context */
    virtual const Result run(Context* context);

    /** Test if equal to another TOBool.
     *
     *   If the boolean value_ is equal then the 2 TOBool's are considered equal.
     */
    virtual bool operator==(const TOBool& other) const;

    // Implement SerializeContract
    virtual bool serialize(Blob& blob);
    virtual bool deserialize(const Blob& blob);

    bool getValue() const;

private:
    bool value_;
};


/**
 * Hold a string value.
 */
class TOString : public TObject
{
public:
    /** Construct a TOString with a given value and optional name */
    TOString(const std::string& value, const std::string& name = std::string());

    /** Destruct a TOString */
    virtual ~TOString();

    /** Run in the given context */
    virtual const Result run(Context* context);

    /** Test if equal to another TOString */
    virtual bool operator==(const TOString& other);

    // Implement SerializeContract
    virtual bool serialize(Blob& blob);
    virtual bool deserialize(const Blob& blob);

    const std::string& getValue() const;

private:
    std::string value_;
};

/** Global singleton for 'true' */
extern TOBool& TOTrue;
/** Global singleton for 'false' */
extern TOBool& TOFalse;

/**
 * Hold a blob
 */
class TOBlob : public TObject
{
public:
    /** Construct a TOBlob and pass a blob */
    TOBlob(const Blob* blob, const std::string& name = std::string());

    /** Destruct a TOBlob */
    virtual ~TOBlob();

    /** Run in the given context */
    virtual const Result run(Context* context);

    /** Test if equal to another TOBlob */
    virtual bool operator==(const TOBlob& other);

    // Implement SerializeContract
    virtual bool serialize(Blob& blob);
    virtual bool deserialize(const Blob& blob);

    const Blob* getValue() const;

private:
    const Blob* blob_;
};

} // namespace base
} // namespace aft
