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
    , categoryData_(std::make_unique<base::StructuredData>(""))
    , category_(UIFacetCategory::Unknown)
    , isMandatory_(false) {

}

UIFacet::UIFacet(const UIFacet& other)
    : data_(std::make_unique<base::StructuredData>(toplevel))
    , categoryData_(std::make_unique<base::StructuredData>(""))
    , category_(other.category_)
    , isMandatory_(other.isMandatory_) {
    
    base::StructuredData sd("");
    if (other.data_->get("", sd)) {
        data_->set("", sd);
    }
    if (other.categoryData_->get("", sd)) {
        categoryData_->set("", sd);
    }
}

UIFacet::UIFacet(const std::string& name, const std::string& value, UIFacetCategory category)
    : data_(std::make_unique<base::StructuredData>(toplevel))
    , categoryData_(std::make_unique<base::StructuredData>(getCategoryName(category)))
    , category_(category)
    , isMandatory_(false) {

    if (!name.empty()) {
        categoryData_->add(name, value);
    }
    data_->add(getCategoryName(), *categoryData_);
}

UIFacet::~UIFacet() {

}

    
UIFacet& UIFacet::operator=(const UIFacet& other) {
    if (&other != this) {
        category_ = other.category_;
        isMandatory_ = other.isMandatory_;
        base::StructuredData sd("");
        if (other.data_->get("", sd)) {
            data_->set("", sd);
        }
        if (other.categoryData_->get("", sd)) {
            categoryData_->set("", sd);
        }
    }
    return *this;
}
    
bool UIFacet::isInitialized() const {
    return UIFacetCategory::Unknown != category_;
}

bool UIFacet::setCategory(UIFacetCategory category) {
    if (isInitialized()) {
        return false;
    }
    category_ = category;
    categoryData_->setName(getCategoryName(category));
    return true;
}


UIFacetCategory UIFacet::getCategory() const {
    return category_;
}

const std::string& UIFacet::getCategoryName() const {
    return getCategoryName(category_);
}

const std::string& UIFacet::getCategoryName(UIFacetCategory category) {
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
    
bool UIFacet::get(const std::string& name, std::string& value) const {
    return categoryData_->get(name, value);
}

bool UIFacet::get(const std::string& name, int& value) const {
    return categoryData_->get(name, value);
}

bool UIFacet::get(const std::string& name, float& value) const {
    std::string strValue;
    if (categoryData_->get(name, strValue)) {
        std::istringstream iss(strValue);
        iss >> value;
        return true;
    }
    return false;
}

bool UIFacet::set(const std::string& name, const std::string& value) {
    if (isInitialized()) {
        return categoryData_->add(name, value);
    }
    return false;
}
    
bool UIFacet::set(const std::string& name, int value) {
    if (isInitialized()) {
        return categoryData_->add(name, value);
    }
    return false;
}
    
bool UIFacet::set(const std::string& name, float value) {
    if (isInitialized()) {
        std::ostringstream oss;
        oss << value;
        return categoryData_->add(name, oss.str());
    }
    return false;
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
