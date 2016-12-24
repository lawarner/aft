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

#include <pthread.h>
#include <signal.h>
#include "osdep/platform.h"
#include "base/callback.h"

using namespace aft::base;
using namespace aft::osdep;

namespace aft
{
namespace osdep
{

class PosixThreadImpl
{
public:
    PosixThreadImpl(TObject* tObject, Context* context)
        : tObject_(tObject)
        , context_(context)
        , callback_(0)
        , threadId_(PTHREAD_INIT)
        {  }

    aft::base::TObject* tObject_;
    aft::base::Context* context_;
    aft::base::Callback* callback_;
    pthread_t threadId_;
    //TODO store a Result here too TObject vs thread result
};

// Run the tobject from within the thread
void* runTObject(void* implData)
{
    PosixThreadImpl& impl = *(PosixThreadImpl *)implData;
    Result result = impl.tObject_->run(impl.context_);
    if (impl.callback_)
    {
        impl.callback_->callback(&result);
    }

    impl.tObject_->setState(TObject::FINISHED_GOOD);
    return 0;
}


PosixThreadHandler::PosixThreadHandler(TObject* tObject, Context* context)
    : impl_(*new PosixThreadImpl(tObject, context))
    , result_(Result(false))
{
}

PosixThreadHandler::~PosixThreadHandler()
{
    //TODO stop tobject and kill thread?
    delete &impl_;
}

Result PosixThreadHandler::getResult()
{
    result_ = impl_.tObject_->getResult();
    return result_;
}

TObject::State
PosixThreadHandler::getState()
{
    return impl_.tObject_->getState();
}

TObject* PosixThreadHandler::getTObject() const
{
    return impl_.tObject_;
}

void PosixThreadHandler::notify(Callback* callback)
{
    impl_.callback_ = callback;
}

bool PosixThreadHandler::unnotify(Callback* callback)
{
    impl_.callback_ = 0;
    return true;
}

void PosixThreadHandler::stop(bool force)
{
    //TODO set stop flag
    if (force)
    {
        const int killSignal = 9;
        pthread_kill(impl_.threadId_, killSignal);
        impl_.tObject_->setState(TObject::FINISHED_BAD);
    }
}

Result PosixThreadHandler::wait()
{
    void* res;
    pthread_join(impl_.threadId_, &res);
    return result_;
}

void PosixThreadHandler::run()
{
    result_ = Result(true);
    // create thread
    pthread_t tid;
    int status = pthread_create(&tid, NULL, runTObject, (void *)&impl_);
    if (status)
    {
        //TODO Log error
        impl_.tObject_->setState(TObject::FINISHED_BAD);
        return;
    }
    impl_.threadId_ = tid;
}

}
}
