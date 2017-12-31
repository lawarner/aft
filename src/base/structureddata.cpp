/*
 *   Copyright 2015-2017 Andy Warner
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
#include <iostream>
#include <sstream>

#include <json/json.h>
#include "blob.h"
#include "structureddata.h"
using namespace aft::base;
using std::string;


StructuredDataDelegate*
StructuredDataDelegate::getDelegate(StructuredData& sd)
{
    return sd.delegate_;  // just because we're friends
}

void StructuredDataDelegate::setSDName(StructuredData& sd, const StructuredDataName& name) const
{
    sd.name_ = name;     // take advantage of our friendship. again.
}


/** Implement the StructuredDataDelegate interface via internal json. */
class JsonDataDelegate : public StructuredDataDelegate
{
public:
    JsonDataDelegate();
    JsonDataDelegate(const JsonDataDelegate& other);
    virtual ~JsonDataDelegate();

    virtual bool add(const StructuredDataName& name, int intValue);
    virtual bool add(const StructuredDataName& name, const std::string& value);
    virtual bool add(const StructuredDataName& name, const StructuredData& value);
    virtual bool addArray(const StructuredDataName& name);
    virtual bool get(const StructuredDataName& name, int& intValue) const;
    virtual bool get(const StructuredDataName& name, std::string& value) const;
    virtual bool get(const StructuredDataName& name, StructuredData& value) const;
    virtual bool getArray(const StructuredDataName& name, 
                          std::vector<std::string>& values) const;

    virtual bool getMembers(std::vector<std::string>& names) const;

    virtual bool parse(const StructuredDataName& name, const std::string& strData);
    virtual bool unparse(const StructuredDataName& name, std::string& strData);

    virtual bool remove(const StructuredDataName& name);

    virtual Type type(const StructuredDataName& name);

private:
    Json::Value* getJsonPtr(const StructuredDataName& name,
                            bool pathOnly = false) const;

    /** Get the JSON value with a given name.
     *  Deprecated, as getJsonPtr is faster and preferred.
     *  This is an internal only method (thus private).
     *
     *  TODO support arrays:
     *  An idea is to always init json_ as an array and for non-array SDs just
     *  use element 0.
     *  @param name  Name to add.
     *  @param value The JSON value is returned into this element.  If this 
     *               method returns false, then this parameter is set to the
     *               last value (in the hierarchy) that was found.
     *  @param pathOnly If true, get the value of the "container" that holds name, 
     *                  which is used when adding a new element with a given name.
     *                  If false, gets the value of name.
     *  @return true if the named value was found, otherwise false.
     */
    bool getJsonValue(const StructuredDataName& name, Json::Value& value,
                      bool pathOnly = false) const;
    Json::Value* makePathJson(const StructuredDataName& name);

private:
    Json::Value& json_;
};


JsonDataDelegate::JsonDataDelegate()
    : json_(*new Json::Value(Json::objectValue))
{
    //TODO possibly set json_ name
}

JsonDataDelegate::JsonDataDelegate(const JsonDataDelegate& other)
: json_(*new Json::Value(other.json_))
{
    //TODO possibly set json_ name
}

JsonDataDelegate::~JsonDataDelegate()
{
    delete &json_;
}

