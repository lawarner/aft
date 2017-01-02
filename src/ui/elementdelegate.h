#pragma once
//
//  elementdelegate.h
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

namespace aft {
    namespace ui {
        // Forward reference
        class Element;
        
        /**
         *  Interface of basic Element interactions
         */
        class ElementDelegate
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
     
    }
}
