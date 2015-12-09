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

#include <string>

#include <base/typedblob.h>
#include <core/fileconsumer.h>
#include <core/fileproducer.h>
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
            const TypedBlob& strBlob = dynamic_cast<const TypedBlob&>(blob);
            std::cout << "ObjectReader blob:  " << strBlob.getString() << std::endl;
            return true;
        }
};

namespace
{

TEST(CorePackageTest, FileConsumer)
{
    TypedBlob blob("file", TypedBlob::STRING, sampleText);
    FileConsumer filecons("/tmp/test_file_cons", true);
    EXPECT_TRUE(filecons.needsData());
    filecons.write(blob);
}

// This test assumes it runs after FileConsumer test
TEST(CorePackageTest, FileProducer)
{
    FileProducer fileprod("/tmp/test_file_cons");
    EXPECT_TRUE(fileprod.hasData());
    TypedBlob blob("");
    EXPECT_TRUE(fileprod.read(blob));
    EXPECT_TRUE((blob.getString() == sampleText));
    std::cout << "file content: " << blob.getString() << std::endl;
}

TEST(CorePackageTest, FileProducerFlow)
{
    FileProducer fileprod("/tmp/test_file_cons");
    EXPECT_TRUE(fileprod.hasData());
    EXPECT_TRUE(fileprod.hasObject(TYPE_TYPEDBLOB));
    EXPECT_FALSE(fileprod.hasObject(TYPE_BLOB));
    EXPECT_FALSE(fileprod.hasObject(TYPE_NONE));

    ObjectReader reader;
    EXPECT_TRUE(fileprod.registerDataCallback(&reader));
    fileprod.flowData();
    EXPECT_TRUE(fileprod.unregisterDataCallback(&reader));
}

TEST(CorePackageTest, FileProducerFlowWords)
{
    FileProducer fileprod("/tmp/test_file_cons", FileProducer::BLOB_WORD);
    EXPECT_TRUE(fileprod.hasData());
    EXPECT_TRUE(fileprod.hasObject(TYPE_TYPEDBLOB));

    ObjectReader reader;
    EXPECT_TRUE(fileprod.registerDataCallback(&reader));
    fileprod.flowData();
    EXPECT_TRUE(fileprod.unregisterDataCallback(&reader));
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

