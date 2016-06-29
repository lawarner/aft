#pragma once
/*
 *   Copyright 2015, 2016 Andy Warner
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

#include <map>
#include <vector>

#include "base/command.h"
#include "base/result.h"

namespace aft
{
namespace base
{
// Forward reference
class BaseConsumer;
class BaseProducer;
class Context;
}

namespace core
{
/**
 *  Basic, built-in commands.
 *  Commands hold no state or values of their own, but use the context.
 *  If no context is passed to process() then the global context is used instead.
 *  Most commands have multiple functions (i.e., Env:get,set,unset,...) but this is only
 *  as a convience. There could just as well be separate classes EnvGet, EnvSet, EnvUnset, ...
 */

/** Simply logs its argument when run.
 *
 *  In the future, this could log/trace before/after running any sub-command.
 *  It could also log only if message changed.
 */
class LogCommand : public aft::base::Command
{
public:
    LogCommand(const std::string& message = std::string(),
               const std::string& type = std::string());
    LogCommand(const std::vector<std::string>& parameters);

    // Implement Command interface
    virtual const base::Result process(base::Context* context = 0);

private:
    std::string message_;
    std::string type_;
};

/** Handle consumers.
 *
 *  Thru parameters, this command handles open, openw, canAcceptData, write and close.
 *  openw will overwrite existing files.
 *  TODO open+ for append mode.
 *  Currently only fileconsumers are supported.
 */
class ConsCommand : public aft::base::Command
{
public:
    ConsCommand(const std::string& type, const std::string& name = std::string());
    virtual ~ConsCommand();
    
    // Implement Command interface
    virtual const base::Result process(base::Context* context = 0);

private:
    std::string type_;      //TODO convert to enum
    base::BaseConsumer* consumer_;
};

/** Handle producers.
 *
 *  Thru parameters, this command handles open, hasData, read and close.
 *  Currently only fileproducers are supported.
 */
class ProdCommand : public aft::base::Command
{
public:
    ProdCommand(const std::string& type, const std::string& name = std::string());
    virtual ~ProdCommand();
    
    // Implement Command interface
    virtual const base::Result process(base::Context* context = 0);

private:
    std::string type_;      //TODO convert to enum
    base::Blob* buffer_;    // holds temporary results
    base::BaseProducer* producer_;
};

/** Handle environment variables
 *
 *  This command handles get, set (global,local), unset, [list]
 */
class EnvCommand : public aft::base::Command
{
public:
    EnvCommand(const std::string& type, const std::string& name,
               const std::string& value = std::string());
    virtual ~EnvCommand();
    
    // Implement Command interface
    virtual const base::Result process(base::Context* context = 0);

private:
    std::string type_;      //TODO convert to enum
};

/** Container for multiple commands
 */
class GroupCommand : public aft::base::Command
{
public:
    GroupCommand(const std::string& name);
    virtual ~GroupCommand();

    // override add method to check that children are of type Command.
    aft::base::TObjectTree* add(aft::base::TObject* tObject, aft::base::TObjectTree* tObjWrapper = 0);
    
    // Implement Command interface
    virtual const base::Result process(base::Context* context = 0);
    //TODO implement run()
};

/** Handle if/then/else conditional
 *
 *  This command handles if, then, else
 */
class IfCommand : public aft::base::Command
{
public:
    IfCommand(const std::string& name, const base::Operation& condition,
              base::Command* trueCommand, base::Command* falseCommand = 0);
    virtual ~IfCommand();
    
    // Implement Command interface
    virtual const base::Result process(base::Context* context = 0);
    
private:
    /** Condition to evaluate. */
    const aft::base::Operation& condition_;
    /** Command to execute if condition is true. */
    aft::base::Command* trueCommand_;
    /** Command to execute if condition is false. */
    aft::base::Command* falseCommand_;
};

} // namespace core
} // namespace aft
