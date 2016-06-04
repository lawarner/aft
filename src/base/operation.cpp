/*
 *  operation.cpp
 *  libaft
 *
 *  Copyright © 2016 Andy Warner. All rights reserved.
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

#include "operation.h"
using namespace aft::base;

Operation::Operation(Type operation)
: type_(operation)
{
    
}

Operation::Operation(Type operation, std::vector<std::string>& parameters,
                     std::vector<TObject *>& objects)
: type_(operation)
, parameters_(parameters)
, objects_(objects)
{
    
}

Operation::~Operation()
{

}

void Operation::addObject(TObject *tObject)
{
    objects_.push_back(tObject);
}

void Operation::addParameter(const std::string& parameter)
{
    parameters_.push_back(parameter);
}

bool Operation::isValid(bool isStrict) const
{
    size_t numObjects = objects_.size();
    size_t numParameters = parameters_.size();

    bool retval = false;

    switch (type_)
    {
        case OperatorZero:
        case OperatorIsNull:
        case OperatorIsTrue:
        case OperatorIsFalse:
            retval = !isStrict || !(numObjects && numParameters);
            break;
        case OperatorAdd:
        case OperatorAppend:
            retval = numObjects > 0 || numParameters > 0;
            break;
        case OperatorSet:
            retval = isStrict ? numObjects == 1 || numParameters == 1 : !(numObjects && numParameters);
            break;
        case OperatorCompare:
        case OperatorIsLessThan:
        case OperatorIsLessThanOrEqual:
        case OperatorIsEqual:
        case OperatorIsGreaterThanOrEqual:
        case OperatorIsGreaterThan:
            retval = numObjects == 1 || numParameters == 1;
            break;
        case OperatorIfTheElse:
            retval = numObjects == 2 || numParameters == 2;
            break;
        default:
            break;
    }

    return retval;
}

const std::vector<TObject *>&
Operation::getObjects() const
{
    return objects_;
}

const std::vector<std::string>&
Operation::getParameters() const
{
    return parameters_;
}

Operation::Type Operation::getType() const
{
    return type_;
}
