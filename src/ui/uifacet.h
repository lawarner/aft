#pragma once
/*
 *  uifacet.h
 *  libaft
 *
 *   Copyright © 2017 Andy Warner
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


namespace aft
{
namespace base
{
// Forward reference
class StructuredData;
}
    
namespace ui
{

/**
 *  Category of UIFacet. Note that facets are organized by category and name/value.
 */
enum class UIFacetCategory {
    START,
    Unknown = START,
    Border,
    Color,
    Decoration,
    Dimension,
    Input,
    Output,
    Other,
    Theme,
    // add new categories above this line
    LAST,
    SIZE = LAST - START
};


/**
 *  UIFacet's modify the look and feel, behavior, size, position or other attributes of UI elements.
 *  Most facets are optional but can be made mandatory.
 */
class UIFacet {
public:
    /** Default constructor for use by STL containers. */
    UIFacet();
    /** Construct a UIFace with name, value and category.
     *  @param name Name of this facet, which should be unique within each category.
     *  @param value Initial string value of facet. This may be parsed and converted by subclasses.
     *  @param category Category of facet. Set to UIFacetCategory::Other if not specified.
     */
    UIFacet(const std::string& name, const std::string& value, UIFacetCategory category = UIFacetCategory::Other);
    UIFacet(const UIFacet& other);
    virtual ~UIFacet();
    
    UIFacet& operator=(const UIFacet& other);

    bool isInitialized() const;
    UIFacetCategory getCategory() const;
    const std::string& getCategoryName() const;
    bool getMandatory() const;
    /** Can only be used once, after default construction. */
    bool setCategory(UIFacetCategory category);
    void setMandatory(bool isMandatory);

    virtual bool get(const std::string& name, std::string& value) const;
    virtual bool get(const std::string& name, int& value) const;
    virtual bool get(const std::string& name, float& value) const;
    virtual bool set(const std::string& name, const std::string& value);
    virtual bool set(const std::string& name, int value);
    virtual bool set(const std::string& name, float value);

public:
    static const std::string& getCategoryName(UIFacetCategory category);

protected:
    /** Used by subclasses of UIFacet to get named values. */
    bool getFacetValue(const std::string& name, std::string& value);
    /** Used by subclasses of UIFacet to set named values. */
    void setFacetValue(const std::string& name, const std::string& value);

private:
    std::unique_ptr<base::StructuredData> data_;
    std::unique_ptr<base::StructuredData> categoryData_;
    UIFacetCategory category_;
    bool isMandatory_;
    std::string categoryPrefix_;
};
    
} // namespace ui
} // namespace aft
