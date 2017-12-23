#pragma once
//
//  basicuis.h
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "ui.h"

namespace aft {
namespace ui {

/** Subclasses of UI hold the specific differences between various input methods.
 *  A tty type of UI in general will have a sort of menu to select the element and
 *  then a prompt and wait (block) for user input at the keyboard.
 *  A gui type of UI is typically form-based where the user navigates and types in
 *  values for elements at will. A button press indicates an action and reads all
 *  element values at once.
 *  A html type of UI is similar to a gui type in that a form in presented as a web page
 *  where elements are acccesed and modified at will. A user action, typically a button,
 *  causes the form to be read as a whole.
 *
 *  Expect more top-level ui types in the future.
 */

class UiTty : public UI {
    
};

class UiGui : public UI {
        
};

class UiHtml : public UI {
        
};

} // namespace ui
} // namespace aft
