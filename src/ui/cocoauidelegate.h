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

// Forward reference
class CocoaElementDelegate;
class UI;

class CocoaUIDelegate : public BaseUIDelegate {
public:
    CocoaUIDelegate(UI* ui);
    virtual ~CocoaUIDelegate() = default;
public:
    virtual bool add(const Element& element) override;
    virtual void flush(const Element& element) override;
    virtual bool focus(const Element& element) override;
    virtual bool hide(const Element& element) override;
    virtual bool input(const Element& element, std::string& value) override;
    virtual bool output(const Element& element, bool showValue = false) const override;
    virtual bool show(const Element& element) override;
    
protected:
    std::unique_ptr<CocoaElementDelegate> delegate_;
};

} // namespace ui
} // namespace aft
