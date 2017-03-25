#pragma once
/*
 *   Copyright © 2015-2017 Andy Warner
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

    /** Construct StructureDataName by combining path and name strings
     *
     *  @param path string representing path
     *  @param name the object's name
     */
    StructuredDataName(const std::string& path, const std::string& name);

    //TODO StructuredDataName(const StructuredDataName&, const std::string& name)

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

    /** Return the parent path as a StructureDataName */
    StructuredDataName getParent() const;

    /** Get the path name as a string
     *
     *  @param fullName if true then the whole path will be returned, otherwise the
     *                  last component of the path is returned.
     */
    const std::string& getPath(bool fullPath = true) const;
    
    /** Compare this SD name to another */
    bool operator==(const StructuredDataName& other);
    
public:
    /** Path separator character */
    static const std::string SEPARATOR;

private:
    void pathFromName();

private:
    std::string fullPath_;
    std::string name_;
    std::vector<std::string> path_;
};
    
} // namespace base
} // namespace aft
