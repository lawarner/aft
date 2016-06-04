#pragma once
/*
 *   operation.h
 *   libaft
 *
 *   Copyright © 2016 Andy Warner
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

#include <vector>
#include "result.h"


namespace aft
{
namespace base
{
// Forward reference
class TObject;

/** Data holder class for operations.
 *  Will be extended later to use Hasher.
 *
 */

class Operation
{
public:
    //TODO make easy to extend by adding new operators (perhaps by string/Hasher)
    enum Type
    {
        OperatorZero,
        OperatorAdd,
        OperatorSet,
        OperatorCompare,
        OperatorIsLessThan,
        OperatorIsLessThanOrEqual,
        OperatorIsEqual,
        OperatorIsGreaterThanOrEqual,
        OperatorIsGreaterThan,
        OperatorIsNull,
        OperatorIsTrue,
        OperatorIsFalse,
        OperatorIfTheElse,
        OperatorAppend,
        OperatorMaxOperator
    };

    Operation(Type operation);
    Operation(Type operation, std::vector<std::string>& parameters,
              std::vector<TObject *>& objects);
    virtual ~Operation();

    void addObject(TObject *tObject);
    void addParameter(const std::string& parameter);

    /** Determine if this operation has all required objects or parameters and validates them
     *  @param isStrict if set then this method checks that the exact number of parameters
     *                  or objects have been specified. If not set then checks that the minimum
     *                  requirements have been met to apply this operation.
     *
     *TODO normalize method to pre-parse objects and parameters?
     *TODO optional context?
     * There could be an internal Operation where the first object does the applyOperation (applyInternalOperation)
     */
    bool isValid(bool isStrict = false) const;

    const std::vector<TObject *>& getObjects() const;

    const std::vector<std::string>& getParameters() const;

    Type getType() const;

protected:
    Type type_;
    std::vector<std::string> parameters_;
    std::vector<TObject *> objects_;
};


/** Interface for objects to implement to use operations */
class OperationClientContract
{
public:
    // TODO Use context
    
    /** Apply the operation if possible.
     *  @return result of applying the operation or Result(FATAL) if cannot apply.
     */
    virtual Result applyOperation(const Operation& operation) = 0;

    /** Determine if an operation can be applied.
     *  @return true if the operation can be applied, otherwise false.
     */
    virtual bool supportsOperation(const Operation& operation) = 0;
};

} // namespace base
} // namespace aft
