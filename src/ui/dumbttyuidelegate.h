#pragma once
/*
 *  dumbttyuidelegate.h
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

#include "ui/uidelegate.h"


namespace aft {
namespace ui {

// Forward reference
class DumbTtyElementDelegate;
class Element;
class UIFacet;

/**
 *  Implements the ui delegate interface for a very basic, dumb terminal.
 */
class DumbTtyUIDelegate : public BaseUIDelegate {
public:
    DumbTtyUIDelegate();
    virtual ~DumbTtyUIDelegate() = default;
public:
    void addInputValue(const Element& element, const std::string& value);

    virtual bool add(const Element& element) override;
    virtual const Element* get(const std::string& name) override;
    virtual bool hide(const Element& element) override;
    virtual bool input(const Element& element, std::string& value) override;
    virtual bool output(const Element& element) const override;
    virtual bool remove(const Element& element) override;
    virtual bool show(const Element& element, bool showValue = false) override;

private:
    /** All elements within this UI use this ElementDelegate */
    std::unique_ptr<DumbTtyElementDelegate> delegate_;
protected:
    Element* root_;
};
    
} // namespace ui
} // namespace aft
