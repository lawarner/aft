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


class StructuredDataDelegate
{
public:
    enum Type
    {
        NOTFOUND, INT, STRING, STRUCTUREDDATA, ARRAY
    };

    virtual ~StructuredDataDelegate() { }

    virtual bool add(const std::string& name, int intValue) = 0;
    virtual bool add(const std::string& name, const std::string& value) = 0;
    virtual bool add(const std::string& name, const StructuredData& value) = 0;
    virtual bool get(const std::string& name, int& intValue) const = 0;
    virtual bool get(const std::string& name, std::string& value) const = 0;
    virtual bool get(const std::string& name, StructuredData& value) const = 0;

    virtual bool parse(const std::string& name, const std::string& strData) = 0;
    virtual bool unparse(const std::string& name, std::string& strData) = 0;

    virtual bool remove(const std::string& name) = 0;

    virtual Type type(const std::string& name) = 0;

    StructuredDataDelegate* getDelegate(StructuredData& sd) const;
};


/**
 *  Container for a multipart, hierarchical name
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
    /** Construct StructuredDataName from a vector of name components.
     *
     *  @param namePath the string components that make this object's name. The full name
     *                  is formed by concatenating the components in namePath, separated
     *                  by SEPARATOR.  The simple name is set to the last component.
     */
    StructuredDataName(const std::vector<std::string>& namePath);

    /** Destruct a StructuredDataName */
    virtual ~StructuredDataName();

    /** Get the name as a string
     *
     *  @param fullName If true then the fully qualified name is returned, otherwise the
     *                  simple name is returned.
     */
    std::string getName(bool fullName = false) const;

    /** Get the name as a vector of components.
     *  @return string vector of name components
     */
    const std::vector<std::string>& getPath() const;

    /** Compare this SD name to another */
    bool operator==(const StructuredDataName& other);

public:
    /** Path separator character */
    static const std::string SEPARATOR;

private:
    std::string fullName_;
    std::string name_;
    std::vector<std::string> path_;
};

/**
 *  StructuredData with optional delegate
 *
 *  TODO arrays. involves StructuredDataName also, i.e., "one.twos[my].three"
 */
class StructuredData : public SerializeContract
{
public:
    /** Construct a structured data object.
     *
     *  @param name Name of this object
     *  @param fromString provides the string to construct StructuredData instance.
     *  @param delegate the object that handles the actual implementation. If not provided,
     *                  then the default json delegate is used as the delegate.
     */
    StructuredData(const StructuredDataName& name,
                   const std::string& fromString = std::string(),
                   StructuredDataDelegate* delegate = 0);
    virtual ~StructuredData();

    // Need add, get, set methods for children.
    // An idea is to have the top level a std::map<string,Blob> so branches can be
    // heterogeneous.  Will probably just have an opaque json container.
    
    bool add(const StructuredDataName& name, const Blob& blob);
    bool add(const StructuredDataName& name, const StructuredData& data);
    bool add(const StructuredDataName& name, int intValue);
    bool add(const StructuredDataName& name, const std::string& value);

    bool get(const StructuredDataName& name, Blob& blob) const;
    bool get(const StructuredDataName& name, StructuredData& data) const;
    bool get(const StructuredDataName& name, int& intValue) const;
    bool get(const StructuredDataName& name, std::string& value) const;
    std::string get(const StructuredDataName& name,
                    const std::string& defValue = std::string()) const;

    bool remove(const StructuredDataName& name);

    /** Similar to add, but only sets value if name already exists in structure. */
    bool set(const StructuredDataName& name, const Blob& blob);
    bool set(const StructuredDataName& name, const StructuredData& data);
    bool set(const StructuredDataName& name, int intValue);
    bool set(const StructuredDataName& name, const std::string& value);

    //TODO: search, mapping and visitors (copy/assignment: full vs. per member)
    //TODO: lock, inheritance

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
