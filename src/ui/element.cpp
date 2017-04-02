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
#include "elementdelegate.h"
#include "uidelegate.h"
#include "core/logger.h"
using namespace aft::core;


namespace aft {
namespace ui {


Element::Element(const std::string& name, ElementDelegate* delegate)
: name_(name)
, delegate_(delegate ? delegate : new BaseElementDelegate)
, isEnabled_(false)
, isVisible_(false)
{
    
}

Element::Element(const Element& other)
    : name_(other.name_)
    , delegate_(other.delegate_)
    , value_(other.value_)
    , defaultValue_(other.defaultValue_)
    , prompt_(other.prompt_)
    , isEnabled_(other.isEnabled_)
    , isVisible_(other.isVisible_) {
    
}

Element& Element::operator=(const Element& other) {
    if (this != &other) {
        name_ = other.name_;
        delegate_ = other.delegate_;
        value_ = other.value_;
        defaultValue_ = other.defaultValue_;
        prompt_ = other.prompt_;
        isEnabled_ = other.isEnabled_;
        isVisible_ = other.isVisible_;
    }
    return *this;
}

Element::~Element()
{

}

void Element::apply(const UIFacet& facet) {
    
}

bool Element::hasValue() const
{
    return isEnabled_ && !value_.empty();
}

void Element::hide()
{
    setVisible(false);
}

bool Element::input()
{
    if (!isEnabled_) {
        return false;
    }
    return delegate_->input(this, value_);
}

void Element::refresh()
{
    hide();
    show();
}

void Element::show(bool forceShow) {
    if (!isVisible_ || forceShow) {
        setVisible(delegate_->output(this));
    }
}

bool Element::validate()
{
    return true;
}

// getters and setters
const std::string&
Element::getName() const
{
    return name_;
}

const std::string&
Element::getValue() const
{
    return value_;
}

std::string Element::getValue(bool refreshValue)
{
    if (refreshValue)
    {
        delegate_->input(this, value_);
    }

    return value_;
}

void Element::setValue(const std::string& value)
{
    value_ = value;
}

std::string Element::getDefault() const
{
    return defaultValue_;
}

void Element::setDefault(const std::string& value)
{
    defaultValue_ = value;
}

std::string Element::getPrompt() const
{
    return prompt_;
}

void Element::setPrompt(const std::string& prompt)
{
    prompt_ = prompt;
}

bool Element::getEnabled() const
{
    return isEnabled_;
}

void Element::setEnabled(bool isEnabled)
{
    isEnabled_ = isEnabled;
}

bool Element::getVisible() const
{
    return isVisible_;
}

void Element::setVisible(bool isVisible)
{
    isVisible_ = isVisible;
    if (!isVisible_) {
        isEnabled_ = false;
    }
}

} // namespace ui
} // namespace aft
