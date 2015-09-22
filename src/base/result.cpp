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

#include "result.h"
using namespace aft::base;


Result::Result(ResultType type)
    : type_(type)
    , isValueSet_(false)
{

}

Result::~Result()
{

}

Result::ResultType
Result::getType() const
{
    return type_;
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

bool Result::getValue(TObject*& object) const
{
    if (!isValueSet_ || type_ != TOBJECT) return false;

    object = value_.object_;
    return true;
}

void Result::setValue(bool value)
{
    //TODO assert type is BOOLEAN
    value_.flag_ = value;
    isValueSet_ = true;
}

void Result::setValue(Command* command)
{
    value_.command_ = command;
    isValueSet_ = true;
}

void Result::setValue(TObject* object)
{
    value_.object_ = object;
    isValueSet_ = true;
}

