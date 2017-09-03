#pragma once

#include "ui/elementdelegate.h"

namespace aft {
namespace osdep {


class CocoaElementDelegate : public ui::ElementDelegate {
public:
    CocoaElementDelegate() = default;
    virtual ~CocoaElementDelegate() = default;
public:
    virtual void flush(const ui::Element* element) override;
    virtual const std::string& getValue(const ui::Element* element) const override;
    virtual void setValue(ui::Element* element, const std::string& value) override;
    virtual bool getFocus(ui::Element* element) const override;
    virtual bool setFocus(ui::Element* element, bool hasFocus = true) override;
    virtual bool getFacet(ui::Element* element, const std::string& name, ui::UIFacetCategory category,
                          ui::UIFacet& facet) const override;
    virtual bool setFacet(ui::Element* element, const ui::UIFacet& facet) override;
    
    virtual bool input(ui::Element* element, std::string& value) override;
    virtual bool output(const ui::Element* element, bool showValue = false) override;

};

} // namespace ui
} // namespace aft
