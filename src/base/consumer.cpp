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

#include <algorithm>

#include "consumer.h"

using namespace aft::base;

bool ConsumerContract::registerWriteCallback(const WriterContract* writer)
{
    if (!writer) return false;

    std::vector<WriterContract*>::iterator it
        = std::find(writers_.begin(), writers_.end(), writer);
    if (it != writers_.end())
    {
        return false;   // writer already in callback list
    }
    writers_.push_back(const_cast<WriterContract*>(writer));
    return true;
}

bool ConsumerContract::unregisterWriteCallback(const WriterContract* writer)
{
    if (!writer) return false;

    std::vector<WriterContract*>::iterator it
        = std::find(writers_.begin(), writers_.end(), writer);
    if (it == writers_.end())
    {
        return false;   // writer not in callback list
    }
    writers_.erase(it);
    return true;
}
