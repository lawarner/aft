#pragma once
/*
 *  uicommand.h
 *  libaft
 *
 *  Copyright © 2016-2017 Andy Warner. All rights reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "base/tobasictypes.h"

namespace aft {
namespace ui {

// Forward reference
class Element;

enum class CommandType {
    Start,
    AddElement,
    RemoveElement,
    SetDefaultValue,
    Exit
};

class UICommand : public base::TOBlob {
public:
    UICommand(CommandType commandType, base::Blob* blob = nullptr);
    UICommand(CommandType commandType, Element* element);
    virtual ~UICommand() = default;

    //TODO need a UIContext
    virtual const base::Result process(base::Context* context = 0);

    CommandType getCommandType() const;
    /** Get related Element, if any. */
    Element* getElement() const;

private:
    CommandType commandType_;
};

} // namespace ui
} // namespace aft
