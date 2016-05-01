#pragma once
//
//  tobjecttype.h
//  libaft
//
//  Copyright © 2016 Andy Warner. All rights reserved.
//

#include <map>
#include <string>

#define TOTYPE_BASE "Base"

namespace aft
{
namespace base
{

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
    bool operator==(const TObjectType& other);

    /** Test if this TObject type is not equal to another. */
    bool operator!=(const TObjectType& other);

public:
    /** Type name for base TObjects */
    static const std::string NameBase;
    /** Type name for command TObjects */
    static const std::string NameCommand;
    /** Type name for TestCase TObjects */
    static const std::string NameTestCase;
    /** Type name for TestSuite TObjects */
    static const std::string NameTestSuite;
    //TODO more command type names

private:
    std::string name_;
    static std::map<std::string, TObjectType*> typeRegistry_;
};
    
} // namespace base
} // namespace aft
