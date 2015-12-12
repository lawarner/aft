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

#include <cassert>
#include <sstream>

#include <json/json.h>
#include "blob.h"
#include "structureddata.h"
using namespace aft::base;


const std::string StructuredDataName::SEPARATOR(".");

////////////////// Helper functions

static std::string
join_(const std::vector<std::string>& elements,
      std::string delimiter = StructuredDataName::SEPARATOR)
{
    std::stringstream ss;
    size_t elems = elements.size();
    for (size_t i = 0; i < elems; ++i)
    {
        ss << elements[i];
        if (i < elems - 1) ss << delimiter;
    }

    return ss.str();
}

static std::vector<std::string>
split_(const std::string& full,
       const std::string& delimiter = StructuredDataName::SEPARATOR)
{
    //TODO check well-formed path.  it should not start or end with SEPARATOR
    std::vector<std::string> ret;
    size_t oldpos = 0;
    size_t pos = full.find(delimiter);
    while (pos != std::string::npos)
    {
        ret.push_back(full.substr(oldpos, pos - oldpos));
        oldpos = pos + 1;
        pos = full.find(delimiter, oldpos);
    }
    ret.push_back(full.substr(oldpos));

    return ret;
}

//////////////////

StructuredDataName::StructuredDataName(const std::string& name)
    : fullName_(name)
{
    if (fullName_.find(SEPARATOR) != std::string::npos)
    {
        path_ = split_(fullName_);
        //Assert !path_.empty()
        name_ = path_.back();
    } else {
        name_ = fullName_;
    }
}

StructuredDataName::StructuredDataName(const std::vector<std::string>& namePath)
    : path_(namePath)
{
    if (!path_.empty())
    {
        fullName_ = join_(path_, SEPARATOR);
        name_ = path_.back();
    }
}

StructuredDataName::~StructuredDataName()
{
}

std::string
StructuredDataName::getName(bool fullName) const
{
    return fullName ? fullName_ : name_;
}

const std::vector<std::string>&
StructuredDataName::getPath() const
{
    return path_;
}

bool StructuredDataName::operator==(const StructuredDataName& other)
{
    return fullName_ == other.fullName_ && path_.size() == other.path_.size();
}

////////////////////

StructuredDataDelegate*
StructuredDataDelegate::getDelegate(StructuredData& sd) const
{
    return sd.delegate_;  // just because we're friends
}


class JsonDataDelegate : public StructuredDataDelegate
{
public:
    JsonDataDelegate();
    virtual ~JsonDataDelegate();

    virtual bool add(const std::string& name, int intValue);
    virtual bool add(const std::string& name, const std::string& value);
    virtual bool add(const std::string& name, const StructuredData& value);
    virtual bool get(const std::string& name, int& intValue) const;
    virtual bool get(const std::string& name, std::string& value) const;
    virtual bool get(const std::string& name, StructuredData& value) const;

    virtual bool parse(const std::string& name, const std::string& strData);
    virtual bool unparse(const std::string& name, std::string& strData);

    virtual bool remove(const std::string& name);

    virtual Type type(const std::string& name);

private:
    Json::Value& json_;
};


JsonDataDelegate::JsonDataDelegate()
    : json_(*new Json::Value)
{
}

JsonDataDelegate::~JsonDataDelegate()
{
    delete &json_;
}

bool JsonDataDelegate::add(const std::string& name, int intValue)
{
    json_[name] = intValue;
    return true;
}

bool JsonDataDelegate::add(const std::string& name, const std::string& value)
{
    json_[name] = value;
    return true;
}

bool JsonDataDelegate::add(const std::string& name, const StructuredData& value)
{
    //TODO Check that value also has a JsonDataDelegate.  Cannot mix delegates.
    StructuredDataDelegate* delegate = getDelegate(const_cast<StructuredData&>(value));
    JsonDataDelegate* jsonDelegate = dynamic_cast<JsonDataDelegate *>(delegate);
    json_[name] = jsonDelegate->json_;

    return true;
}

bool JsonDataDelegate::get(const std::string& name, int& intValue) const
{
    if (json_.isMember(name))
    {
        intValue = json_[name].asInt();
        return true;
    }
    return false;
}

bool JsonDataDelegate::get(const std::string& name, std::string& value) const
{
    if (json_.isMember(name))
    {
        value = json_[name].asString();
        return true;
    }
    return false;
}

bool JsonDataDelegate::get(const std::string& name, StructuredData& value) const
{
    if (!json_.isMember(name))
    {
        return false;
    }

    //TODO Check that value also has a JsonDataDelegate.  Cannot mix delegates.
    JsonDataDelegate* delegate = dynamic_cast<JsonDataDelegate *>(getDelegate(value));
    delegate->json_ = json_[name];
    return true;
}

bool JsonDataDelegate::parse(const std::string& name, const std::string& strData)
{
    if (strData.empty())
    {
        return false;
    }

    Json::Value value;
    Json::Reader reader(Json::Features::strictMode());
    if (!reader.parse(strData, value, false))
    {
        return false;
    }
    if (name.empty())
    {
        json_ = value;
    } else {
        json_[name] = value;
    }
    return true;
}

