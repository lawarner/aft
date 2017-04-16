#pragma once
//
//  cocoauidelegate.h
//  libafttests
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "ui/uidelegate.h"

class CocoaUIDelegate : public aft::ui::UIDelegate {
public:
    CocoaUIDelegate();
    virtual ~CocoaUIDelegate() = default;
public:
    using Element = aft::ui::Element;
    
    void addInputValue(const Element& element, const std::string& value);
    
    virtual bool add(const Element& element) override;
    virtual bool focus(const Element& element) override;
    virtual const Element* get(const std::string& name) override;
    virtual bool hide(const Element& element) override;
    virtual bool input(const Element& element, std::string& value) override;
    virtual bool output(const Element& element) const override;
    virtual bool remove(const Element& element) override;
    virtual bool show(const Element& element, bool showValue = false) override;
    
protected:
    std::unique_ptr<Element> root_;
};
