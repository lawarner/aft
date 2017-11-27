/*
 *
 *   element.cpp
 *   libaft
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

#include "element.h"

namespace aft {
namespace ui {

Element::ElementId Element::nextId_ = 1;

Element::Element(const std::string& name)
    : name_(name)
    , cow_(false)
    , isBlocked_(false)
    , id_(nextId_++)
    , isEnabled_(false)
    , isValueSet_(false)
    , isVisible_(false)  {
    
}

Element::Element(const Element& other)
    : name_        (other.name_)
    , cow_         (true)
    , isBlocked_   (other.isBlocked_.load())
    , id_          (other.id_)
    , value_       (other.value_)
    , defaultValue_(other.defaultValue_)
    , prompt_      (other.prompt_)
    , isEnabled_   (other.isEnabled_)
    , isValueSet_  (other.isValueSet_)
    , isVisible_   (other.isVisible_)
    , facet_       (other.facet_) {

}

Element&
Element::operator=(const Element& other) {
    if (this != &other) {
        name_         = other.name_;
        cow_          = true;
        isBlocked_    = other.isBlocked_.load();
        id_           = other.id_;
        value_        = other.value_;
        defaultValue_ = other.defaultValue_;
        prompt_       = other.prompt_;
        isEnabled_    = other.isEnabled_;
        isValueSet_   = other.isValueSet_;
        isVisible_    = other.isVisible_;
        facet_        = other.facet_;
    }
    return *this;
}

bool Element::operator==(const Element& other) {
    return id_ == other.id_;
}
    
void Element::apply(const UIFacet& facet) {
    auto cats = facet.getCategories();
    for (auto cat : cats) {
        auto names = facet.getNames(cat);
        for (const auto& name : names) {
            std::string value;
            if (facet.get(name, value, cat)) {
                //TODO check if already exists
                if (facet_.set(name, value, cat)) {
                    clone();
                }
            }
        }
    }
}

void Element::block() {
    isBlocked_ = true;
}

bool Element::getFacet(std::string& value, const std::string& name,
                       UIFacetCategory category) const {
    return facet_.get(name, value, category);
}

bool Element::hasValue() const {
    return isValueSet_;
}

bool Element::isBlocked() const {
    return isBlocked_;
}
    
bool Element::remove(const std::string& name, UIFacetCategory category) {
    return facet_.remove(name, category);
}
    
void Element::remove(const UIFacet& facet) {
    auto cats = facet.getCategories();
    for (auto cat : cats) {
        auto names = facet.getNames(cat);
        for (const auto& name : names) {
            facet_.remove(name, cat);
        }
    }
}

void Element::unblock() {
    isBlocked_ = false;
}
    

bool Element::validate() {
    return true;
}

// getters and setters

Element::ElementId
Element::getId() const {
    return id_;
}

ElementHandle
Element::getHandle() const {
    return ElementHandle(this);
}

const std::string&
Element::getName() const {
    return name_;
}

const std::string&
Element::getValue() const {
    return value_;
}

void Element::setValue(const std::string& value) {
    value_ = value;
    isValueSet_ = true;
    clone();
}

const std::string&
Element::getDefault() const {
    return defaultValue_;
}

void Element::setDefault(const std::string& value) {
    defaultValue_ = value;
    clone();
}

const std::string&
Element::getPrompt() const {
    return prompt_;
}

void Element::setPrompt(const std::string& prompt) {
    prompt_ = prompt;
    clone();
}

bool Element::getEnabled() const {
    return isEnabled_;
}

void Element::setEnabled(bool isEnabled) {
    isEnabled_ = isEnabled;
    clone();
}

bool Element::getVisible() const {
    return isVisible_;
}

void Element::setVisible(bool isVisible) {
    isVisible_ = isVisible;
    if (!isVisible_) {
        isEnabled_ = false;
    }
    clone();
}

void Element::clone() {
    if (cow_) {
        id_ = nextId_++;
        cow_ = false;
    }
}

} // namespace ui
} // namespace aft
