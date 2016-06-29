//
//  element.cpp
//  libaft
/*
 *   Copyright © 2016 Andy Warner
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
#include "uidelegate.h"
#include "core/logger.h"
using namespace aft::core;


namespace aft
{
namespace ui
{
class UIDefaultDelegate : public UIDelegate
{
public:
    UIDefaultDelegate() { }
    virtual ~UIDefaultDelegate() { }

    /** Add the element to the user interface, if possible. */
    virtual bool add(const Element& element)
    {
        aftlog << "Element " << element.getName() << " added for UI" << std::endl;
        return true;
    }

    /** Get user input from element */
    virtual bool input(const Element& element, std::string& value)
    {
        value = element.getDefault();
        aftlog << "Element " << element.getName() << " returns default: " << value << std::endl;
        return true;
    }
    /** Output the element to the user interface.
     *
     *  The various output formats for an element is:
     *  Fully specified:   <element_name>: <prompt> [<default_value>]?
     *  Default missing:   <element_name>: <prompt>?
     *  Prompt missing:    <element_name> [<default_value>]?
     *  No Prompt,Default: <element_name>?
     */
    virtual bool output(const Element& element)
    {
        aftlog << "Element " << element.getName();
        if (!element.getPrompt().empty()) aftlog << ": " << element.getPrompt();
        std::string defValue = element.getDefault().empty() ? "? " : " [" + element.getDefault() + "]? ";
        aftlog << defValue << std::endl;

        return true;
    }
    virtual bool remove(const Element& element)
    {
        aftlog << "Element " << element.getName() << " removed for UI" << std::endl;
        return true;
    }
};

}
}

using namespace aft::ui;


Element::Element(const std::string& name, UIDelegate* delegate)
: name_(name)
, delegate_(delegate ? delegate : new UIDefaultDelegate)
, isVisible_(false)
, isEnabled_(false)
{
    
}

Element::~Element()
{

}

bool Element::hasValue() const
{
    return isEnabled_ && !value_.empty();
}

void Element::hide()
{
    setVisible(false);
    delegate_->remove(*this);
}

bool Element::input()
{
    return delegate_->input(*this, value_);
}

void Element::refresh()
{
    hide();
    show();
}

void Element::show(bool forceShow)
{
    if (!isVisible_ || forceShow)
    {
        setVisible(delegate_->output(*this));
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

std::string Element::getValue() const
{
    return value_;
}

std::string Element::getValue(bool refreshValue)
{
    if (refreshValue)
    {
        delegate_->input(*this, value_);
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
}
