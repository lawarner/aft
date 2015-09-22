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


namespace aft
{
namespace core
{

/**
 *  Context used to run test cases.
 */
class RunContext : public aft::base::Context
{
public:
    RunContext();
    virtual ~RunContext();

    // logger, readers, writers, dispositions
    // gui
};

} // namespace core
} // namespace aft
