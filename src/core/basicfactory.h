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

#include "base/factory.h"

namespace aft
{
namespace base
{
// Forward reference
class Blob;
class Context;
class TObject;
}

namespace core
{

/**
 *  Factory for basic (built-in) commands.
 */
class BasicCommandFactory : public base::BaseFactory
{
public:
    /** Create a BasicCommandFactory. */
    BasicCommandFactory();
    /** Destruct a BasicCommandFactory. */
    virtual ~BasicCommandFactory();

    virtual base::TObject* construct(const std::string& name,
                                     const base::Blob* blob = 0,
                                     const base::Context* context = 0);
};

} // namespace core
} // namespace aft
