/*
 *   Copyright 2016 Andy Warner
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

#include "context.h"
#include "predicate.h"
#include "tobjecttree.h"
#include "tobjecttype.h"
#include "result.h"
#include "visitor.h"
using namespace aft::base;


const std::string BasePredicate::TOTypeName("Predicate");
const TObjectType& BasePredicate::TOType = TObjectType::get(TOTypeName);


class EvalVisitor : public VisitorContract
{
public:
    Result visit(TObject* obj, void* data)
    {
        Context* context = (Context *) data;
        return obj->process(context);
    }
};

class EvalContext : public Context
{
public:
    EvalContext(const std::string& name = std::string());
    virtual ~EvalContext();
    
private:
    EvalVisitor evalVisitor_;
};

EvalContext::EvalContext(const std::string& name)
: aft::base::Context(evalVisitor_, name)
{
    
}

EvalContext::~EvalContext()
{
    
}

BasePredicate::BasePredicate()
: TObjectContainer(TOType)
{
    
}

BasePredicate::BasePredicate(const TObject* condition)
: TObjectContainer(TOType)
{
    add(const_cast<TObject *>(condition));
}

BasePredicate::~BasePredicate()
{
    
}

bool BasePredicate::isTrue() const
{
    TObjectTree* children = getChildren();
    if (!children)
    {
        return false;
    }

    EvalContext context;
    TObjectTree::Children::iterator it = children->visitUntil(context.getVisitor(), 0);
    return it != children->getChildren().end();
}

bool BasePredicate::operator==(const PredicateContract& other) const
{
    return isTrue() == other.isTrue();
}
