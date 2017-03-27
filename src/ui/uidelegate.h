#pragma once
/*
 *  uidelegate.h
 *  libaft
 *
 *  Copyright © 2016-2017 Andy Warner. All rights reserved.
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
class Element;
class UIFacet;

/**
 *  Interface of basic Element interactions
 */
class UIDelegate
{
public:
    /** Add the element to the user interface, if possible. */
    virtual bool add(const Element& element) = 0;
    /** Set focus to element, if possible. */
    virtual bool focus(const Element& element) = 0;
    /** Hide user element from display */
    virtual bool hide(const Element& element) = 0;
    /** Get user input from element */
    virtual bool input(const Element& element, std::string& value) = 0;
    /** Output the element to the user interface */
    virtual bool output(const Element& element) = 0;
    /** Remove or hide element from the user interface, if possible. */
    virtual bool remove(const Element& element) = 0;
    /** Show user element */
    virtual bool show(const Element& element) = 0;
};

class BaseUIDelegate : public UIDelegate {
public:
    virtual bool add(const Element& element) override;
    virtual bool focus(const Element& element) override;
    virtual bool hide(const Element& element) override;
    virtual bool input(const Element& element, std::string& value) override;
    virtual bool output(const Element& element) override;
    virtual bool remove(const Element& element) override;
    virtual bool show(const Element& element) override;

protected:
    
};

} // namespace ui
} // namespace aft
