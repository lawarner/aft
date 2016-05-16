/*
 *  tobjecttype.cpp
 *  libaft
 *
 *   Copyright © 2016 Andy Warner. All rights reserved.
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
 *
 */

#include "tobasictypes.h"
#include "tobjecttype.h"
using namespace aft::base;
using namespace std;


std::map<std::string, TObjectType*> TObjectType::typeRegistry_;

const std::string TObjectType::NameBase("Base");
const std::string TObjectType::NameBasicType("BasicType");
const std::string TObjectType::NameCommand("Command");
const std::string TObjectType::NameTestCase("TestCase");
const std::string TObjectType::NameTestSuite("TestSuite");

const TObjectType& TObjectType::TypeBase      = TObjectType::get(NameBase);
const TObjectType& TObjectType::TypeBasicType = TObjectType::get(NameBasicType);
const TObjectType& TObjectType::TypeCommand   = TObjectType::get(NameCommand);
const TObjectType& TObjectType::TypeTestCase  = TObjectType::get(NameTestCase);
const TObjectType& TObjectType::TypeTestSuite = TObjectType::get(NameTestSuite);

//TODO rework this static dependency problem
//     This is a bad workaround, but I put all static TObject initializers here
//     because they depend on TObjectType's class members being initialized first.
TOBool& aft::base::TOTrue  = *new TOBool(true, "true");
TOBool& aft::base::TOFalse = *new TOBool(false, "false");


TObjectType::TObjectType(const std::string& typeName)
: name_(typeName)
{

}

TObjectType::~TObjectType()
{

}


TObjectType& TObjectType::get(const std::string& typeName)
{
    map<string,TObjectType*>::const_iterator it = typeRegistry_.find(typeName);
    if (it == typeRegistry_.end())
    {
        TObjectType* tot = new TObjectType(typeName);
        typeRegistry_[typeName] = tot;
        return *tot;
    }

    return *it->second;
}

bool TObjectType::exists(const std::string& typeName)
{
    map<string,TObjectType*>::const_iterator it = typeRegistry_.find(typeName);
    if (it == typeRegistry_.end())  return false;
    
    return true;
}

std::string TObjectType::name() const
{
    return name_;
}

bool TObjectType::operator==(const TObjectType& other)
{
    // if names are equal then it is the same type. but each name in the registry has a
    // unique TObjectType reference, so just compare that.
    return this == &other;
}

bool TObjectType::operator==(TObjectType& other)
{
    // if names are equal then it is the same type. but each name in the registry has a
    // unique TObjectType reference, so just compare that.
    return this == &other;
}

bool TObjectType::operator!=(const TObjectType& other)
{
    return !operator==(other);
}
