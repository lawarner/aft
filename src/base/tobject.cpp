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

#include <iostream>
#include "callback.h"
#include "context.h"
#include "result.h"
#include "thread.h"
#include "tobasictypes.h"
#include "tobject.h"
#include "tobjecttree.h"

using namespace aft::base;


static bool findVisitor(TObject* obj, void* data)
{
    if (!obj || !data) return false;

    const std::string* name = (const std::string *) data;
    if (obj->getName() == *name)
    {
        return true;
    }
    return false;
}

static bool runVisitor(TObject* obj, void* data)
{
    std::cout << "Run visitor " << obj->getName() << std::endl;
//    Context* context = (Context*) data;
//    const Result result = obj->run(context);

    return true;
}


TObject::TObject(const std::string& name)
    : name_(name)
    , state_(UNINITIALIZED)
    , result_(Result(false))
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

const Result&
TObject::getResult() const
{
    return result_;
}

TObject::State
TObject::getState() const
{
    return state_;
}

bool TObject::rewind(Context* context)
{
    result_ = Result(false);
    return false;
}

const Result
TObject::run(Context* context)
{
// check state_ is INITIAL
    //TODO: return context->runner(testObject);
//    return const_cast<TObject&>(testObject);
//    return const_cast<TObject&>(*this);
    result_ = Result(this);
    return result_;
}

const Result
TObject::start(Context* context, Callback* callback)
{
    ThreadManager* tman = ThreadManager::instance();
    ThreadHandler* thread = tman->thread(this, context);
    thread->notify(callback);
    thread->run();

    return thread->getResult();
}

bool TObject::operator==(const TObject& other) const
{
    if (getName() == other.getName() &&
        getState() == other.getState())
    {
        return true;
    }

    return false;
}

bool TObject::operator!=(const TObject& other) const
{
    return !operator==(other);
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


// -------------------------------------

TObjectContainer::TObjectContainer(const std::string& name)
    : TObject(name)
    , children_(0)
{
}

TObjectContainer::~TObjectContainer()
{
    if (children_) delete children_;
}


TObjectTree*
TObjectContainer::add(TObject* tObject, TObjectTree* tObjWrapper)
{
    if (!children_)
    {
        children_ = new TObjectTree(this);
    }

    if (tObjWrapper)
    {
        return tObjWrapper->add(tObject);
    }

    return children_->add(tObject);
}

TObject*
TObjectContainer::find(const TObjectKey& key)
{
    TObjectTree::Children::iterator it;
    it = children_->visitUntil(findVisitor, (void *)&key);

    if (it != children_->getChildren().end())
    {
        return (*it)->getValue();
    }

    return 0;
}

bool TObjectContainer::remove(TObject* testObject)
{
    if (!children_) return false;

    return children_->remove(testObject);
}

Children* TObjectContainer::getChildren() const
{
    return children_;
}

const Result
TObjectContainer::run(Context* context)
{
    bool retval;
    if (children_)
    {
        retval = children_->visit(runVisitor, context);
    } else {
        retval = runVisitor(this, context);
    }

    result_ = Result(retval);
    return result_;
}

const Result
TObjectContainer::start(Context* context, Callback* callback)
{
    //TODO theader ...
    Result result = run(context);
    if (callback)
    {
        callback->callback(&result);
    }

    result_ = Result(true);
    return result_;
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
