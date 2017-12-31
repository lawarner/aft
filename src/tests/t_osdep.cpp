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
#include <base/callback.h>
#include <base/context.h>
#include <base/thread.h>
#include <base/tobasictypes.h>
#include <base/tobject.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace std;


class SampleContext : public Context
{
public:
    SampleContext() { }
};

class SampleCallback : public Callback
{
    virtual void callback(Result *result)
    {
        std::cout << "Thread notified with result: " << result->getType() << std::endl;
    }
};


namespace {

TEST(OsdepPackageTest, ThreadHandler) {
    ThreadManager* tman = ThreadManager::instance();
    EXPECT_TRUE(tman != nullptr);

    SampleContext context;
    ThreadHandler* aThread = tman->thread(&TOTrue, &context);
    EXPECT_TRUE(aThread != nullptr);

    TObject::State toState = aThread->getState();
    EXPECT_EQ(toState, TObject::INITIAL);

    SampleCallback callback;
    aThread->notify(&callback);
    aThread->run();
    aThread->wait();
    toState = aThread->getState();
    std::cout << "Final state of thread: " << toState << std::endl;
    EXPECT_EQ(TObject::FINISHED_GOOD, toState);
}

TEST(OsdepPackageTest, MultiThreads)
{
    ThreadManager* tman = ThreadManager::instance();
    EXPECT_TRUE(tman != nullptr);

    SampleContext context;
    SampleCallback callback;
    const int numThreads = 10;
    ThreadHandler* threads[numThreads];
    for (ThreadHandler*& thread : threads) {
        thread = tman->thread(&TOTrue, &context);
        EXPECT_TRUE(thread != nullptr);
        cout << "Created thread " << thread << endl;
        thread->notify(&callback);

    }

    for (ThreadHandler*& thread : threads) {
        cout << "Start thread " << thread << endl;
        thread->run();
    }
    
    for (ThreadHandler*& thread : threads) {
        cout << "Wait for thread " << thread << endl;
        thread->wait();
    }
    
    for (ThreadHandler*& thread : threads) {
        cout << "Remove thread " << thread << endl;
        delete thread;
    }
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

