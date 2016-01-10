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

#include <vector>

#include "base/blob.h"
#include "base/context.h"
#include "base/factory.h"
#include "base/result.h"
#include "base/structureddata.h"
#include "base/tobjecttree.h"
#include "core/logger.h"
#include "testcase.h"
using namespace aft;
using namespace aft::core;


TestCase::TestCase(const std::string& name)
    : TObjectContainer(name)
{
    state_ = INITIAL;
}

TestCase::~TestCase()
{
}

bool
TestCase::open()
{
    if (state_ == INITIAL)
    {
        state_ = PREPARED;
        return true;
    }

    return false;
}

bool
TestCase::rewind(base::Context* context)
{
    return base::TObjectContainer::rewind(context);
}

const base::Result
TestCase::run(base::Context* context)
{
    if (state_ != PREPARED)
    {
        return base::Result(false);
    }

    return base::TObjectContainer::run(context);
}

void
TestCase::close()
{
    //TODO check other states, i.e., if running then stop.
    if (state_ != INVALID && state_ != UNINITIALIZED)
    {
        state_ = INITIAL;
    }
}

bool TestCase::serialize(base::Blob& blob)
{
    base::StructuredData sd("TestCase");

    sd.add("name", getName());
    sd.addArray("commands");

    base::TObjectTree::Children& cmds = children_->getChildren();
    base::TObjectTree::Children::iterator it;
    for (it = cmds.begin(); it != cmds.end(); ++it)
    {
        TObject* tObj = (*it)->getValue();
        if (tObj)
        {
            base::Blob cmdBlob("");
            tObj->serialize(cmdBlob);
            base::StructuredData sdcmd("command", cmdBlob.getString());
//            base::StructuredData sdcmd("command");
//            sdcmd.add(base::StructuredDataName(tObj->getName()), "args, ...");
            sd.add("commands.", sdcmd);
        }
    }

    return sd.serialize(blob);
}

bool TestCase::deserialize(const base::Blob& blob)
{
    base::StructuredData sd("");
    if (!sd.deserialize(blob)) return false;

    std::string name;
    if (sd.get("name", name)) setName(name);

    // commands
    base::MecFactory* mec = base::MecFactory::instance();
    std::vector<std::string> cmds;
    if (!sd.getArray("commands", cmds))
    {
        return false;
    }

    const std::string category("Command");
    std::vector<std::string>::const_iterator it;
    for (it = cmds.begin(); it != cmds.end(); ++it)
    {
        base::StructuredData sdParams("", *it);
        std::string cmdName = sdParams.get("name");
        base::Blob params(cmdName, base::Blob::STRING, *it);
        base::TObject* tobj = mec->construct(category, cmdName, &params);
        if (!tobj)
        {
            aftlog << loglevel(Error) << "Cannot construct object" << std::endl;
            return false;
        }
        add(tobj);
    }

    return true;
}
