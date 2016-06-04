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
#include "tobject.h"


namespace aft
{
namespace base
{
// Forward reference
class Context;

/** Define a C function type for visitor using TObject */
typedef Result (*CVisitor)(TObject* obj, void* data);


/**
 *  Standard Visitor interface for TObjectTree and other classes to use.
 *
 *  TODO this needs an iteration type (parent, children, depth, order)
 */
class VisitorContract
{
public:
    virtual Result visit(TObject* obj, void* data) = 0;
};

    /**
     *  Base visitor implementation for visitor that just returns a true Result.
     *  Uses an iterator
     */
    class BaseVisitor : public VisitorContract
    {
    public:
        virtual Result visit(TObject* obj, void* data)
        {
            return Result(true);
        }
    private:
        //
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

/**
 *  Default visitor that calls each child's process() method.
 *  @param data must be a pointer to a Context or 0.
 */
class ProcessVisitor : public VisitorContract
{
public:
    Result visit(TObject* obj, void* data)
    {
        Context* context = (Context *)data;
        return obj->process(context);
    }
};

} // namespace base
} // namespace aft
