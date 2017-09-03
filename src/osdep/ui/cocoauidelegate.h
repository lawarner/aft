#pragma once
//
//  cocoauidelegate.h
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "ui/uidelegate.h"

namespace aft {
namespace ui {
// Forward refernce
class UI;
}
    
namespace osdep {

// Forward reference
class CocoaElementDelegate;

class CocoaUIDelegate : public ui::BaseUIDelegate {
public:
    CocoaUIDelegate(ui::UI* ui);
    virtual ~CocoaUIDelegate() = default;
public:
    virtual bool add(const ui::Element& element) override;
    virtual void flush(const ui::Element& element) override;
    virtual bool focus(const ui::Element& element) override;
    virtual bool hide(const ui::Element& element) override;
    virtual bool input(ui::Element& element, std::string& value) override;
    virtual bool output(const ui::Element& element, bool showValue = false) const override;
    virtual bool show(const ui::Element& element) override;
    
protected:
    std::unique_ptr<CocoaElementDelegate> delegate_;
};

} // namespace osdep
} // namespace aft
