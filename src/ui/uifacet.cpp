#pragma once
/*
 *  uifacet.cpp
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

#include <iostream>
#include <sstream>

#include "uifacet.h"
#include "base/structureddata.h"


namespace aft
{
namespace ui
{
// This has to be in sync with the UIFacetCategory enum values
static const std::string uiFacectCategoryName[static_cast<int>(UIFacetCategory::SIZE)] = {
    "Unknown",
    "Border",
    "Color",
    "Decoration",
    "Dimension",
    "Input",
    "Output",
    "Other",
    "Theme"
};
    
static const std::string toplevel("UIFacet");

UIFacet::UIFacet()
    : data_(std::make_unique<base::StructuredData>(toplevel))
    , isMandatory_(false) {

}

UIFacet::UIFacet(const std::string& name, const std::string& value, UIFacetCategory category)
    : data_(std::make_unique<base::StructuredData>(toplevel))
    , isMandatory_(false) {

    if (!name.empty()) {
        data_->add(makeKeyName(category, name), value);
    }
}

UIFacet::UIFacet(const UIFacet& other)
    : data_(std::make_unique<base::StructuredData>(toplevel))
    , isMandatory_(other.isMandatory_) {
        
        base::StructuredData sd("");
        if (other.data_->get("", sd)) {
            data_->set("", sd);
        }
    }

UIFacet::~UIFacet() {

}

UIFacet& UIFacet::operator=(const UIFacet& other) {
    if (&other != this) {
        isMandatory_ = other.isMandatory_;
        base::StructuredData sd("");
        if (other.data_->get("", sd)) {
            data_->set("", sd);
        }
    }
    return *this;
}

UIFacetCategory
UIFacet::getCategory(const std::string& catName) {

    size_t numCategories = sizeof(uiFacectCategoryName) / sizeof(uiFacectCategoryName[0]);
    int retval = 0;
    for (size_t idx = 0; idx < numCategories; ++idx) {
        if (catName == uiFacectCategoryName[idx]) {
            retval = idx;
            break;
        }
    }
    return static_cast<UIFacetCategory>(retval);
}

const std::string&
UIFacet::getCategoryName(UIFacetCategory category) {

    size_t catIdx = static_cast<size_t>(category);
    if (catIdx >= static_cast<size_t>(UIFacetCategory::LAST)) {
        return uiFacectCategoryName[0];
    }
    return uiFacectCategoryName[catIdx];
}
    
bool UIFacet::getMandatory() const {
    return isMandatory_;
}

void UIFacet::setMandatory(bool isMandatory) {
    isMandatory_ = isMandatory;
}
    
bool UIFacet::get(const std::string& name, std::string& value,
                  UIFacetCategory category) const {
    return data_->get(makeKeyName(category,name), value);
}

bool UIFacet::get(const std::string& name, int& value,
                  UIFacetCategory category) const {
    return data_->get(makeKeyName(category,name), value);
}

bool UIFacet::get(const std::string& name, float& value,
                  UIFacetCategory category) const {
    std::string strValue;
    if (data_->get(makeKeyName(category, name), strValue)) {
        std::istringstream iss(strValue);
        iss >> value;
        return true;
    }
    return false;
}

std::vector<UIFacetCategory>
UIFacet::getCategories() const {
    std::vector<UIFacetCategory> categories;
    std::vector<std::string> catNames;
    if (data_->getMembers(catNames)) {
        for (const auto& name : catNames) {
            UIFacetCategory cat = getCategory(name);
            if (UIFacetCategory::Unknown != cat) {
                categories.push_back(cat);
            }
        }
    }
    return categories;
}

std::vector<std::string>
UIFacet::getNames(UIFacetCategory category) const {
    std::vector<std::string> names;
    base::StructuredData sd("");
    if (data_->get(getCategoryName(category), sd)) {
        sd.getMembers(names);
    }
    return names;
}

base::StructuredDataName
UIFacet::makeKeyName(UIFacetCategory category, const std::string& name) {
    std::ostringstream oss;
    oss << getCategoryName(category) << "." << name;
    return oss.str();
}

bool UIFacet::remove(const std::string& name, UIFacetCategory category) {
    return data_->remove(makeKeyName(category, name));
}

bool UIFacet::set(const std::string& name, const std::string& value,
                  UIFacetCategory category) {
    return data_->add(makeKeyName(category, name), value);
}
    
bool UIFacet::set(const std::string& name, int value,
                  UIFacetCategory category) {
    return data_->add(makeKeyName(category, name), value);
}
    
bool UIFacet::set(const std::string& name, float value,
                  UIFacetCategory category) {
    std::ostringstream oss;
    oss << value;
    return data_->add(makeKeyName(category, name), oss.str());
}

/** Used by subclasses of UIFacet to get named values. */
bool UIFacet::getFacetValue(const std::string& name, std::string& value) {
    return false;
}

/** Used by subclasses of UIFacet to set named values. */
void UIFacet::setFacetValue(const std::string& name, const std::string& value) {
    //
}

} // namespace ui
} // namespace aft