bool JsonDataDelegate::unparse(const std::string& name, std::string& strData)
{
    if (name.empty())
    {
        Json::FastWriter writer;
        strData = writer.write(json_);
    } else {
        if (type(name) == NOTFOUND) return false;

        Json::FastWriter writer;
        strData = writer.write(json_[name]);
    }
    return true;
}

bool JsonDataDelegate::remove(const std::string& name)
{
    if (json_.removeMember(name) == Json::Value::null)
    {
        return false;
    }
    return true;
}

StructuredDataDelegate::Type JsonDataDelegate::type(const std::string& name)
{
    StructuredDataDelegate::Type retType = STRING;
    Json::Value jsonValue = json_[name];
    switch (jsonValue.type())
    {
    case Json::nullValue:
        retType = NOTFOUND;
        break;
    case Json::intValue:
    case Json::uintValue:
        retType = INT;
        break;
    case Json::realValue:    // reals are strings
    case Json::stringValue:
        retType = STRING;
        break;
    case Json::booleanValue:
        retType = INT;
        break;
    case Json::arrayValue:   //TODO arrays are not yet supported
        retType = NOTFOUND;
        break;
    case Json::objectValue:
        retType = STRUCTUREDDATA;
        break;
    }

    return retType;
}

////////////////////

StructuredData::StructuredData(const StructuredDataName& name,
                               const std::string& fromString,
                               StructuredDataDelegate* delegate)
    : name_(name)
    , delegate_(delegate ? delegate : new JsonDataDelegate)
{
    if (!fromString.empty())
    {
        Blob blob("serial", Blob::STRING, fromString);
        deserialize(blob);
    }
}

StructuredData::~StructuredData()
{
    delete delegate_;
}

bool StructuredData::add(const StructuredDataName& name, const Blob& blob)
{
    if (blob.getType() == Blob::STRING)
    {
        return delegate_->add(name.getName(), blob.getString());
    }
    else if (blob.getType() == Blob::JSON)
    {
        return delegate_->parse(name.getName(), blob.getString());
    }

    return false;
}

bool StructuredData::add(const StructuredDataName& name, const StructuredData& data)
{
    return delegate_->add(name.getName(), data);
}

bool StructuredData::add(const StructuredDataName& name, int intValue)
{
    return delegate_->add(name.getName(), intValue);
}

bool StructuredData::add(const StructuredDataName& name, const std::string& value)
{
    return delegate_->add(name.getName(), value);
}

bool StructuredData::get(const StructuredDataName& name, Blob& blob) const
{
    std::string strData;
    if (delegate_->get(name.getName(), strData))
    {
        blob = Blob(name.getName(true), Blob::STRING, strData);
        return true;
    }

    return false;
}

bool StructuredData::get(const StructuredDataName& name, StructuredData& data) const
{
    return delegate_->get(name.getName(), data);
}

bool StructuredData::get(const StructuredDataName& name, int& intValue) const
{
    return delegate_->get(name.getName(), intValue);
}

bool StructuredData::get(const StructuredDataName& name, std::string& value) const
{
    return delegate_->get(name.getName(), value);
}

std::string
StructuredData::get(const StructuredDataName& name,
                    const std::string& defValue) const
{
    std::string strval;
    if (delegate_->get(name.getName(), strval))
    {
        return strval;
    }

    return defValue;
}


bool StructuredData::remove(const StructuredDataName& name)
{
    return delegate_->remove(name.getName());
}

bool StructuredData::set(const StructuredDataName& name, const Blob& blob)
{
    if (delegate_->type(name.getName()) != StructuredDataDelegate::NOTFOUND)
    {
        return add(name, blob);
    }
    return false;
}

bool StructuredData::set(const StructuredDataName& name, const StructuredData& data)
{
    if (delegate_->type(name.getName()) != StructuredDataDelegate::NOTFOUND)
    {
        return add(name, data);
    }
    return false;
}

bool StructuredData::set(const StructuredDataName& name, int intValue)
{
    if (delegate_->type(name.getName()) != StructuredDataDelegate::NOTFOUND)
    {
        return add(name, intValue);
    }
    return false;
}

bool StructuredData::set(const StructuredDataName& name, const std::string& value)
{
    if (delegate_->type(name.getName()) != StructuredDataDelegate::NOTFOUND)
    {
        return add(name, value);
    }
    return false;
}


bool StructuredData::serialize(Blob& blob)
{
    std::string strData;
    if (delegate_->unparse("", strData))
    {
        blob = Blob(name_.getName(), Blob::STRING, strData);
        return true;
    }
    return false;
}

bool StructuredData::deserialize(const Blob& blob)
{
    //TODO check blob type is STRING
    name_ = StructuredDataName(blob.getName());
    return delegate_->parse(name_.getName(), blob.getString());
}
