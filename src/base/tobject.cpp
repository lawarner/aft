/*
 *   Copyright 2015, 2016 Andy Warner
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
#include "tobjecttype.h"
#include "visitor.h"

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

class RunVisitor : public VisitorContract
{
public:
    Result visit(TObject* obj, void* data)
    {
        Context* context = (Context *) data;
        obj->process(context);
        return Result(true);
    }
};

/*
static Result traceVisitor(TObject* obj, void* data)
{
    if (data)
    {
        Context* context = (Context*) data;
        std::cout << "Trace visit " << obj->getName()
                  << ", context=" << context->getName() << std::endl;
    } else {
        std::cout << "Run visitor " << obj->getName() << std::endl;
    }

    return Result(true);
}
*/

TObject::TObject(const std::string& name)
: type_(TObjectType::get(TObjectType::NameBase))
, name_(name)
, state_(UNINITIALIZED)
, result_(Result(false))
{

}

TObject::TObject(TObjectType& type, const std::string& name)
: type_(type)
, name_(name)
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

TObjectType&
TObject::getType() const
{
    return type_;
}

void TObject::setName(const std::string& name)
{
    name_ = name;
}

TObject::State
TObject::setState(State state)
{
    State oldState = state_;
    state_ = state;
    return oldState;
}

bool TObject::rewind(Context* context)
{
    result_ = Result(false);
    return false;
}

const Result
TObject::process(Context* context)
{
    return Result(true);
}

const Result
TObject::run(Context* context)
{
// check state_ is PREPARED
//    result_ = Result(this);
//    return result_;
    if (context)
    {
        result_ = context->getVisitor().visit(this, context);
    } else {
        RunVisitor runVisitor;
        result_ = runVisitor.visit(this, context);
    }
//TODO set state_ as one of finished
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

bool TObject::stop(bool force)
{
    ThreadManager* tman = ThreadManager::instance();
    ThreadHandler* thread = tman->find(this);
    if (thread)
    {
        thread->stop(force);
        return true;
    }

    return false;
}

bool TObject::operator==(const TObject& other) const
{
    if (getName() == other.getName() &&
        getType() == other.getType() &&
        getState() == other.getState() &&
        getResult() == other.getResult())
    {
        return true;
    }

    return false;
}

bool TObject::operator!=(const TObject& other) const
{
    return !operator==(other);
}

TObject& TObject::operator=(const TObject& other)
{
    if (this != &other)
    {
        type_ = other.type_;
        name_ = other.name_;
        state_ = other.state_;
        result_ = other.result_;
    }

    return *this;
}

bool
TObject::serialize(Blob& blob)
{
    return false;
}

bool
TObject::deserialize(const Blob& blob)
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

bool TObjectContainer::remove(TObject* tObject)
{
    if (!children_) return false;

    return children_->remove(tObject);
}

Children* TObjectContainer::getChildren() const
{
    return children_;
}

const Result
TObjectContainer::run(Context* context)
{
    RunVisitor runVisitor;
    VisitorContract& visitor = context ? context->getVisitor() : runVisitor;
    if (children_)
    {
        result_ = children_->visit(visitor, context);
    } else {
        result_ = visitor.visit(this, context);
    }

    return result_;
}

bool
TObjectContainer::serialize(Blob& blob)
{
    return false;
}

bool
TObjectContainer::deserialize(const Blob& blob)
{
    return false;
}
