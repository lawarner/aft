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

#include <string>

#include <base/blob.h>
#include <core/basiccommands.h>
#include <core/commandcontext.h>
#include <core/fileconsumer.h>
#include <core/fileproducer.h>
#include <core/outlet.h>
#include <core/queueproc.h>
#include <core/stringconsumer.h>
#include <core/stringproducer.h>
#include <gtest/gtest.h>
using namespace aft::base;
using namespace aft::core;
using std::cout;
using std::endl;
using std::string;


const std::string sampleText("This is just sample text that you can safely ignore.\n");
const std::string sampleWords[]{"This", "is", "just", "sample", "text", "that",
    "you", "can", "safely", "ignore."};

class ObjectReader : public ReaderContract {
public:
    virtual bool pushData(const TObject& object) override
    {
        return false;
    }
    virtual bool pushData(const Result& result) override
    {
        return false;
    }
    virtual bool pushData(const Blob& blob) override
    {
        std::cout << "ObjectReader blob:  " << blob.getString() << endl;
        return true;
    }
    virtual bool roomForData() const override
    {
        return true;
    }
    virtual bool roomForObject(ProductType productType) const override
    {
        if (roomForData() && productType == ProductType::BLOB)
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
    EXPECT_TRUE(strcons.canAcceptData());
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

    StringProducer wordprod(sampleText, ParcelType::BLOB_WORD);
    size_t words = 0;
    while (wordprod.read(blob)) {
        EXPECT_EQ(sampleWords[words++], blob.getString());
    }
    constexpr size_t numWords = sizeof(sampleWords) / sizeof(sampleWords[0]);
    EXPECT_EQ(numWords, words);
}

TEST(CorePackageTest, FileConsumer) {
    Blob blob("file", Blob::STRING, sampleText);
    FileConsumer filecons("/tmp/test_file_cons", true);
    EXPECT_TRUE(filecons.canAcceptData());
    filecons.write(blob);
}

// This test assumes it runs after FileConsumer test
TEST(CorePackageTest, FileProducer) {
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
    EXPECT_TRUE(fileprod.hasObject(ProductType::BLOB));
    EXPECT_FALSE(fileprod.hasObject(ProductType::NONE));

    ObjectReader reader;
    EXPECT_TRUE(fileprod.registerDataCallback(&reader));
    fileprod.flowData();
    EXPECT_TRUE(fileprod.unregisterDataCallback(&reader));
}

TEST(CorePackageTest, FileProducerFlowWords)
{
    FileProducer fileprod("/tmp/test_file_cons", ParcelType::BLOB_WORD);
    EXPECT_TRUE(fileprod.hasData());
    EXPECT_TRUE(fileprod.hasObject(ProductType::BLOB));

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
        EXPECT_TRUE(qproc.canAcceptData());
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
    //TODO more getArgument tests
}

TEST(CorePackageTest, BasicCommands) {
    LogCommand logCmd("This is a logged message.");
    logCmd.setState(TObject::PREPARED);
    Result value = logCmd.run();
    std::cout << "-- result type: " << value.getTypeName() << std::endl;
    EXPECT_TRUE(value);
    
    EnvCommand setEnvCmd("set", "ABC", sampleText);
    setEnvCmd.setState(TObject::PREPARED);
    value = setEnvCmd.run();
    EXPECT_TRUE(value);

    EnvCommand getEnvCmd("get", "ABC");
    getEnvCmd.setState(TObject::PREPARED);
    value = getEnvCmd.run();
    EXPECT_EQ(value.asString(), sampleText);
    
    EnvCommand setXyz("set", "XYZ", "xyz");
    setXyz.setState(TObject::PREPARED);
    value = setXyz.run();
    EXPECT_TRUE(value);
    
    EnvCommand listEnv("list", "");
    listEnv.setState(TObject::PREPARED);
    value = listEnv.run();
    EXPECT_EQ("ABC=" + sampleText + "\nXYZ=xyz\n", value.asString());

    LogCommand logResult("", "result");
    logResult.setState(TObject::PREPARED);
    value = logResult.run();
    EXPECT_TRUE(value);
}

TEST(CorePackageTest, Outlet) {
    const string outletName("Test outlet");
    Outlet outlet(outletName);
    EXPECT_EQ(outletName, outlet.name());
    EXPECT_EQ(OutletType::None, outlet.type());

    const string sampleText("This is just some sample text.");
    StringProducer strprod(sampleText);
    EXPECT_TRUE(outlet.plugin(&strprod));
    EXPECT_EQ(OutletType::In, outlet.type());
    EXPECT_TRUE(outlet.hasData());

    Blob blob("");
    EXPECT_TRUE(outlet.read(blob));
    EXPECT_EQ(sampleText, blob.getString());

    EXPECT_TRUE(outlet.unplug(&strprod));
    EXPECT_EQ(OutletType::None, outlet.type());

    StringConsumer strcons;
    EXPECT_TRUE(outlet.plugin(&strcons));
    EXPECT_EQ(OutletType::Out, outlet.type());
    EXPECT_TRUE(outlet.canAcceptData());
    EXPECT_TRUE(outlet.write(blob));
    EXPECT_EQ(sampleText, strcons.getContents());

    EXPECT_FALSE(outlet.unplug(&strprod));
    EXPECT_EQ(OutletType::Out, outlet.type());
    EXPECT_TRUE(outlet.unplug(&strcons));
    EXPECT_EQ(OutletType::None, outlet.type());

    QueueProc qproc;
    EXPECT_TRUE(outlet.plugin(&qproc));
    EXPECT_EQ(OutletType::InOut, outlet.type());
    EXPECT_TRUE(outlet.write(blob));
    Blob blob1("");
    EXPECT_TRUE(outlet.read(blob1));
    EXPECT_EQ(sampleText, blob1.getString());
    Blob blobName("name", Blob::STRING, outletName);
    EXPECT_TRUE(outlet.write(blobName));
    EXPECT_TRUE(outlet.read(blob1));
    EXPECT_EQ(outletName, blob1.getString());

    EXPECT_FALSE(outlet.read(blob1));
    EXPECT_TRUE(outlet.unplug(&qproc));
    EXPECT_EQ(OutletType::None, outlet.type());

    EXPECT_TRUE(outlet.plugin(&strprod));
    EXPECT_EQ(OutletType::In, outlet.type());
    EXPECT_TRUE(outlet.unplug());
    EXPECT_EQ(OutletType::None, outlet.type());
    EXPECT_TRUE(outlet.plugin(&strcons));
    EXPECT_EQ(OutletType::Out, outlet.type());
    EXPECT_TRUE(outlet.unplug());
    EXPECT_EQ(OutletType::None, outlet.type());
    EXPECT_TRUE(outlet.plugin(&qproc));
    EXPECT_EQ(OutletType::InOut, outlet.type());
    EXPECT_TRUE(outlet.unplug());
    EXPECT_EQ(OutletType::None, outlet.type());
#if 0
        // An Entity can be at TObject type level, or an actual named TObject.
        const EntityList& consumes() const;
        const EntityList& provides() const;
        const EntityList& requires() const;
        void consumes(const EntityList& entities);
        void provides(const EntityList& entities);
        void requires(const EntityList& entities);
        // Implement ProcContract (ProviderContract)
        virtual Result read(TObject& object) override;
        virtual Result read(Result& result) override;
        virtual bool hasObject(ProductType productType) override;
        // Implement ProcContract (ConsumerContract)
        virtual Result write(const TObject& object) override;
        virtual Result write(const Result& result) override;
#endif
    }
    

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

