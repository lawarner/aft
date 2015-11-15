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

#include <vector>
#include "context.h"
#include "thread.h"
#include "osdep/platform.h"

using namespace aft::base;
using namespace aft::osdep;
using namespace std;

ThreadManager* ThreadManager::instance_ = 0;


class aft::base::ThreadManagerImpl
{
public:
    vector<ThreadHandler*> threads;
};


ThreadManager::ThreadManager(Context* context)
    : impl_(*new ThreadManagerImpl)
{
}

ThreadManager::~ThreadManager()
{
    stopAll();
    delete &impl_;
}

ThreadManager* ThreadManager::instance()
{
    if (!instance_)
    {
        instance_ = new ThreadManager;
    }
    return instance_;
}

void ThreadManager::stopAll(bool force)
{
    vector<ThreadHandler*>::iterator it;
    for (it = impl_.threads.begin(); it != impl_.threads.end(); ++it)
    {
        (*it)->stop(force);
    }
}

ThreadHandler* ThreadManager::thread(TObject* tObject, Context* context)
{
    ThreadHandler* threadHandler = new PlatformThreadHandler(tObject, context);
    impl_.threads.push_back(threadHandler);
    return threadHandler;
}

