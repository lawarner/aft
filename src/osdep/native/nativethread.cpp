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

#include <future>
#include <mutex>
#include <signal.h>
#include "osdep/platform.h"
#include "base/callback.h"

using namespace aft::base;
using namespace std;

namespace aft
{
namespace osdep
{

// Forward reference
Result runTObject(void* implData);

class NativeThreadImpl
{
public:
    NativeThreadImpl(TObject* tObject, Context* context)
    : tObject_(tObject)
    , context_(context)
    , callback_(nullptr)
    , stopped_(false)
    {
        lock_.lock();
        future_ = async(launch::async, runTObject, this);
    }

    ~NativeThreadImpl()
    {
    }

    TObject* tObject_;
    aft::base::Context* context_;
    aft::base::Callback* callback_;
    std::future<Result> future_;
    std::mutex lock_;
    bool stopped_;
    //TODO store a Result here for TObject vs thread result
};

// Run the tobject from within the thread
Result runTObject(void* implData)
{
    NativeThreadImpl& impl = *(NativeThreadImpl *)implData;
    // wait for locked mutex to start
    impl.lock_.lock();

    //TODO check stopped_

    Result result(false);
    if (!impl.stopped_)
    {
        result = impl.tObject_->run(impl.context_);
    }

    if (impl.callback_)
    {
        impl.callback_->callback(&result);
    }
    
    impl.tObject_->setState(TObject::FINISHED_GOOD);
    impl.lock_.unlock();
    return result;
}
    

NativeThreadHandler::NativeThreadHandler(TObject* tObject, Context* context)
    : impl_(*new NativeThreadImpl(tObject, context))
    , result_(Result(false))
{
}

NativeThreadHandler::~NativeThreadHandler()
{
    stop(true);
    delete &impl_;
}

Result NativeThreadHandler::getResult()
{
    result_ = impl_.tObject_->getResult();
    return result_;
}

TObject::State
NativeThreadHandler::getState()
{
    return impl_.tObject_->getState();
}

TObject* NativeThreadHandler::getTObject() const
{
    return impl_.tObject_;
}

void NativeThreadHandler::notify(Callback* callback)
{
    impl_.callback_ = callback;
}

bool NativeThreadHandler::unnotify(Callback* callback)
{
    impl_.callback_ = 0;
    return true;
}

void NativeThreadHandler::stop(bool force)
{
    // NOTE: No way to kill threads in native c++14 (if force)
    impl_.stopped_ = true;
}

Result NativeThreadHandler::wait()
{
    result_ = impl_.future_.get();
    return result_;
}

void NativeThreadHandler::run()
{
    if (impl_.stopped_)
    {
        result_ = Result(false);
    }
    else
    {
        result_ = Result(true);
        impl_.lock_.unlock();       // kick off thread
    }
}

} // namespace osdep
} // namespace aft
