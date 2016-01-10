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
#include <vector>
#include "base/serialize.h"


namespace aft
{
namespace base
{
// Forward reference
class Blob;
class StructuredData;


/**
 *  Container for a multipart, hierarchical name
 *
 *  Arrays are supported and indexed numerically.  To get element 2 from an array
 *  use syntax like the following example: "myobj.myarray.2"
 *  When adding an element to the array use, for example, "myobj.myarray."
 *  A more complex example:  "myobj.arr.2.info.0" will get element 2 of "myobj.arr"
 *  which is an object containing an array element named "info", where we address the
 *  first element.
 *
 *  Note that in the future bracket notation may be added.
 */
class StructuredDataName
{
public:
    /** Construct StructuredDataName based on string.
     *
     *  @param name this object's name. If it contains SEPARATOR then the name
     *              is split into components separated by SEPARATOR and the simple
     *              name is set to the last component.
     */
    StructuredDataName(const std::string& name = std::string());
    /** Construct StructuredDataName based on char string.
     *
     *  @param name this object's name. If it contains SEPARATOR then the name
     *              is split into components separated by SEPARATOR and the simple
     *              name is set to the last component.
     */
    StructuredDataName(const char* name);

    /** Construct StructuredDataName from a vector of name components.
     *
     *  @param namePath the string components that make this object's name. The full name
     *                  is formed by concatenating the components in namePath, separated
     *                  by SEPARATOR.  The simple name is set to the last component.
     */
    StructuredDataName(const std::vector<std::string>& namePath);

    /** Destruct a StructuredDataName */
    virtual ~StructuredDataName();

    /** @return true if name is empty, otherwise false. */
    bool empty() const;

    /** Get the name as a vector of components.
     *  @return string vector of name components
     */
    const std::vector<std::string>& getComponents() const;

    /** Get the name as a string
     *
     *  @param fullName If true then the fully qualified name is returned, otherwise the
     *                  simple name is returned.
     */
    std::string getName(bool fullName = false) const;

    /** Get the path name as a string */
    const std::string& getPath(bool fullPath = true) const;

    /** Compare this SD name to another */
    bool operator==(const StructuredDataName& other);

public:
    /** Path separator character */
    static const std::string SEPARATOR;

private:
    std::string fullPath_;
    std::string name_;
    std::vector<std::string> path_;
};

/**
 *  Structured Data delegate interface
 */
class StructuredDataDelegate
{
public:
    /** The type of structured data member. */
    enum Type
    {
        NOTFOUND, INT, STRING, STRUCTUREDDATA, ARRAY
    };

    /** Destruct a StructuredDataDelegate */
    virtual ~StructuredDataDelegate() { }

    /** Add a named int value to the structured data. */
    virtual bool add(const StructuredDataName& name, int intValue) = 0;
    /** Add a named string value to the structured data. */
    virtual bool add(const StructuredDataName& name, const std::string& value) = 0;
    /** Add a named sub-structured data to the structured data. */
    virtual bool add(const StructuredDataName& name, const StructuredData& value) = 0;
    virtual bool addArray(const StructuredDataName& name) = 0;

    virtual bool get(const StructuredDataName& name, int& intValue) const = 0;
    virtual bool get(const StructuredDataName& name, std::string& value) const = 0;
    virtual bool get(const StructuredDataName& name, StructuredData& value) const = 0;

    /** Get all elements of an array. */
    virtual bool getArray(const StructuredDataName& name, 
                          std::vector<std::string>& values) const = 0;

    /** Create structured data from a string */
    virtual bool parse(const StructuredDataName& name, const std::string& strData) = 0;
    /** Get a string representation of the structured data */
    virtual bool unparse(const StructuredDataName& name, std::string& strData) = 0;

    /** Remove a structured data member.
     *  @param name hierarchical name of member
     *  @return true if member was removed, otherwise false.
     */
    virtual bool remove(const StructuredDataName& name) = 0;

    /** Get the type of structured data member.
     *  @param name hierarchical name of member
     *  @return the type of the named member
     */
    virtual Type type(const StructuredDataName& name) = 0;

