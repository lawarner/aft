#pragma once
//
//  element.h
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

#include <string>

namespace aft
{
namespace ui
{
// Forward reference
class UIDelegate;

/**
 * Represent an element or group of elements in a UI
 *
 * Some possible Element types: Number, Group, Custom, CustomGroup, ...
 */
class Element
{
public:
    Element(const std::string& name, UIDelegate* delegate = 0);
    virtual ~Element();

    // work methods
    /** True if value has already been read/updated. */
    virtual bool hasValue() const;
    /** Remove element from the UI */
    virtual void hide();
    /** Get a value from user input and store into value_ */
    virtual bool input();
    /** Refresh (redraw) the element in the UI */
    virtual void refresh();
    /** Layout and draw the element within the UI */
    virtual void show(bool forceShow = false);
    /** Validate the value of this element.
     *  This base class implementation always returns true.
     */
    virtual bool validate();

    // getters and setters
    const std::string& getName() const;

    /** Get the value for this element.
     *  @param refreshValue if true then prompt the UI for a value, otherwise use the current value.
     *  @return The value of this element.
     */
    virtual std::string getValue(bool refreshValue);
    /** Get the current value for this element. */
    virtual std::string getValue() const;
    /** Set the value for this element. */
    virtual void setValue(const std::string& value);
    virtual std::string getDefault() const;
    virtual void setDefault(const std::string& value);
    virtual std::string getPrompt() const;
    virtual void setPrompt(const std::string& prompt);
    virtual bool getEnabled() const;
    virtual void setEnabled(bool isEnabled);
    virtual bool getVisible() const;
    virtual void setVisible(bool isVisible);

private:
    const std::string name_;

protected:
    UIDelegate* delegate_;

    // current value for display
    std::string value_;

    std::string defaultValue_;
    std::string prompt_;
    bool isEnabled_;
    bool isVisible_;
    // entity, decor, attributes, Result
};

} // namespace ui
} // namespace aft