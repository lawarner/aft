/*
 *  uicommand.cpp
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

#include "element.h"
#include "uicommand.h"
#include "base/blob.h"

namespace aft {
namespace ui {

UICommand::UICommand(CommandType commandType, base::Blob* blob)
    : base::TOBlob(base::TObjectType::TypeUiCommand, blob, "UiCommand")
    , commandType_(commandType) {

}

UICommand::UICommand(CommandType commandType, Element* element)
    : base::TOBlob(base::TObjectType::TypeUiCommand, new base::Blob("element", element), "UiCommand")
    , commandType_(commandType) {

}
    
CommandType
UICommand::getCommandType() const {
    return commandType_;
}

Element*
UICommand::getElement() const {
    Element* element = nullptr;
    base::Blob* blob = getValue();
    if (blob && blob->getData()) {
        void* bdata = const_cast<void *>(blob->getData());
        element = static_cast<Element *>(bdata);
    }
    return element;
}

const base::Result UICommand::process(base::Context* context) {
    switch (commandType_) {
        case CommandType::Start:
            break;
            
        default:
            break;
    }
    return false;
}

}
}
