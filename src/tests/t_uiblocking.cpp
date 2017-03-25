//
//  t_uiblocking.cpp
//  libaft
//
//  Created by Lynn Warner on 3/25/17.
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include <stdio.h>
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

#include <algorithm>
#include <string>
#include <vector>

#include <core/logger.h>
#include <ui/element.h>
#include <ui/ui.h>
#include <ui/uidelegate.h>
#include <ui/uifacet.h>
#include <gtest/gtest.h>
using namespace aft::core;
using namespace aft::ui;


namespace
{
    class UiPackageTest : public ::testing::Test
    {
    protected:
        virtual void SetUp()
        {
        }
        virtual void TearDown()
        {
        }
        
        bool runElementTests(Element& element, const std::string& defaultValue, const std::string& prompt)
        {
            aftlog << std::endl << "++++ Start tests for element " << element.getName() << " ++++" << std::endl;
            aftlog << "++ Show element:" << std::endl;
            EXPECT_FALSE(element.getVisible());
            element.show();
            EXPECT_TRUE(element.getVisible());
            aftlog << "++ Will not show element again:" << std::endl;
            element.show();
            aftlog << "++ But will show element after hide/show:" << std::endl;
            element.hide();
            element.show();
            aftlog << "++ Show element again (force):" << std::endl;
            element.show(true);
            
            aftlog << "++ Show element with prompt:" << std::endl;
            element.setPrompt(prompt);
            EXPECT_TRUE(element.getPrompt() == prompt);
            element.show(true);
            aftlog << "++ Show element with prompt and default value:" << std::endl;
            element.setDefault(defaultValue);
            element.show(true);
            
            aftlog << "++ Refresh:" << std::endl;
            element.refresh();
            
            element.setEnabled(true);
            EXPECT_TRUE(element.getDefault() == defaultValue);
            EXPECT_TRUE(element.getValue().empty());
            EXPECT_TRUE(element.input());
            EXPECT_TRUE(element.getValue() == defaultValue);
            element.setValue("to another");
            EXPECT_TRUE(element.getValue() == "to another");
            EXPECT_TRUE(element.getValue(true) == defaultValue);
            
            EXPECT_TRUE(element.getVisible());
            element.setVisible(false);
            EXPECT_FALSE(element.getVisible());
            EXPECT_FALSE(element.getEnabled());
            element.setEnabled(true);
            EXPECT_TRUE(element.getEnabled());
            
            bool retval = element.validate();
            aftlog << "++++ End tests for element " << element.getName()
            << " returns " << std::boolalpha << retval << " ++++" << std::endl;
            return retval;
        }
    };

TEST(BasePackageTest, TerminalInput)
{
    std::cout << "Enter your name or something: " << std::endl;
    char cname[80];
    std::cin.getline(cname, 80);
    std::string name(cname, strlen(cname));
    
    aftlog << "So you are " << name << ", are you?" << std::endl;
}
    
TEST_F(UiPackageTest, BaseElement)
{
    Element elOne("One");
    EXPECT_TRUE(runElementTests(elOne, "the first", "Enter first item"));
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
