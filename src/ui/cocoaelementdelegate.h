#pragma once

#include "ui/elementdelegate.h"

namespace aft {
namespace ui {


class CocoaElementDelegate : public ElementDelegate {
public:
    CocoaElementDelegate() = default;
    virtual ~CocoaElementDelegate() = default;
public:
    virtual void flush(const Element* element) override;
    virtual const std::string& getValue(const Element* element) const override;
    virtual void setValue(Element* element, const std::string& value) override;
    virtual bool getFocus(Element* element) const override;
    virtual bool setFocus(Element* element, bool hasFocus = true) override;
    virtual bool getFacet(Element* element, const std::string& name, UIFacetCategory category,
                          UIFacet& facet) const override;
    virtual bool setFacet(Element* element, const UIFacet& facet) override;
    
    virtual bool input(const Element* element, std::string& value) override;
    virtual bool output(const Element* element, bool showValue = false) override;

};

} // namespace ui
} // namespace aft