    /** Get pointer to a StructuredData delegate.
     *  @param sd reference to StructuredData
     *  @return the StructuredData's delegate
     */
    StructuredDataDelegate* getDelegate(StructuredData& sd) const;

    /** Set the name of a StructuredData
     *  @param sd reference to StructuredData
     *  @param name Name to assign to sd
     */
    void setSDName(StructuredData& sd, const StructuredDataName& name) const;
};

/**
 *  StructuredData with optional delegate
 *
 *  Note that arrays are always indexed numerically i.e., "one.twos[0].three".
 *  That is because "myobj.myarray[myelement]" is equivalent to "myobj.myarray.myelement".
 */
class StructuredData : public SerializeContract
{
public:
    /** Construct a structured data object.
     *
     *  @param name Name of this object
     *  @param fromString provides the string to construct the StructuredData instance.
     *  @param delegate the object that handles the actual implementation. If the 
     *             delegate is not provided, then the default json delegate is used.
     */
    StructuredData(const StructuredDataName& name,
                   const std::string& fromString = std::string(),
                   StructuredDataDelegate* delegate = 0);

    /** Destruct a structured data object. */
    virtual ~StructuredData();

    // An idea is to have the top level a std::map<string,Blob> so branches can be
    // heterogeneous.  Will probably just have an opaque json container.
    
    /** Add a named blob to the structured data.
     *  @param name Name of the object.  If adding an element to an array, then name
     *              specifies the array name plus a trailing dot (".").
     *  @return true if the blob was added, otherwise false.
     */
    bool add(const StructuredDataName& name, const Blob& blob);

    /** Add a named structured data as a member of this structured data.
     *
     *  Note that data must have the same delegate as 'this' */
    bool add(const StructuredDataName& name, const StructuredData& data);
    /** Add a named int value to the structured data. */
    bool add(const StructuredDataName& name, int intValue);
    /** Add a named string value to the structured data. */
    bool add(const StructuredDataName& name, const std::string& value);

    /** Add a named (empty) array to the structured data.
     *  @param name Name of the new array
     *  @return true if the array cannot be created, otherwise false.
     *          Delegates that do not implement arrays return false.
     *          Also, if a non-array element exists with the same name then false
     *          is returned.
     */
    bool addArray(const StructuredDataName& name);

    /** Get a named blob from the structured data. */
    bool get(const StructuredDataName& name, Blob& blob) const;
    /** Get a named sub-structured data from the structured data. */
    bool get(const StructuredDataName& name, StructuredData& data) const;
    /** Get a named int from the structured data. */
    bool get(const StructuredDataName& name, int& intValue) const;
    /** Get a named string from the structured data. */
    bool get(const StructuredDataName& name, std::string& value) const;
    /** Alternative form for getting string elements. */
    std::string get(const StructuredDataName& name,
                    const std::string& defValue = std::string()) const;

    /** Get all elements of an array. */
    bool getArray(const StructuredDataName& name, 
                  std::vector<std::string>& values) const;

    bool isArray(const StructuredDataName& name) const;

    /** Remove the element and all subelements of name.
     *  @param name name to remove.  Any children are lost.
     *  @return true if the named data could be remove, otherwise false.
     */
    bool remove(const StructuredDataName& name);

    /** Set the value of an existing name.
     *
     *  Similar to add, but only sets value if name already exists in structure.
     */
    bool set(const StructuredDataName& name, const Blob& blob);
    bool set(const StructuredDataName& name, const StructuredData& data);
    bool set(const StructuredDataName& name, int intValue);
    bool set(const StructuredDataName& name, const std::string& value);

    //TODO: search, mapping and visitors (copy/assignment: full vs. per member)
    //TODO: freeze(Gas,Water,Ice), inheritance

    // Implement SerializeContract Interface
    virtual bool serialize(Blob& blob);
    virtual bool deserialize(const Blob& blob);

protected:
    StructuredDataName name_;
    StructuredDataDelegate* delegate_;

    friend class StructuredDataDelegate;
};

} // namespace base
} // namespace aft
