/*   cppmain.mm
 *   libafttests
 *   Copyright © 2017 Andy Warner
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

#include "cppmain.hpp"
#include <ui/cocoauidelegate.h>
#include <ui/element.h>
#include <ui/elementhandle.h>
#include <ui/ui.h>
using namespace aft::ui;
using aft::base::Result;
using std::cout;
using std::endl;

int CppMain(int argc, const char* argv[]) {
    cout << "Hello from CppMain" << endl;
    const std::string defaultVal("the first");
    const std::string prompt("Please type ok");
    Element elOne("One");
    elOne.setPrompt(prompt);
    elOne.setDefault(defaultVal);
    UI cocoaUi(nullptr, 0);
    cocoaUi.setUiDelegate(new CocoaUIDelegate(&cocoaUi));
    cocoaUi.addElement(&elOne);
    if (cocoaUi.init()) {
        cocoaUi.output();
        cocoaUi.flush();
        cocoaUi.input();
        std::string value;
        if (cocoaUi.getElementValue(value)) {
            cout << "Value entered: " << value << endl;
        }
        cocoaUi.deinit();
    }
    else {
        cout << "Got error in init of Cocoa UI" << endl;
    }

    cout << "Exit CppMain" << endl;
    return 0;
}

#if 0
#include <core/logger.h>
#include <ui/dumbttyelementdelegate.h>
#include <ui/dumbttyuidelegate.h>
#include <ui/uidelegate.h>
#include <ui/uifacet.h>
#include <gtest/gtest.h>

namespace {
    
    class UiPackageTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            UI dumbUi(root_, 0);
            dumbUi_.setUiDelegate(new DumbTtyUIDelegate(&dumbUi));
            EXPECT_EQ(Result(true), dumbUi.init());
        }
        virtual void TearDown() {
            dumbUi_.deinit();
        }
        
        bool runUiTests(const std::string& expected) {
            dumbUi_.output();
            dumbUi_.flush();
            dumbUi_.input();
            std::string value;
            EXPECT_TRUE(dumbUi_.getElementValue(value));
            EXPECT_EQ(expected, value);
            value.clear();
            EXPECT_TRUE(dumbUi_.getElementValue(root_, value));
            EXPECT_EQ(expected, value);
            value.clear();
            EXPECT_TRUE(dumbUi_.getElementValue(handle_, value));
            EXPECT_EQ(expected, value);
            return expected == value;
        }
        
        ElementHandle setRootElement(Element* rootElement) {
            root_ = rootElement;
            handle_ = dumbUi_.addElement(rootElement);
            return handle_;
        }
        
    protected:
        UI dumbUi_;
        DumbTtyUIDelegate* uiDelegate_;
        Element* root_;
        ElementHandle handle_;
    };
    
    TEST_F(UiPackageTest, DumbTtyElementDefaultValue) {
        Element elOne("One");
        const std::string defaultVal("the first");
        const std::string prompt("Just hit return");
        elOne.setDefault(defaultVal);
        elOne.setPrompt(prompt);
        ElementHandle handle = setRootElement(&elOne);
        EXPECT_TRUE(handle.isValid());
        
        EXPECT_TRUE(runUiTests(defaultVal));
    }
    
    TEST_F(UiPackageTest, DumbTtyElementEnterOk) {
        const std::string defaultVal("the first");
        const std::string prompt("Please type ok");
        Element elOne("One");
        elOne.setPrompt(prompt);
        elOne.setDefault(defaultVal);
        ElementHandle handle = setRootElement(&elOne);
        EXPECT_TRUE(handle.isValid());
        
        EXPECT_TRUE(runUiTests("ok"));
    }
    
    TEST_F(UiPackageTest, DumbTtyUiTwoTopElements) {
        Element elOne("One");
        const std::string defaultVal("the first");
        const std::string prompt("Just hit return");
        elOne.setDefault(defaultVal);
        elOne.setPrompt(prompt);
        ElementHandle handle = setRootElement(&elOne);
        EXPECT_TRUE(handle.isValid());
        
        Element elTwo("Two");
        const std::string defaultVal2("the second");
        const std::string prompt2("Hit return");
        elOne.setDefault(defaultVal2);
        elOne.setPrompt(prompt2);
        ElementHandle handle2 = dumbUi_.addElement(&elTwo);
        EXPECT_TRUE(handle2.isValid());
    }
    
    TEST_F(UiPackageTest, DumbTtyUiDelegateTest) {
        
    }
    
} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
