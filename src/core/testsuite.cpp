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
#include "base/result.h"
#include "base/tobjecttree.h"
#include "testsuite.h"
using namespace aft;
using namespace aft::core;


bool
TestSuite::open()
{
    return true;
}

bool
TestSuite::rewind(base::Context* context)
{
    return TObjectContainer::rewind(context);
}

const base::Result
TestSuite::run(base::Context* context)
{
    base::TObjectContainer::iterator iter;
    base::Result result(false);
    if (!children_) return result;

    for (iter = children_->begin(); iter != children_->end(); ++iter)
    {
        result = iter->run(context);
    }
    return result;
}

void
TestSuite::close()
{

}
