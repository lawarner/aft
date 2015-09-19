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

#include <base/result.h>
#include <core/logger.h>
using namespace aft::base;
using namespace aft::core;

int main(int argc, const char* argv[])
{
    aftlog << "Test Result object" << std::endl;

    Result testResult;	// defaults to BOOLEAN
    testResult.setValue(true);
    bool value;
    if (testResult.getValue(value))
    {
        aftlog << "Got value, true == " << std::boolalpha << value << std::endl;
    } else {
        aftlog << Error << "Unable to get value from Result" << std::endl;
    }

    return 0;
}
