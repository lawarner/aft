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

#include <map>
#include <sstream>

#include "logger.h"
#include "commandcontext.h"
#include "base/tobasictypes.h"
using namespace aft;
using namespace aft::core;


static const std::string DELIMIT_NAME("::");
static const std::string DELIMIT_NUMBER("##");

// Helper functions

static std::string
makeKey(const std::string& command, const std::string& argName)
{
    std::ostringstream ss(command);
    ss << DELIMIT_NAME << argName;
    return ss.str();
}

static std::string
makeKey(const std::string& command, int argNumber)
{
    std::ostringstream ss(command);
    ss << DELIMIT_NUMBER << argNumber;
    return ss.str();
}

///////////////////////////////////
class aft::core::CommandContextImpl
{
public:
    CommandContextImpl()
        : handler_(*new base::PropertyHandler("CommandHandler"))
        {  }
    virtual ~CommandContextImpl()
        {
            delete &handler_;
        }

    base::PropertyHandler& handler_;
    std::map<std::string, aft::base::TObject> tobjects_;
};


CommandContext::CommandContext()
    : impl_(*new CommandContextImpl)
{
}

CommandContext::~CommandContext()
{
    delete &impl_;
}


bool CommandContext::getArgument(const std::string& command, int argNumber,
                     base::TObject& tObject)
{
    return true;
}

bool CommandContext::getArgument(const std::string& command, const std::string& argName,
                     base::TObject& tObject)
{
    return true;
}

std::string CommandContext::getArgument(const std::string& command, int argNumber,
                                        const std::string& defValue)
{
    const std::string& retval = impl_.handler_.getValue(makeKey(command, argNumber));
    if (retval.empty())
    {
        return defValue;
    }
    return retval;
}

std::string CommandContext::getArgument(const std::string& command, const std::string& argName,
                                        const std::string& defValue)
{
    const std::string& retval = impl_.handler_.getValue(makeKey(command, argName));
    if (retval.empty())
    {
        return defValue;
    }
    return retval;
}

bool CommandContext::setArgument(const std::string& command, int argNumber,
                 const base::TObject& tObject)
{
    impl_.tobjects_[makeKey(command, argNumber)] = tObject;
    return true;
}

bool CommandContext::setArgument(const std::string& command, const std::string& argName,
                     const base::TObject& tObject)
{
    impl_.tobjects_[makeKey(command, argName)] = tObject;
    return true;
}

bool CommandContext::setArgument(const std::string& command, int argNumber,
                                 const std::string& value)
{
    impl_.handler_.setValue(makeKey(command, argNumber), value);
    return true;
}

bool CommandContext::setArgument(const std::string& command, const std::string& argName,
                                 const std::string& value)
{
    impl_.handler_.setValue(makeKey(command, argName), value);
    return true;
}

