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

#include <string>

namespace aft
{
namespace base
{
// Forward reference
class Blob;
class Command;
class TObject;

/**
 * This is a handy wrapper for return types, since it can have various types.
 * This should replace many cases where a reference to a TObject is returned.
 */
//TODO make serializable
class Result
{
public:
    enum ResultType
    {
        UNKNOWN = -1,
        FATAL,
        BLOB,
        BOOLEAN,
        COMMAND,
        ITERATOR,
        STRING,
        TOBJECT
    };

    Result(ResultType type = BOOLEAN);
    Result(Blob* blob);
    Result(bool value);
    Result(Command* command);
    Result(const std::string& strValue);
    Result(TObject* object);

    virtual ~Result();

    std::string asString() const;

    ResultType getType() const;
    std::string getTypeName() const;
    bool getValue(Blob*& blob) const;
    bool getValue(bool& value) const;
    bool getValue(Command*& command) const;
    bool getValue(std::string& strValue) const;
    bool getValue(TObject*& object) const;
    void setValue(Blob* blob);
    void setValue(bool value);
    void setValue(Command* command);
    void setValue(const std::string& strValue);
    void setValue(TObject* object);

    /** Test if this result is equal to another */
    bool operator==(const Result& other) const;
    
    /** Test if this result is not equal to another */
    bool operator!=(const Result& other) const;
    
    /** Determine if this result is false.
     *  TODO maybe incorporate FATAL
     *  @return true if either the result is not set or it holds a false boolean value.
     */
    bool operator!() const;

    // Copy constructor and assignment operators
    // (for now the default do shallow copies, as needed)

private:
    ResultType type_;
    bool isValueSet_;
    union Values
    {
        Blob* blob_;
        bool flag_;
        Command* command_;
        void* iterator_;
        std::string* string_;
        TObject* object_;
    } value_;
};

} // namespace base
} // namespace aft
