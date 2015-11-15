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

#include "result.h"

namespace aft
{
namespace base
{
// Forward reference
class TObject;

/** Define a visitor C function for TObject */
typedef bool (*BVisitor)(TObject* obj, void* data);
typedef Result (*CVisitor)(TObject* obj, void* data);


/**
 *  Standard Visitor interface for TObjectTree and other classes to use.
 */
class VisitorContract
{
public:
    virtual Result visit(TObject* obj, void* data) = 0;
};

/**
 *  Default implementation for visitor that just returns a true Result.
 */
class Visitor : public VisitorContract
{
public:
    virtual Result visit(TObject* obj, void* data)
    {
        return Result(true);
    }
};

} // namespace base
} // namespace aft
