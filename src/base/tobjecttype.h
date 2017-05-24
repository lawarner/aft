#pragma once
/*
 *  tobjecttype.h
 *  libaft
 *
 *   Copyright © 2016-2017 Andy Warner. All rights reserved.
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

#include <map>
#include <string>

#define TOTYPE_BASE "Base"

namespace aft
{
namespace base
{

/**
 * Class to track specific types of TObject subclasses.
 *
 * This class maintains a unique registry of named TObjectTypes.
 */
class TObjectType
{
private:
    TObjectType(const std::string& typeName);
    ~TObjectType();
    
public:
    /** Get the TObject type by name.
     *  A new type will be created and registered if it did not yet exist.
     */
    static TObjectType& get(const std::string& typeName);

    /** Check if TObject type exists */
    static bool exists(const std::string& typeName);

    /** Get the name of this TObject type */
    std::string name() const;
    
    /** Test if this TObject type is equal to another. */
    bool operator==(const TObjectType& other) const;
    bool operator==(TObjectType& other);

    /** Test if this TObject type is not equal to another. */
    bool operator!=(const TObjectType& other) const;

public:
    /** Type name for base TObjects */
    static const std::string NameBase;
    /** Type name for TOBasicTypes */
    static const std::string NameBasicType;
    /** Type name for command TObjects */
    static const std::string NameCommand;
    /** Type name for TestCase TObjects */
    static const std::string NameTestCase;
    /** Type name for TestSuite TObjects */
    static const std::string NameTestSuite;
    /** Type name for UICommand TObjects */
    static const std::string NameUiCommand;
    //TODO more tobject type names
    
    /** Cached common reference for TypeBase */
    static const TObjectType& TypeBase;
    /** Cached common reference for TypeBasicType */
    static const TObjectType& TypeBasicType;
    /** Cached common reference for TypeCommand */
    static const TObjectType& TypeCommand;
    /** Cached common reference for TypeTestCase */
    static const TObjectType& TypeTestCase;
    /** Cached common reference for TypeTestSuite */
    static const TObjectType& TypeTestSuite;
    /** Cached common reference for TypeUiCommand */
    static const TObjectType& TypeUiCommand;

private:
    std::string name_;
    static std::map<std::string, TObjectType*> typeRegistry_;
};
    
} // namespace base
} // namespace aft
