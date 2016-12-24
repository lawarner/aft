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
#include "result.h"
#include "tobject.h"


namespace aft
{
namespace base
{
// Forward reference
class Callback;
class Context;
class ThreadManagerImpl;


/**
 *  Handle running a process in a thread.
 *
 *  Methods are provided to query, stop, wait or get the intermediate or final
 *  result.
 */
class ThreadHandler
{
public:
    virtual ~ThreadHandler();

    /**
     *  Get the result of the tobject running in this thread.  If the thread has
     *  finished, this gets the final result.  If the thread is still running, 
     *  the some tobjects may return intermediate results (but this is not
     *  guaranteed).
     *
     *  @return The current result of running thread (if any) or the final result
     *          for an ended thread.
     */
    virtual Result getResult() = 0;
    /** Get the current state of the thread. */
    virtual TObject::State getState() = 0;

    virtual TObject* getTObject() const = 0;

    /**
     *  Register a callback to receive notifications from the thread.
     *
     *  Note that the callback will always be notified when a thread has completed.
     *  Other notifications are possible, but depend on the implementation of the
     *  TObject.
     *
     *  @param callback Callback instance to call to deliver notifications.
     */
    virtual void notify(Callback* callback) = 0;
    /**
     *  Unregister a callback for receiving notifications from this thread.
     *
     *  @param callback Callback instance that previously registered for notifications.
     *  @return true if the callback was succesfully unregistered, otherwise false.
     */
    virtual bool unnotify(Callback* callback) = 0;

    /**
     *  Start the thread running.
     *
     *  Note that some implementations create/run the thread at the same time.  In those
     *  cases, calling run() has not effect.
     */
    virtual void run() = 0;
    /**
     *  Stop a running thread.
     *
     *  @param force if true then the thread will be forcibly terminated.
     */
    virtual void stop(bool force = false) = 0;

    /** Wait for this thread to exit. */
    virtual Result wait() = 0;
};


/**
 *  Single place to create threads from other parts of the library.
 *
 *  Internally, this class uses the osdep mechanism to instantiate the
 *  correct ThreadHandler sub-class.
 */
class ThreadManager
{
public:
    /** Get the single ThreadManager instance. */
    static ThreadManager* instance();

    /**
     *  Find the thread handler running a given TObject.
     *  @param tObject find ThreadHandler related to TObject
     *  @return pointer to the TObject's ThreadHandler or 0 if non found.
     */
    ThreadHandler* find(const TObject* tObject) const;

    /**
     *  Stop all running threads.
     *
     *  Call stop on all ThreadHandlers
     *  @param force if true then the threads will be forcibly terminated.
     */
    /** Call stop on all ThreadHandlers */
    void stopAll(bool force = false);

    /** start a thread to run a tObject in.
     *
     *  @param tObject the TObject that will run in the thread.
     *  @param context the context used to run the TObject.
     *  @return a thread handler to track the thread.  Returns 0 if an error occurred
     *          creating the thread.
     */
    ThreadHandler* thread(TObject* tObject, Context* context = 0);

private:
    ThreadManager(Context* context = 0);
    virtual ~ThreadManager();

private:
    ThreadManagerImpl& impl_;
    static ThreadManager* instance_;
    friend ThreadHandler;
};

} // namespace base
} // namespace aft
