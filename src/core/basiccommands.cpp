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
#include "logger.h"
using namespace aft;
using namespace aft::core;


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

