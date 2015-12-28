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
#include "basiccommands.h"
#include "commandcontext.h"
#include "logger.h"
using namespace aft;
using namespace aft::core;


LogCommand::LogCommand(const std::string& message)
    : Command("Log")
    , message_(message)
{
    parameters_.push_back(message_);
}

const base::Result
LogCommand::process(base::Context* context)
{
    if (message_.empty())
    {
        aftlog << "Command logger: " << (context ? context->getName() : "(no context)")
               << std::endl;
    } else {
        aftlog << "Command logger: " << message_ << std::endl;
    }

    return base::Result(true);
}

const base::Result
LogCommand::setup(base::Context* context)
{
    CommandContext* cmdcontext = dynamic_cast<CommandContext *>(context);
    //TODO get and set 
    return base::Result(true);
}

