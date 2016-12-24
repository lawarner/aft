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

#include <algorithm>
#include <mutex>
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
    void addThread(ThreadHandler* thread)
    {
        unique_lock<mutex> lck(mutex_);
        threads_.push_back(thread);
    }

    ThreadHandler* findThread(const TObject* tObject)
    {
        unique_lock<mutex> lck(mutex_);
        for (auto* thread : threads_) {
            if (thread->getTObject() == tObject) {
                return thread;
            }
        }
        
        return nullptr;
    }

    void removeThread(ThreadHandler* thread)
    {
        unique_lock<mutex> lck(mutex_);
        auto it = find(threads_.begin(), threads_.end(), thread);
        if (it != threads_.end())
        {
            threads_.erase(it);
        }
    }

    void stopAll(bool force)
    {
        unique_lock<mutex> lck(mutex_);
        for (auto* thread : threads_) {
            thread->stop(force);
        }
    }

private:
    vector<ThreadHandler*> threads_;
    mutex mutex_;
};


ThreadHandler::~ThreadHandler()
{
    ThreadManager* threadMan = ThreadManager::instance();
    threadMan->impl_.removeThread(this);
}

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

ThreadHandler* ThreadManager::find(const TObject* tObject) const
{
    return impl_.findThread(tObject);
}

void ThreadManager::stopAll(bool force)
{
    impl_.stopAll(force);
}

ThreadHandler* ThreadManager::thread(TObject* tObject, Context* context)
{
    ThreadHandler* threadHandler = new PlatformThreadHandler(tObject, context);
    impl_.addThread(threadHandler);

    return threadHandler;
}

