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

#include "logger.h"
#include "loghandler.h"
#include "base/tobasictypes.h"
using namespace aft;
using namespace aft::core;

LogHandler::LogHandler()
    : base::PropertyHandler("LogHandler")
{
}

LogHandler::~LogHandler()
{
}

// Take string from tObject and call setup(), return TOTrue
base::TObject& LogHandler::handle(const base::TObject& tObject)
{
    const base::TOString& logConfig = dynamic_cast<const base::TOString&>(tObject);
    if (logConfig.getValue().empty())
    {
        //TODO try to get another string
        setup("Untitled");
    } else {
        setup(logConfig.getValue());
    }
    return base::TOTrue;
}

base::TObject& LogHandler::handle(base::Context* context, const base::TObject& tObject)
{
    // ignoring context for now
    return handle(tObject);
}

// Convenience methods.
void LogHandler::setup(const std::string& logConfig)
{
    Logger::initStandardLoggers(logConfig);
}

