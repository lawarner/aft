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

#include <iostream>

#include <base/blob.h>
#include <base/factory.h>
#include <base/plugin.h>
#include <base/tobject.h>
#include <gtest/gtest.h>
using namespace aft::base;


namespace
{

const std::string BUNDLE_NAME("sample_plugin");

TEST(PluginTest, BasePlugin)
{
    BasePlugin plugin(BUNDLE_NAME, ".");
    std::cout << "Initial describe: " << plugin.describePlugins() << std::endl;

    EXPECT_TRUE(plugin.loadPlugins());
    std::cout << "During describe: " << plugin.describePlugins() << std::endl;

    EXPECT_TRUE(plugin.getFactory() != 0);
    EXPECT_TRUE(plugin.getPluginLoader() != 0);

    Blob blob("valueBlob", Blob::STRING, "Sample TObject");
    TObject* tobject = plugin.createInstance(&blob);
    EXPECT_TRUE(tobject != 0);

    //TODO check results
    tobject->run();
    delete tobject;

    plugin.unloadPlugins();
    std::cout << "After describe: " << plugin.describePlugins() << std::endl;
}

TEST(PluginTest, MecFactoryFactory)
{
    BasePlugin plugin(BUNDLE_NAME, ".");
    EXPECT_TRUE(plugin.loadPlugins());

    BaseFactory* factory = plugin.getFactory();
    EXPECT_TRUE(factory != 0);

    MecFactory* mec = MecFactory::instance();
    mec->addFactory(factory);

    const std::string objectName("anObject");
    Blob blob("valueBlob", Blob::STRING, "Sample TObject");
    TObject* tobj = mec->construct(factory->category(), objectName, &blob);
    EXPECT_TRUE(tobj != 0);
    EXPECT_EQ(tobj->getName(), objectName);

    //TODO check results
    tobj->run();
    free(tobj);

    mec->removeFactory(factory);
    plugin.unloadPlugins();
}

TEST(PluginTest, MecFactoryBundle)
{
    MecFactory* mec = MecFactory::instance();
    PluginContract* plugin = mec->loadBundle(BUNDLE_NAME, ".");
    EXPECT_TRUE(plugin != 0);
    FactoryContract* factory = plugin->getFactory();
    EXPECT_TRUE(factory != 0);

    const std::string objectName("anObject");
    Blob blob("valueBlob", Blob::STRING, "Sample TObject");
    TObject* tobj = mec->construct(factory->category(), objectName, &blob);
    EXPECT_TRUE(tobj != 0);
    EXPECT_EQ(tobj->getName(), objectName);

    //TODO check results
    tobj->run();
    free(tobj);

    EXPECT_TRUE(mec->unloadBundle(plugin));
    free(plugin);
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

