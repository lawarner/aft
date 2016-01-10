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

#include "base/context.h"
#include "base/propertyhandler.h"
#include "base/tobject.h"


namespace aft
{
namespace core
{
// Forward reference
class CommandContextImpl;

/**
 *  Used for running commands.
 *  @deprecated Will probably refactor to set of interfaces that various sub-classes
 *              of Context can implement (see RunContext).
 */
class CommandContext : public aft::base::Context
{
public:
    CommandContext();
    virtual ~CommandContext();

    // Convenience methods
    bool getArgument(const std::string& command, int argNumber,
                     aft::base::TObject& tObject);
    bool getArgument(const std::string& command, const std::string& argName,
                     aft::base::TObject& tObject);
    std::string getArgument(const std::string& command, int argNumber,
                            const std::string& defValue = std::string());
    std::string getArgument(const std::string& command, const std::string& argName,
                            const std::string& defValue = std::string());
    bool setArgument(const std::string& command, int argNumber,
                     const aft::base::TObject& tObject);
    bool setArgument(const std::string& command, const std::string& argName,
                     const aft::base::TObject& tObject);
    bool setArgument(const std::string& command, int argNumber,
                     const std::string& value);
    bool setArgument(const std::string& command, const std::string& argName,
                     const std::string& value);

private:
    CommandContextImpl& impl_;
};

} // namespace core
} // namespace aft
