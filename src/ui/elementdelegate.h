#pragma once
/*
 *  elementdelegate.h
 *  libaft
 *
 *  Copyright © 2017 Andy Warner. All rights reserved.
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
#include "uifacet.h"

namespace aft {
namespace ui {
// Forward reference
class Element;
        
/**
 *  Interface of basic Element interactions
 */
class ElementDelegate {
public:
    /** Get the focus state of the element. */
    virtual bool getFocus(Element* element) const = 0;
    /** Change focus state within element. */
    virtual bool setFocus(Element* element, bool hasFocus = true) = 0;
    /** Get facet by name and category */
    virtual bool getFacet(Element* element, const std::string& name, UIFacetCategory category,
                          UIFacet& facet) const = 0;
    /** Set a facet on an element */
    virtual bool setFacet(Element* element, const UIFacet& facet) = 0;

    /** Get user input from element */
    virtual bool input(Element* element, std::string& value) = 0;
    /** Output the element to the user interface
     *  TODO add bool to just output value
     */
    virtual bool output(Element* element) = 0;
};

class BaseElementDelegate : public ElementDelegate {
public:
    BaseElementDelegate();
    virtual ~BaseElementDelegate() = default;

    virtual bool getFocus(Element* element) const override;
    virtual bool setFocus(Element* element, bool hasFocus = true) override;
    virtual bool getFacet(Element* element, const std::string& name, UIFacetCategory category,
                          UIFacet& facet) const override;
    virtual bool setFacet(Element* element, const UIFacet& facet) override;

    virtual bool input(Element* element, std::string& value) override;
    virtual bool output(Element* element) override;
};

}
}
