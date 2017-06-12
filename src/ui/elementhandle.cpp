//
//  elementhandle.cpp
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "elementhandle.h"
#include "element.h"

using namespace aft::ui;

ElementHandle::ElementHandle(const aft::ui::Element* element)
: id_(Invalid)
, valid_(false) {

    if (nullptr != element) {
        id_ = element->getId();
        valid_ = true;
    }
}

bool ElementHandle::operator==(const ElementHandle& other) const {
    return other.id_ == id_;
}

bool ElementHandle::isValid() const {
    return valid_;
}

uint32_t
ElementHandle::getId() const {
    return id_;
}
