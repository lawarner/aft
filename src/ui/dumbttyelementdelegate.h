#pragma once
//
//  dumbttyelementdelegate.hpp
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "ui/elementdelegate.h"

namespace aft {
namespace ui {

class DumbTtyElementDelegate : public ElementDelegate {
public:
    DumbTtyElementDelegate();
    ~DumbTtyElementDelegate() = default;
    
    virtual const std::string& getValue(const Element* element) const override;
    virtual void setValue(Element* element, const std::string& value) override;
    virtual bool getFocus(Element* element) const override;
    virtual bool setFocus(Element* element, bool hasFocus = true) override;
    virtual bool getFacet(Element* element, const std::string& name, UIFacetCategory category,
                          UIFacet& facet) const override;
    virtual bool setFacet(Element* element, const UIFacet& facet) override;
    
    virtual bool input(Element* element, std::string& value) override;
    virtual bool output(const Element* element) override;
};

} // namespace ui
} // namespace aft
