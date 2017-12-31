/*
 *   Copyright © 2015-2017 Andy Warner
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

// scenario: create objects (testcase, commands), run, serialize to file, deserialize and run again.
/* File contents is:
 { "commands":[{"name":"Log","parameters":["This is the first message.",""]},
               {"name":"Cons","parameters":["openw","/tmp/sc1-file.txt"]},
               {"name":"Cons","parameters":["write","/tmp/sc1-file.txt","Hello World!\n"]},
               {"name":"Cons","parameters":["close","/tmp/sc1-file.txt"]},
               {"name":"Prod","parameters":["open","/tmp/sc1-file.txt"]},
               {"name":"Prod","parameters":["read","/tmp/sc1-file.txt"]},
               {"name":"Log","parameters":["","result"]},
               {"name":"Prod","parameters":["close","/tmp/sc1-file.txt"]},
               {"name":"Log","parameters":["This is the last message.",""]}],
   "name":"Scenario 1",
   "result":"true",
   "state":1,
   "type":"TestCase"}

  Scenario 2 will also have for loop, if-then-else and group commands.
  Scenario 3 will start adding entities and dependencies.
  Scenario 4 will be a Test Suite containing the testcases from Scenarios 1-3.
 */

#include <string>

#include <base/blob.h>
#include <base/factory.h>
#include <core/basiccommands.h>
#include <core/basicfactory.h>
#include <core/fileconsumer.h>
#include <core/fileproducer.h>
#include <core/logger.h>
#include <core/runcontext.h>
#include <core/testcase.h>
using namespace aft::base;
using namespace aft::core;
using std::endl;

bool initMec()
{
    MecFactory* mec = MecFactory::instance();
    mec->addFactory(new BasicCommandFactory);

    return true;
}

int main(int argc, char* argv[]) {
    aftlog << "Starting Scenario 1: " << argv[0] << std::endl;
    if (!initMec()) {
        aftlog << loglevel(Error) << "Cannot initialize the MEC." << std::endl;
        return 1;
    }

    TestCase testCase("Scenario 1");
    const std::string consumerName("/tmp/sc1-file.txt");
    
    testCase.add(new LogCommand("This is the first message."));
    testCase.add(new ConsCommand("openw", consumerName));
    ConsCommand* writeCmd = new ConsCommand("write", consumerName);
    Blob blobData("", Blob::STRING, "Hello World!\n");
    writeCmd->setup(nullptr, &blobData);
    testCase.add(writeCmd);
    testCase.add(new ConsCommand("close", consumerName));
    testCase.add(new ProdCommand("open", consumerName));
    testCase.add(new ProdCommand("read", consumerName));
    testCase.add(new LogCommand("", "result"));
    testCase.add(new ProdCommand("close", consumerName));
    
    testCase.add(new LogCommand("This is the last message."));

    // First run
    aftlog << "\n==================== Running initial testcase: "<< testCase.getName() << endl;
    RunContext context("Run Context", &testCase);
    testCase.open();
    testCase.run(&context);
    testCase.close();

    Blob blob("");
    if (testCase.serialize(blob)) {
        FileConsumer filecons("/tmp/scenario1.aft", true);
        if (!filecons.canAcceptData()) {
            aftlog << loglevel(Error) << "Cannot write to testcase file" << endl;
            return 1;
        }

        filecons.write(blob);
    } else {
        aftlog << loglevel(Error) << "Cannot serialize testcase" << endl;
        return 2;
    }

    FileProducer fileprod("/tmp/scenario1.aft");
    if (!fileprod.hasData()) {
        aftlog << loglevel(Error) << "Cannot open testcase file" << endl;
        return 3;
    }
    if (fileprod.read(blob)) {
        TestCase tc2;
        if (!tc2.deserialize(blob)) {
            aftlog << loglevel(Error) << "Cannot deserialize testcase" << endl;
            return 4;
        }
        aftlog << "\n==================== Running testcase from file: " << tc2.getName() << endl;
        RunContext context2("Run Context 2", &tc2);
        tc2.open();
        Result result = tc2.run(&context2);
        tc2.close();
    } else {
        aftlog << loglevel(Error) << "Cannot read testcase file" << endl;
        return 5;
    }

    aftlog << "\n==================== Finished Scenario 1" << endl;

    return 0;
}
