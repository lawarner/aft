#pragma once
/*
 *   Copyright 2015-2017 Andy Warner
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
 *  TODO split into Command interface and BaseCommand implementation.
 */
class Command : public aft::base::TObjectContainer
{
public:
    /** Define data structure used for parameters. */
    typedef std::vector<std::string> ParameterList;

    /** Construct command with given name. */
    Command(const std::string& name);

    /** Construct command with given name and (deserialized) parameters. */
    Command(const std::string& name, const ParameterList& parameters);
    
    /** Destruct command. */
    virtual ~Command();

    /** Process the command according to context.
     *  If context is not specified, then command will be processed using the global context.
     *  @return Result of processing the command.
     */
    virtual const Result process(Context* context = nullptr) = 0;

    /** Only iterates down to CommandType level */
    virtual const Result run(Context* context = nullptr);

    /** Set up static info from context and optional blob parameters. */
    virtual const Result setup(Context* context = nullptr, const Blob* parameters = nullptr);

    // Implement SerializeContract Interface
    virtual bool serialize(Blob& blob);
    virtual bool deserialize(const Blob& blob);
    
    /** Copy contents of this Command from another */
    virtual Command& operator=(const Command& other);

protected:
    // Or maybe use StructuredData
    ParameterList parameters_;
};

} // namespace base
} // namespace aft
