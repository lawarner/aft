/*
 *   Copyright 2015, 2016 Andy Warner
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

#include <string>

#include <base/blob.h>
#include <core/basiccommands.h>
#include <core/commandcontext.h>
#include <core/fileconsumer.h>
#include <core/fileproducer.h>
#include <core/queueproc.h>
#include <core/stringconsumer.h>
#include <core/stringproducer.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;


const std::string sampleText("This is just sample text that you can safely ignore.\n");

class ObjectReader : public ReaderContract
{
public:
    virtual bool dataAvailable(const TObject& object)
        {
            return false;
        }
    virtual bool dataAvailable(const Result& result)
        {
            return false;
        }
    virtual bool dataAvailable(const Blob& blob)
        {
            std::cout << "ObjectReader blob:  " << blob.getString() << std::endl;
            return true;
        }
    virtual bool roomForData() const
    {
        return true;
    }
    virtual bool roomForObject(ProductType productType) const
    {
        if (roomForData() && productType == TYPE_BLOB)
        {
            return true;
        }
        return false;
    }
};

namespace
{

TEST(CorePackageTest, StringConsumer)
{
    Blob blob("string", Blob::STRING, sampleText);
    StringConsumer strcons;
    EXPECT_TRUE(strcons.needsData());
    strcons.write(blob);
    EXPECT_TRUE(strcons.getContents() == sampleText);
}

TEST(CorePackageTest, StringProducer)
{
    StringProducer strprod(sampleText);
    EXPECT_TRUE(strprod.hasData());
    Blob blob("");
    EXPECT_TRUE(strprod.read(blob));
    EXPECT_TRUE((blob.getString() == sampleText));
    std::cout << "string content: " << blob.getString() << std::endl;

    StringProducer wordprod(sampleText, PARCEL_BLOB_WORD);
    while (wordprod.read(blob))
    {
        std::cout << "word content: " << blob.getString() << std::endl;
    }
}

TEST(CorePackageTest, FileConsumer)
{
    Blob blob("file", Blob::STRING, sampleText);
    FileConsumer filecons("/tmp/test_file_cons", true);
    EXPECT_TRUE(filecons.needsData());
    filecons.write(blob);
}

// This test assumes it runs after FileConsumer test
TEST(CorePackageTest, FileProducer)
{
    FileProducer fileprod("/tmp/test_file_cons");
    EXPECT_TRUE(fileprod.hasData());
    Blob blob("");
    EXPECT_TRUE(fileprod.read(blob));
    EXPECT_TRUE((blob.getString() == sampleText));
    std::cout << "file content: " << blob.getString() << std::endl;
}

TEST(CorePackageTest, FileProducerFlow)
{
    FileProducer fileprod("/tmp/test_file_cons");
    EXPECT_TRUE(fileprod.hasData());
    EXPECT_TRUE(fileprod.hasObject(TYPE_BLOB));
    EXPECT_FALSE(fileprod.hasObject(TYPE_NONE));

    ObjectReader reader;
    EXPECT_TRUE(fileprod.registerDataCallback(&reader));
    fileprod.flowData();
    EXPECT_TRUE(fileprod.unregisterDataCallback(&reader));
}

TEST(CorePackageTest, FileProducerFlowWords)
{
    FileProducer fileprod("/tmp/test_file_cons", PARCEL_BLOB_WORD);
    EXPECT_TRUE(fileprod.hasData());
    EXPECT_TRUE(fileprod.hasObject(TYPE_BLOB));

    ObjectReader reader;
    EXPECT_TRUE(fileprod.registerDataCallback(&reader));
    fileprod.flowData();
    EXPECT_TRUE(fileprod.unregisterDataCallback(&reader));
}

TEST(CorePackageTest, QueueProc)
{
    QueueProc qproc;

    for (int idx = 0; idx < 3; ++idx)
    {
        EXPECT_TRUE(qproc.needsData());
        Blob blob("b", Blob::STRING, "This is a string of sorts.");
        EXPECT_TRUE(qproc.write(blob));
    }
    
    while (qproc.hasData())
    {
        Blob blob("");
        EXPECT_TRUE(qproc.read(blob));
        std::cout << " Blob from Q: " << blob.getString() << std::endl;
    }
}
    
TEST(CorePackageTest, CommandContext)
{
    const std::string COMMAND("Open");
    const std::string ARGNAME("url");
    const std::string URL("http://localhost:8080");
    CommandContext ctx;
    EXPECT_TRUE(ctx.setArgument(COMMAND, ARGNAME, URL));

    std::string url = ctx.getArgument(COMMAND, ARGNAME);
    EXPECT_EQ(URL, url);
}

TEST(CorePackageTest, BasicCommands)
{
    LogCommand logCmd("This is a logged message.");
    logCmd.setState(TObject::PREPARED);
    logCmd.run();

    EnvCommand setEnvCmd("set", "ABC", sampleText);
    setEnvCmd.setState(TObject::PREPARED);
    setEnvCmd.run();

    EnvCommand getEnvCmd("get", "ABC");
    getEnvCmd.setState(TObject::PREPARED);
    Result value = getEnvCmd.run();
    LogCommand logResult("", "result");
    logResult.setState(TObject::PREPARED);
    logResult.run();
    EXPECT_EQ(value.asString(), sampleText);
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

