#pragma once
/*
 *  ui.h
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

#include <cstddef>
#include <functional>
#include "base/proc.h"
#include "base/result.h"
#include "ui/uidelegate.h"


namespace aft
{
namespace base
{
// Forward reference
class Context;
}

namespace ui
{
// Forward reference
class Element;

        
/**
 *  A UI is basically a Producer/Consumer.
 *  The consumer receives commands and entities and the producer yields named result values.
 *  An initial UI element can be specified but this can be dynamically expanded by consumer write methods.
 *  The UI has a UIDelegate to perform the operations in a specific way.
 *
 *  Elements within a UI can use different ElementDelegates, but when they use a single instance then the delegate
 *  can implement various interactions between elements. StackUIDelegate in t_ui shows a simple example of this.
 *  LayoutElements will enforce that all child elements use its UI delegate (probably permanently hide or
 *  eject childen element that use a different delegate?).
 *
 *  TODO: need chain of command with order from most recently added. This is to select the proper element.
 *  TODO: need a way to specify callbacks for user actions, for example tracking mouse movements.
 *
 *  Agree on the most generic of UI hierarchies such as element, field, group (frame/uicluster/dialog/form/menu)
 *  For some (dumb) UI's groups would fail down.
 *  For example, a dumb terminal only has hierarchical menus and fields.
 *
 *  The UI uses a UIDelegate to perform operations in a platform specific way. An Element uses a ElementDelegate
 *  to perform its operations, also in a platform specific way.
 *  UI operations include refresh, getBounds, draw, hide, focus, describe, etc.
 *  Element operations include setups, getValue, setValue, getUserInput, setUserOutput, etc.
 */
class UI : public aft::base::BaseProc
{
public:
    using ElementList = std::vector<Element *>;
    using CallbackFunction = std::function<void(Element*)>;
    
    /** Construct a UI with an optional top level element, optional maximum size and optional UI delegate.
     *  @param element Optional top level Element. More top level elements can be added later by
     *                 using #addElement.
     *  @param maxSize Maximum top level Elements allowed. If not specified then size is unlimited.
     *  @param uiDelegate UI delegate to use. If not specified (or 0) then the default UI delegate is used.
     */
    UI(Element* element = nullptr, unsigned int maxSize = 0, UIDelegate* uiDelegate = nullptr);
    /** Destruct a UI. */
    virtual ~UI();
    
    /** Add an element to top level of UI hierarchy */
    virtual bool addElement(Element* element);
    
    /** Get a pointer to the current top level element, if any. */
    virtual Element* currentElement() const;

    /** Return index to an element, or -1 if not found. */
    virtual ssize_t findElement(Element* element) const;

    /** Switch to the first top level element.
     *  @return true if the switch to the first element was successful.
     */
    virtual bool firstElement();

    Element* getElement(unsigned int idx);

    /** Switch to the next top level element if any.
     *  @return the index of the UI element after the current. Returns -1 if there is no next element.
     */
    virtual ssize_t nextElement();

    /** Remove element from top level of UI hierarchy */
    virtual bool removeElement(Element* element);

    // many UI's need to have an init and deinit
    virtual base::Result init(base::Context* context = nullptr);
    virtual base::Result deinit(base::Context* context = nullptr);
    
    // High level methods
    virtual void draw();
    virtual void erase();
    virtual void input();
    virtual void output();
    void registerListener(CallbackFunction* listener);
    void unregisterListener(CallbackFunction* listener);
    
    // Producer contract
    virtual bool read(base::TObject& object) override;
    virtual bool read(base::Result& result) override;
    virtual bool read(base::Blob& blob) override;
    virtual bool hasData() override;
    virtual bool hasObject(base::ProductType productType) override;
    
    // Consumer contract
    virtual bool canAcceptData() override;
    virtual bool write(const base::TObject& object) override;
    virtual bool write(const base::Result& result) override;
    virtual bool write(const base::Blob& blob) override;

private:
    unsigned int maxSize_;
    unsigned int currentElement_;
    ElementList uiElements_;
    std::vector<CallbackFunction*> listeners_;
    std::unique_ptr<UIDelegate> uiDelegate_;
};

} // namespace ui
} // namespace aft
