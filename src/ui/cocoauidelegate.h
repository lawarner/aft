#pragma once
//
//  cocoauidelegate.hpp
//  libaft
//
//  Created by Lynn Warner on 4/14/17.
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "uidelegate.h"

namespace aft {
namespace ui {
    

class CocoaUIDelegate : public UIDelegate {
public:
    CocoaUIDelegate();
    virtual ~CocoaUIDelegate() = default;
public:
    virtual bool add(const Element& element) override;
    virtual bool focus(const Element& element) override;
    virtual const Element* get(const std::string& name) override;
    virtual bool hide(const Element& element) override;
    virtual bool input(const Element& element, std::string& value) override;
    virtual bool output(const Element& element) const override;
    virtual bool remove(const Element& element) override;
    virtual bool show(const Element& element, bool showValue = false) override;
    
protected:

};

} // namespace ui
} // namespace aft
