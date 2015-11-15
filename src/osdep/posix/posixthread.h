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

class PosixThreadHandler : public aft::base::ThreadHandler
{
public:
    PosixThreadHandler(aft::base::TObject* tObject, aft::base::Context* context = 0);
    virtual ~PosixThreadHandler();

    virtual aft::base::Result getResult();
    virtual aft::base::TObject::State getState();
    virtual void notify(aft::base::Callback* callback);
    virtual bool unnotify(aft::base::Callback* callback);
    virtual void stop(bool force);
    virtual aft::base::Result wait();
    virtual void run();

private:
    PosixThreadImpl& impl_;
    aft::base::Result result_;
};

} // namespace osdep
} // namespace aft
