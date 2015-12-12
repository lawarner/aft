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

#include <iostream>
#include <base/blob.h>
#include <base/context.h>
#include <base/factory.h>
#include <base/tobject.h>
#include <gtest/gtest.h>
using namespace aft::base;


class TOSample : public TObject
{
public:
    /** Construct a TOSample with a given value and optional name */
    TOSample(const std::string& value, const std::string& name = std::string())
        : TObject(name)
        , value_(value)
        {  }

    /** Destruct a TOSample */
    virtual ~TOSample() {  }

    /** Run in the given context */
    virtual const Result run(Context* context)
        {
            std::cout << "Sample TO: " << value_ << std::endl;
            return Result(this);
        }

    /** Test if equal to another TOSample */
    virtual bool operator==(const TOSample& other)
        {
            return TObject::operator==(other) && (value_ == other.value_);
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

    const std::string& getValue() const
        {
            return value_;
        }

private:
    std::string value_;
};


class SampleFactory : public BaseFactory
{
public:
    SampleFactory()
        : BaseFactory("TObject", "SampleFactory")
        {  }
    virtual ~SampleFactory() { }

    virtual TObject* construct(const std::string& name, const Blob* blob = 0,
                               const Context* context = 0);

};

TObject*
SampleFactory::construct(const std::string& name, const Blob* blob,
                               const Context* context)
{
    std::string value("(no value)");
    if (blob)
    {
        value = blob->getString();
    }

    TOSample* tobj = new TOSample(value, name);
    return tobj;
}


extern "C"
{
aft::base::BaseFactory* initialize()
{
    return new SampleFactory;
}

void deinitialize()
{
}

}

