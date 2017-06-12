#pragma once
//
//  elementhandle.h
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include <stdint.h>

namespace aft {
namespace ui {

// Forward reference
class Element;

/** An element handle is way to identify a given element within a UI. */

class ElementHandle {
public:
    static const uint32_t Invalid = 0;
    // TODO probably private, friend Element
    ElementHandle(const aft::ui::Element* element = nullptr);
    ~ElementHandle() = default;

    bool operator==(const ElementHandle& other) const;
    bool isValid() const;
    uint32_t getId() const;

private:
    uint32_t id_;
    bool valid_;
};

} // namespace ui
} // namespace aft
