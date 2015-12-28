#pragma once
/*
 *   Copyright 2015 Andy Warner
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

#include "base/result.h"
#include "base/tobject.h"

namespace aft
{
namespace base
{
// Forward reference
class Context;

/** A TObject to be run in (hierarchical) sequence.
 *
 *  TODO need to split into Command interface and BaseCommand implementation.
 */
class Command : public aft::base::TObjectContainer
{
public:
    /** Construct command with given name. */
    Command(const std::string& name);

    /** Destruct command. */
    virtual ~Command();

    /** Process the command according to context. */
    virtual const Result process(Context* context = 0) = 0;
    /** Set up static info from context. */
    virtual const Result setup(Context* context = 0) = 0;

    // Implement SerializeContract Interface
    virtual bool serialize(Blob& blob);
    virtual bool deserialize(const Blob& blob);

protected:
    // Or maybe use StructuredData
    std::vector<std::string> parameters_;
};

} // namespace base
} // namespace aft
