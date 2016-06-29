#pragma once
//
//  uidelegate.h
//  libaft
//
//  Created by Lynn Warner on 7/1/16.
//  Copyright © 2016 Andy Warner. All rights reserved.
//

#include <string>


namespace aft
{
namespace ui
{
// Forward reference
class Element;

/**
 *  Interface of basic Element interactions
 */
class UIDelegate
{
public:
    /** Add the element to the user interface, if possible. */
    virtual bool add(const Element& element) = 0;
    /** Get user input from element */
    virtual bool input(const Element& element, std::string& value) = 0;
    /** Output the element to the user interface */
    virtual bool output(const Element& element) = 0;
    /** Remove or hide element from the user interface, if possible. */
    virtual bool remove(const Element& element) = 0;
};

} // namespace ui
} // namespace aft
