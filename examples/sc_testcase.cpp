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

// scenario: create objects (testcase), serialize, deserialize and run.

#include <string>

#include <base/blob.h>
#include <base/factory.h>
#include <core/basiccommands.h>
#include <core/basicfactory.h>
#include <core/fileconsumer.h>
#include <core/fileproducer.h>
#include <core/logger.h>
#include <core/testcase.h>
using namespace aft::base;
using namespace aft::core;


bool initMec()
{
    MecFactory* mec = MecFactory::instance();
    mec->addFactory(new BasicCommandFactory);

    return true;
}

int main(int argc, char* argv[])
{
    aftlog << "Starting Scenario 1: " << argv[0] << std::endl;
    if (!initMec())
    {
        aftlog << loglevel(Error) << "Cannot initialize the MEC." << std::endl;
        return 1;
    }

    TestCase testCase("Scenario 1");
    testCase.add(new LogCommand("This is the first message."));
    testCase.add(new LogCommand("This is the last message."));

    Blob blob("");
    if (testCase.serialize(blob))
    {
        FileConsumer filecons("/tmp/scenario1.aft", true);
        if (!filecons.needsData())
        {
            aftlog << loglevel(Error) << "Cannot write to testcase file" << std::endl;
            return 1;
        }

        filecons.write(blob);
    }
    else
    {
        aftlog << loglevel(Error) << "Cannot serialize testcase" << std::endl;
        return 2;
    }

    FileProducer fileprod("/tmp/scenario1.aft");
    if (!fileprod.hasData())
    {
        aftlog << loglevel(Error) << "Cannot open testcase file" << std::endl;
        return 3;
    }
    if (fileprod.read(blob))
    {
        TestCase tc2;
        if (!tc2.deserialize(blob))
        {
            aftlog << loglevel(Error) << "Cannot deserialize testcase" << std::endl;
            return 4;
        }
        aftlog << "Start of testcase: " << tc2.getName() << std::endl;
        tc2.open();
        Result result = tc2.run(0);
        tc2.close();
    } else {
        aftlog << loglevel(Error) << "Cannot read testcase file" << std::endl;
        return 5;
    }

    aftlog << "Finished Scenario 1" << std::endl;

    return 0;
}