bool JsonDataDelegate::add(const StructuredDataName& name, int intValue)
{
    if (name.getPath().empty()) {
        if (name.getName().empty() || json_.type() != Json::objectValue) {
            return false;
        }
        json_[name.getName()] = intValue;
    }
    else {
        Json::Value* val = (Json::Value*) getJsonPtr(name, true);
        if (!val) {
            val = makePathJson(name.getParent());
        }
        if (!val) {
            return false;
        }
        if (name.getName().empty()) {
            if (val->isArray()) {
                Json::Value retval = val->append(intValue);
                if (retval.isNull()) {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            (*val)[name.getName()] = intValue;
        }
    }

    return true;
}

// To add to an array, getPath() cannot be empty, for example "myarr." or "myobj.myarr."
bool JsonDataDelegate::add(const StructuredDataName& name, const std::string& value)
{
    if (name.getPath().empty()) {
        if (name.getName().empty() || json_.type() != Json::objectValue) {
            return false;
        }
        json_[name.getName()] = value;
    }
    else {
        Json::Value* val = (Json::Value*) getJsonPtr(name, true);
        if (!val) {
            val = makePathJson(name.getParent());
        }
        if (!val) {
            return false;
        }
        if (name.getName().empty()) {
            if (val->isArray()) {
                Json::Value retval = val->append(value);
                if (retval.isNull()) {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            (*val)[name.getName()] = value;
        }
    }

    return true;
}

bool JsonDataDelegate::add(const StructuredDataName& name, const StructuredData& value)
{
    //TODO Check that value also has a JsonDataDelegate.  Cannot mix delegates.
    StructuredDataDelegate* delegate = getDelegate(const_cast<StructuredData&>(value));
    JsonDataDelegate* jsonDelegate = dynamic_cast<JsonDataDelegate *>(delegate);

    if (name.getPath().empty()) {
        if (name.getName().empty()) {
            json_ = jsonDelegate->json_;;
        } else {
            if (json_.type() != Json::objectValue) {
                return false;
            }
            json_[name.getName()] = jsonDelegate->json_;
        }
    }
    else {
        Json::Value* val = (Json::Value*) getJsonPtr(name, true);
        if (!val) {
            val = makePathJson(name.getParent());
        }
        if (!val) {
            return false;
        }
        if (name.getName().empty())
        {
            if (val->isArray())
            {
                Json::Value retval = val->append(jsonDelegate->json_);
                if (retval.isNull()) {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            (*val)[name.getName()] = jsonDelegate->json_;
        }
    }

    return true;
}

bool JsonDataDelegate::addArray(const StructuredDataName& name) {
    if (name.getPath().empty()) {
        json_[name.getName()] = Json::Value(Json::arrayValue);
    }
    else {
        Json::Value val;
        if (!getJsonValue(name, val, true)) return false;  // container not found, cannot add
        val[name.getName()] = Json::Value(Json::arrayValue);
    }

    return true;
}

bool JsonDataDelegate::get(const StructuredDataName& name, int& intValue) const {
    if (nullptr == getJsonPtr(name, false)) return false;
    Json::Value* val = getJsonPtr(name, true);
    if (!val) return false;
    if (val->isArray()) {
        if (name.getName().empty()) return false;
        int idx = 0;
        std::istringstream iss(name.getName());
        iss >> idx;
        intValue = (*val)[idx].asInt();
    } else {
        if (!val->isMember(name.getName())) {
            return false;
        }
        intValue = (*val)[name.getName()].asInt();
    }

    return true;
}

bool JsonDataDelegate::get(const StructuredDataName& name, std::string& value) const {
    if (nullptr == getJsonPtr(name, false)) return false;
    Json::Value* val = getJsonPtr(name, true);
    if (nullptr == val) return false;
    if (val->isArray()) {
        if (name.getName().empty()) return false;
        int idx = 0;
        std::istringstream iss(name.getName());
        iss >> idx;
        value = (*val)[idx].asString();
    } else {
        if (!val->isMember(name.getName())) {
            return false;
        }
        value = (*val)[name.getName()].asString();
    }

    return true;
}

bool JsonDataDelegate::get(const StructuredDataName& name, StructuredData& value) const {
    if (nullptr == getJsonPtr(name, false)) return false;
    Json::Value* val = getJsonPtr(name, true);
    if (!val) return false;
    //TODO Check that value also has a JsonDataDelegate.  Cannot mix delegates.
    JsonDataDelegate* delegate = dynamic_cast<JsonDataDelegate *>(getDelegate(value));

    if (name.empty()) {
        delegate->json_ = json_;
        return true;
    }
    else if (val->isArray())
    {
        if (name.getName().empty()) return false;
        int idx = 0;
        std::istringstream iss(name.getName());
        iss >> idx;
        delegate->json_ = (*val)[idx];
    }
    else {
        if (!val->isMember(name.getName())) {
            return false;
        }
        delegate->json_ = (*val)[name.getName()];
    }
    setSDName(value, name);

    return true;
}

bool JsonDataDelegate::getArray(const StructuredDataName& name, 
                                std::vector<std::string>& values) const
{
    if (name.getName().empty()) return false;

    Json::Value* val = getJsonPtr(name);
    if (!val || !val->isArray()) return false;
    for (int idx = 0; idx < (int)val->size(); ++idx)
    {
        Json::Value& jval = (*val)[idx];
        if (jval.isConvertibleTo(Json::stringValue))
        {
            values.push_back(jval.asString());
        } else {
            Json::FastWriter writer;
            std::string strData = writer.write(jval);
            values.push_back(strData);
        }
    }

    return true;
}

bool JsonDataDelegate::getMembers(std::vector<std::string>& names) const {
    names = json_.getMemberNames();
    return true;
}

Json::Value*
JsonDataDelegate::getJsonPtr(const StructuredDataName& name, bool pathOnly) const {
    if (name.empty()) {
        return &json_;
    }
    Json::Value* retval = nullptr;
    const auto& names = name.getComponents();
    size_t numElements = names.size();
    if (numElements == 0) return retval;
    if (pathOnly) --numElements;
    
    //TODO should we check json_.isNull()?
    retval = &json_;
    for (size_t idx = 0; idx < numElements; ++idx) {
        if (Json::objectValue == retval->type()) {
            retval = (Json::Value *)retval->find(names[idx].c_str(), names[idx].c_str() + names[idx].size());
        }
        else if (retval->isArray()) {
            if (string::npos == names[idx].find_first_not_of("0123456789")) {
                int iname = std::stoi(names[idx]);
                if (iname >= 0 && iname < (int)retval->size()) {
                    retval = &(*retval)[iname];
                }
                else {
                    retval = nullptr;
                }
            }
            else if (retval->isMember(names[idx])) {
                retval = &(*retval)[names[idx]];
            }
            else {
                retval = nullptr;
            }
        }
        if (!retval) break;
    }

    return retval;
}

bool JsonDataDelegate::getJsonValue(const StructuredDataName& name,
                                    Json::Value& value, bool pathOnly) const {
    const auto& names = name.getComponents();
    size_t numElements = names.size();
    if (numElements == 0) return false;
    if (pathOnly) {
        --numElements;
        if (names.back().empty()) {
            if (numElements > 0) --numElements;
        }
    }

    //TODO should we check json_.isNull()?
    value = json_;
    for (size_t idx = 0; idx < numElements; ++idx) {
        if (!value.isMember(names[idx])) return false;
        value = value[names[idx]];
    }

    return true;
}

Json::Value*
JsonDataDelegate::makePathJson(const StructuredDataName& name) {
    Json::Value* retval = nullptr;
    const std::vector<std::string>& names = name.getComponents();
    size_t numElements = names.size();
    if (numElements == 0) return retval;
    
    retval = &json_;
    for (size_t idx = 0; idx < numElements; ++idx) {
        if (!retval->find(names[idx].c_str(), names[idx].c_str() + names[idx].size())) {
            (*retval)[names[idx]] = *new Json::Value(Json::objectValue);
        }
        retval = const_cast<Json::Value*>(retval->find(names[idx].c_str(), names[idx].c_str() + names[idx].size()));
    }
    
    return retval;
}

bool JsonDataDelegate::parse(const StructuredDataName& name, const std::string& strData) {
    if (strData.empty()) {
        return false;
    }

    Json::Value receiveVal;
    if (!name.empty())
    {
        if (!getJsonValue(name, receiveVal)) return false;
    }

    Json::Value value;
    Json::Reader reader(Json::Features::strictMode());
    if (!reader.parse(strData, value, false))
    {
        std::cout << "Error parsing JSON:\n" << reader.getFormattedErrorMessages() << std::endl;
        return false;
    }

    if (name.empty())
    {
        json_ = value;
    } else {
        receiveVal = value;
    }
#if 0
    if (name.empty())
    {
        json_ = value;
    } else {
        json_[name] = value;
    }
#endif
    return true;
}

bool JsonDataDelegate::unparse(const StructuredDataName& name, std::string& strData) {
    if (name.empty()) {
#if 1
        Json::StyledWriter writer;
        strData = writer.write(json_);
#else
        Json::FastWriter writer;
        strData = writer.write(json_);
#endif
    } else {
        Json::Value value;
        if (!getJsonValue(name, value)) return false;

        Json::FastWriter writer;
        strData = writer.write(value);
    }
    return true;
}

bool JsonDataDelegate::remove(const StructuredDataName& name)
{
    Json::Value* value = getJsonPtr(name, true);
    if (nullptr == value) {
        return false;      // container not found
    }

    if (value->removeMember(name.getName()) == Json::Value::null) {
        return false;
    }
    return true;
}

StructuredDataDelegate::Type
JsonDataDelegate::type(const StructuredDataName& name)
{
    StructuredDataDelegate::Type retType = NOTFOUND;
    Json::Value jsonValue;
    if (!getJsonValue(name, jsonValue)) return retType;

    switch (jsonValue.type())
    {
    case Json::nullValue:
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
    case Json::arrayValue:
        retType = ARRAY;
        break;
    case Json::objectValue:
        retType = STRUCTUREDDATA;
        break;
    }

    return retType;
}

////////////////////////////

StructuredData::StructuredData(const StructuredDataName& name,
                               const std::string& fromString,
                               StructuredDataDelegate* delegate)
    : name_(name)
    , delegate_(delegate ? delegate : new JsonDataDelegate) {

    if (!fromString.empty()) {
        Blob blob("", Blob::STRING, fromString);
        deserialize(blob);
    }
}

StructuredData::StructuredData(const StructuredDataName& name, const Blob& blob,
                               StructuredDataDelegate* delegate)
: name_(name)
, delegate_(delegate ? delegate : new JsonDataDelegate)
{
    const void* data = blob.getData();
    if (data) // This is a special case where an SD pointer is copied here.
    {
        if (!delegate) delete delegate_;
        const JsonDataDelegate* sdDelegate = static_cast<const JsonDataDelegate *>(data);
        delegate_ = new JsonDataDelegate(*sdDelegate);
    }
    else
    {
        std::string fromString = blob.getString();
        if (!fromString.empty())
        {
            Blob blob("", Blob::STRING, fromString);
            deserialize(blob);
        }
    }
}

StructuredData::~StructuredData()
{
    delete delegate_;
}

bool StructuredData::add(const StructuredDataName& name, const Blob& blob)
{
    if (blob.getType() == Blob::JSON)
    {
        return delegate_->parse(name, blob.getString());
    }

    //TODO add struct to include type, name and string
    //TODO handle blob members
    return delegate_->add(name, blob.getString());
}

bool StructuredData::add(const StructuredDataName& name, const StructuredData& data)
{
    return delegate_->add(name, data);
}

bool StructuredData::add(const StructuredDataName& name, int intValue)
{
    return delegate_->add(name, intValue);
}

bool StructuredData::add(const StructuredDataName& name, const std::string& value)
{
    return delegate_->add(name, value);
}

bool StructuredData::addArray(const StructuredDataName& name)
{
    StructuredDataDelegate::Type sdType = delegate_->type(name);

    if (sdType == StructuredDataDelegate::ARRAY)
    {
        return true;	// Name already exists as an array, so good.
    }
    else if (sdType != StructuredDataDelegate::NOTFOUND)
    {
        return false;	// Name already exists but is not an array. Not so good.
    }

    return delegate_->addArray(name);
}

bool StructuredData::get(const StructuredDataName& name, Blob& blob) const
{
    std::string strData;
    if (delegate_->get(name, strData))
    {
        blob = Blob(name.getName(true), Blob::STRING, strData);
        return true;
    }

    return false;
}

bool StructuredData::get(const StructuredDataName& name, StructuredData& data) const
{
    return delegate_->get(name, data);
}

bool StructuredData::get(const StructuredDataName& name, int& intValue) const
{
    return delegate_->get(name, intValue);
}

bool StructuredData::get(const StructuredDataName& name, std::string& value) const
{
    return delegate_->get(name, value);
}

std::string
StructuredData::get(const StructuredDataName& name,
                    const std::string& defValue) const
{
    std::string strval;
    if (delegate_->get(name, strval))
    {
        return strval;
    }

    return defValue;
}

bool StructuredData::getArray(const StructuredDataName& name, 
                              std::vector<std::string>& values) const
{
    return delegate_->getArray(name, values);
}

bool StructuredData::getMembers(std::vector<std::string>& names) const {
    return delegate_->getMembers(names);
}

const StructuredDataName&
StructuredData::getName() const {
    return name_;
}

void StructuredData::setName(const StructuredDataName& name) {
    name_ = name;
}

bool StructuredData::isArray(const StructuredDataName& name) const
{
    return delegate_->type(name) == StructuredDataDelegate::ARRAY;
}

bool StructuredData::remove(const StructuredDataName& name)
{
    return delegate_->remove(name);
}

bool StructuredData::set(const StructuredDataName& name, const Blob& blob)
{
    if (delegate_->type(name) != StructuredDataDelegate::NOTFOUND)
    {
        return add(name, blob);
    }
    return false;
}

bool StructuredData::set(const StructuredDataName& name, const StructuredData& data)
{
    if (delegate_->type(name) != StructuredDataDelegate::NOTFOUND)
    {
        return add(name, data);
    }
    return false;
}

bool StructuredData::set(const StructuredDataName& name, int intValue)
{
    if (delegate_->type(name) != StructuredDataDelegate::NOTFOUND)
    {
        return add(name, intValue);
    }
    return false;
}

bool StructuredData::set(const StructuredDataName& name, const std::string& value)
{
    if (delegate_->type(name) != StructuredDataDelegate::NOTFOUND)
    {
        return add(name, value);
    }
    return false;
}


bool StructuredData::serialize(Blob& blob)
{
    std::string strData;
    if (delegate_->unparse("", strData)) {
        blob = Blob(name_.getName(true), Blob::STRING, strData); // or JSON?
        return true;
    }
    return false;
}

bool StructuredData::deserialize(const Blob& blob)
{
    if (blob.getType() != Blob::STRING && blob.getType() != Blob::JSON)
    {
        return false;
    }

//    name_ = StructuredDataName(blob.getName());
//    return delegate_->parse(name_, blob.getString());
    return delegate_->parse("", blob.getString());
}
