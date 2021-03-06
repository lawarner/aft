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

namespace aft
{
namespace base
{
// Forward reference
class Result;

/** Define an old C style callback for Result */
typedef void (*CCallback)(Result *result);


/**
 *  Standard Callback interface used to deliver a Result, typically from a different
 *  thread.
 */
class CallbackContract
{
public:
    virtual void callback(Result *result) = 0;
};

/**
 *  Default no-op implementation for CallbackContract.
 */
class Callback : public CallbackContract
{
public:
    virtual void callback(Result *result) { }
};

} // namespace base
} // namespace aft
