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
#include "base/thread.h"


namespace aft
{
namespace osdep
{
// Forward reference
class PosixThreadImpl;

/**
 *  Thread handler implemented using posix threads (pthreads).
 *  @copydoc aft::base::ThreadHandler
 */
class PosixThreadHandler : public aft::base::ThreadHandler
{
public:
    /** Construct a PosixThreadHandler.
     *  @param tObject object that will be run in the thread
     *  @param context context used to run the object
     */
    PosixThreadHandler(aft::base::TObject* tObject, aft::base::Context* context = 0);
    /** Destruct a PosixThreadHandler */
    virtual ~PosixThreadHandler();

    virtual base::Result getResult();
    virtual base::TObject::State getState();
    virtual base::TObject* getTObject() const;
    virtual void notify(base::Callback* callback);
    virtual bool unnotify(base::Callback* callback);
    virtual void stop(bool force);
    virtual base::Result wait();
    virtual void run();

private:
    PosixThreadImpl& impl_;
    aft::base::Result result_;
};

} // namespace osdep
} // namespace aft
