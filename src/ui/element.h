#pragma once
/*
 *  element.h
 *  libaft
 *
 *   Copyright © 2016-2017 Andy Warner
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

#include "ui/elementhandle.h"
#include "ui/uifacet.h"
#include <atomic>
#include <string>
#include <unordered_map>

namespace aft {
namespace ui {

/**
 * Represent an element or group of elements in a UI
 *
 * Some possible Element types: Number, Group, Custom, CustomGroup, ...
 */
class Element {
public:
    using ElementId = uint32_t;

public:
    /** Construct a named element. */
    Element(const std::string& name);
    /** Construct a copy of another element. */
    Element(const Element& other);
    /** Set this element to the value of another element. */
    Element& operator=(const Element& other);
    /** Destruct an element. */
    virtual ~Element() = default;

    /** Compare this element to another element.
     *  Tow elements are equal only if they have the same ID.
     *  @param  other Other element
     *  @return true if this element is equal to other element, otherwise false.
     */
    bool operator==(const Element& other);

    // work methods
    virtual void apply(const UIFacet& facet);
    virtual void block();
    virtual bool getFacet(std::string& value, const std::string& name,
                          UIFacetCategory category = UIFacetCategory::Other) const;
    /** True if value has already been read/updated. */
    virtual bool hasValue() const;
    virtual bool isBlocked() const;
    virtual bool remove(const std::string& name, UIFacetCategory category = UIFacetCategory::Other);
    virtual void remove(const UIFacet& facet);
    virtual void unblock();
    /** Validate the value of this element.
     *  This base class implementation always returns true.
     */
    virtual bool validate();

    // getters and setters
    ElementId getId() const;
    ElementHandle getHandle() const;
    const std::string& getName() const;
    /** Get the value stored for this element. */
    const std::string& getValue() const;
    /** Set the value for this element. */
    virtual void setValue(const std::string& value);
    /** Get the default value for this element */
    virtual const std::string& getDefault() const;
    /** Set the default value for this element */
    virtual void setDefault(const std::string& value);
    virtual const std::string& getPrompt() const;
    virtual void setPrompt(const std::string& prompt);
    virtual bool getEnabled() const;
    virtual void setEnabled(bool isEnabled);
    virtual bool getVisible() const;
    virtual void setVisible(bool isVisible);

private:
    void clone();
private:
    std::string name_;
    bool  cow_;
    std::atomic<bool> isBlocked_;
    
    // unique id
    ElementId id_;
    static ElementId nextId_;

protected:
    // current value for display
    std::string value_;

    std::string defaultValue_;
    std::string prompt_;
    bool isEnabled_;
    bool isValueSet_;
    bool isVisible_;

    UIFacet facet_;
    // entity, decor, attributes, Result
};

} // namespace ui
} // namespace aft
