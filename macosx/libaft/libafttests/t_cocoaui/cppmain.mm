/*   cppmain.mm
 *   libafttests that is part of AFT
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
#include <osdep/ui/cocoauidelegate.h>
#include <ui/element.h>
#include <ui/elementhandle.h>
#include <ui/ui.h>
#include <gtest/gtest.h>
using namespace aft::ui;
using aft::base::Result;
using aft::osdep::CocoaUIDelegate;
using std::cerr;
using std::cout;
using std::endl;

namespace {
    
class UiPackageTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        uiDelegate_ = new CocoaUIDelegate(&cocoaUi_);
        cocoaUi_.setUiDelegate(uiDelegate_);
        EXPECT_EQ(Result(true), cocoaUi_.init());
    }
    virtual void TearDown() {
        cocoaUi_.deinit();
    }
    
    bool runUiTests(const std::string& expected) {
        cocoaUi_.output();
        cocoaUi_.flush();
        cocoaUi_.input();
        std::string value;
        EXPECT_TRUE(cocoaUi_.getElementValue(value));
        EXPECT_EQ(expected, value);
        value.clear();
        EXPECT_TRUE(cocoaUi_.getElementValue(root_, value));
        EXPECT_EQ(expected, value);
        value.clear();
        EXPECT_TRUE(cocoaUi_.getElementValue(handle_, value));
        EXPECT_EQ(expected, value);
        return expected == value;
    }

    ElementHandle setRootElement(Element* rootElement) {
        root_ = rootElement;
        handle_ = cocoaUi_.addElement(rootElement);
        return handle_;
    }

protected:
    UI cocoaUi_;
    CocoaUIDelegate* uiDelegate_;
    Element* root_;
    ElementHandle handle_;
};

TEST_F(UiPackageTest, CocoaUiElementDefaultValue) {
    Element elOne("One");
    const std::string defaultVal("the first");
    const std::string prompt("Just click OK button");
    elOne.setDefault(defaultVal);
    elOne.setPrompt(prompt);
    ElementHandle handle = setRootElement(&elOne);
    EXPECT_TRUE(handle.isValid());
    
    EXPECT_TRUE(runUiTests(defaultVal));
}

TEST_F(UiPackageTest, CocoaUiElementEnterOk) {
    const std::string defaultVal("the first");
    const std::string prompt("Please type ok");
    Element elOne("One");
    elOne.setPrompt(prompt);
    elOne.setDefault(defaultVal);
    ElementHandle handle = setRootElement(&elOne);
    EXPECT_TRUE(handle.isValid());
    
    EXPECT_TRUE(runUiTests("ok"));
    Element* currElement = cocoaUi_.currentElement();
    ASSERT_TRUE(nullptr != currElement);
    currElement->setValue("Press Cancel to exit.");
    cocoaUi_.output();
    cocoaUi_.flush();
    cocoaUi_.input();
}

} // (anon) namespace

int CppMain(int argc, const char* argv[]) {
    ::testing::InitGoogleTest(&argc, const_cast<char **>(argv));
    return RUN_ALL_TESTS();
}

#if 0
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
    
#endif
