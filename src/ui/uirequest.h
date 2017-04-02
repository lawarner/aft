#pragma once
//
//  uirequest.h
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include <stdint.h>


/** Request properties */
struct UIRequestProperty {
    using Type = uint32_t;
    
    static constexpr Type Mandatory = 1;
    static constexpr Type AcceptDefault = 2;
    static constexpr Type DoneAction = 4;
};

/** Requests that are sent to a UI for processing.
 *  Requests for data are pushed on a queue that the ui processes.
 *  Requests have properties such as mandatory, accept default value, "done" action.
 */
class UIRequest {
public:
    UIRequest(UIRequestProperty::Type properties = 0);
    ~UIRequest() = default;
    
private:
    UIRequestProperty::Type properties_;
};

