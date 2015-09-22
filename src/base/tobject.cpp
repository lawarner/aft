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

#include "context.h"
#include "tobject.h"

using namespace aft::base;


static bool runVisitor(TObject* obj, void* data)
{
    Context* context = (Context*) data;
    TObject& testObject =  obj->run(context);

    return true;
}


TObject::TObject(const std::string& name)
    : name_(name)
    , state_(UNINITIALIZED)
{

}

TObject::~TObject()
{

}

const std::string&
TObject::getName() const
{
    return name_;
}

TObject::State
TObject::getState() const
{
    return state_;
}

TObject&
TObject::run(Context* context)
{
// check state_ is INITIAL
    //TODO: return context->runner(testObject);
//    return const_cast<TObject&>(testObject);
    return const_cast<TObject&>(*this);
}

bool TObject::operator==(const TObject& other)
{
    if (getName() == other.getName() &&
        getState() == other.getState())
    {
        return true;
    }

    return false;
}

Blob*
TObject::serialize()
{
    return 0;
}

bool
TObject::deserialize(const Blob* blob)
{
    return false;
}


// -----------------------------------

TObjectContainer::TObjectContainer(const std::string& name)
    : TObject(name)
    , children_(0)
{
}

TObjectContainer::~TObjectContainer()
{
    if (children_) delete children_;
}


bool TObjectContainer::add(TObject* testObject)
{
    if (!children_)
    {
        children_ = new Children;
    }
    return children_->add(testObject);
}

TObject*
TObjectContainer::find(const TObjectKey& key)
{
    return 0;
}

bool TObjectContainer::remove(TObject* testObject)
{
    if (!children_) return false;

    return children_->remove(testObject);
}

TObject&
TObjectContainer::run(Context* context)
{
    children_->visit(runVisitor, context);
//    return const_cast<TObject&>(*this);
    return *this;
/*
    TObject current = testObject;
    Children::iterator it;
    for (it = children_->begin(); it != children_->end(); ++it)
    {
        current = (*it).run(context, current);
    }
    return const_cast<TObject&>(current);
*/
}


Blob*
TObjectContainer::serialize()
{
    return 0;
}

bool
TObjectContainer::deserialize(const Blob* blob)
{
    return false;
}
