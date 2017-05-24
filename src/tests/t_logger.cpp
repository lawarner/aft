/*
 *   Copyright 2015-2017 Andy Warner
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

#include <unistd.h>

#include <core/logger.h>
#include <gtest/gtest.h>
using namespace aft;
using core::aftlog;
using std::cout;
using std::endl;

namespace {

TEST(LoggerTest, TerminalLog)
{
    char* currDir = getcwd(0, 0);
    cout << "Current Directory is " << currDir << endl;

    ASSERT_TRUE(core::aftlog.good());
    aftlog << "This is a log message." << endl;
    ASSERT_TRUE(core::aftlog.good());
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
