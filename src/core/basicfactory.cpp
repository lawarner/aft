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

#include "base/blob.h"
#include "base/context.h"
#include "base/structureddata.h"
#include "basiccommands.h"
#include "basicfactory.h"
#include "logger.h"
using namespace aft;
using namespace aft::core;


static base::TObject* builtinCommand(const std::string& command,
                                     const std::vector<std::string>& parameters)
{
    base::Command* tobj = 0;
    if (command == "Log")
    {
        tobj = new LogCommand(parameters[0]);
    }
    else if (command == "Cons")
    {
        tobj = new ConsCommand(parameters[0],
                               parameters.size() > 1 ? parameters[1] : "");
        if (parameters.size() > 2)
        {
            base::Blob blobData("", base::Blob::STRING, parameters[2]);
            tobj->setup(0, &blobData);
        }
    }
    else if (command == "Prod")
    {
        tobj = new ProdCommand(parameters[0],
                               parameters.size() > 1 ? parameters[1] : "");
        if (parameters.size() > 2)
        {
            base::Blob blobData("", base::Blob::STRING, parameters[2]);
            tobj->setup(0, &blobData);
        }
    }

    return tobj;
}

BasicCommandFactory::BasicCommandFactory()
    : BaseFactory("Command", "BasicCommands")
{
}

BasicCommandFactory::~BasicCommandFactory()
{
}

base::TObject*
BasicCommandFactory::construct(const std::string& name, const base::Blob* blob,
                               const base::Context* context)
{
    if (!blob) return 0;

    base::TObject* retval = 0;
    switch (blob->getType())
    {
    case base::Blob::COMMAND:
        retval = (base::TObject *) blob->getData();
        break;
    case base::Blob::JSON:
        //TODO general json parser
        break;
    case base::Blob::STRING:
    {
        base::StructuredData sd("");
        if (!sd.deserialize(*blob)) break;

        std::vector<std::string> parameters;
        sd.getArray("parameters", parameters);
        retval = builtinCommand(sd.get("name"), parameters);
    }
        break;
    default:
        break;
    }

    return retval;
}

