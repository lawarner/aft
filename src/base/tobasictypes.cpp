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
#include "tobasictypes.h"
using namespace aft::base;

TOBool& aft::base::TOTrue  = *new TOBool(true, "true");
TOBool& aft::base::TOFalse = *new TOBool(false, "false");


TOBool::TOBool(bool value, const std::string& name)
    : TObject(name)
    , value_(value)
{
    state_ = INITIAL;
}

TOBool::~TOBool()
{
}

const Result TOBool::run(Context* context)
{
    return Result(this);
}

bool TOBool::operator==(const TOBool& other) const
{
//    return TObject::operator==(other) && (value_ == other.value_);
    return value_ == other.value_;
}

Blob* TOBool::serialize()
{
    return TObject::serialize();
}

bool TOBool::deserialize(const Blob* blob)
{
    return TObject::deserialize(blob);
}

bool TOBool::getValue() const
{
    return value_;
}


TOString::TOString(const std::string& value, const std::string& name)
    : TObject(name)
    , value_(value)
{
}

TOString::~TOString()
{
}

const Result TOString::run(Context* context)
{
    return Result(this);
}

bool TOString::operator==(const TOString& other)
{
    return TObject::operator==(other) && (value_ == other.value_);
}

Blob* TOString::serialize()
{
    return TObject::serialize();
}

bool TOString::deserialize(const Blob* blob)
{
    return TObject::deserialize(blob);
}

const std::string& TOString::getValue() const
{
    return value_;
}


TOBlob::TOBlob(const Blob* blob, const std::string& name)
    : TObject(name)
    , blob_(blob)
{
}

TOBlob::~TOBlob()
{
}

const Result
TOBlob::run(Context* context)
{
    return Result(this);
}

bool TOBlob::operator==(const TOBlob& other)
{
    return TObject::operator==(other) && (blob_ == other.blob_);
}

Blob* TOBlob::serialize()
{
    return TObject::serialize();
}

bool TOBlob::deserialize(const Blob* blob)
{
    return TObject::deserialize(blob);
}

const Blob*
TOBlob::getValue() const
{
    return blob_;
}

