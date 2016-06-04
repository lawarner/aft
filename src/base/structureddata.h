#pragma once
/*
 *   Copyright 2015, 2016 Andy Warner
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
#include "base/structureddataname.h"


namespace aft
{
namespace base
{
// Forward reference
class Blob;


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
    static StructuredDataDelegate* getDelegate(StructuredData& sd);

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

    /** Construct a structured data object.
     *
     *  @param name Name of this object
     *  @param blob If blob contains a string then it is to construct the StructuredData instance.
     *              If blob contains a data pointer, then it is cast directly as the internal 
     *              structure (json, etc.).
     *  @param delegate the object that handles the actual implementation. If the
     *             delegate is not provided, then the default json delegate is used.
     */
    StructuredData(const StructuredDataName& name, const Blob& blob,
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

    /** Check if the given element name is an array. */
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
