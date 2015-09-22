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

#include "producer.h"

using namespace aft::base;

bool ProducerContract::registerDataCallback(const ReaderContract* reader)
{
    if (!reader) return false;

    std::vector<ReaderContract*>::iterator it
        = std::find(readers_.begin(), readers_.end(), reader);
    if (it != readers_.end())
    {
        return false;   // reader already in callback list
    }
    readers_.push_back(const_cast<ReaderContract*>(reader));
    return true;
}

bool ProducerContract::unregisterDataCallback(const ReaderContract* reader)
{
    if (!reader) return false;

    std::vector<ReaderContract*>::iterator it
        = std::find(readers_.begin(), readers_.end(), reader);
    if (it == readers_.end())
    {
        return false;   // reader not in callback list
    }
    readers_.erase(it);
    return true;
}
