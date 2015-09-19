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

namespace aft
{
namespace base
{
// Forward reference
class Command;
class TestObject;


class Result
{
public:
    enum ResultType
    {
        UNKNOWN = -1,
        FATAL,
        BOOLEAN,
        COMMAND,
        ITERATOR,
        TESTOBJECT
    };

    Result(ResultType type = BOOLEAN);
    virtual ~Result();

    ResultType getType() const;
    bool getValue(bool& value) const;
    bool getValue(Command*& command) const;
    bool getValue(TestObject*& object) const;
    void setValue(bool value);
    void setValue(Command* command);
    void setValue(TestObject* object);

private:
    ResultType type_;
    bool isValueSet_;
    union Values
    {
        bool flag_;
        Command* command_;
        void* iterator_;
        TestObject* object_;
    } value_;
};

} // namespace base
} // namespace aft
