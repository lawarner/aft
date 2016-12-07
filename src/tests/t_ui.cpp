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
#include <gtest/gtest.h>
using namespace aft::core;
using namespace aft::ui;


class EmptyUiDelegate : public UIDelegate
{
public:
    bool add(const Element& element)
    {
        return false;
    }
    /** Get user input from element */
    bool input(const Element& element, std::string& value)
    {
        value = element.getDefault();
        return true;
    }
    /** Output the element to the user interface */
    bool output(const Element& element)
    {
        aftlog << "(Custom) Element " << element.getName() << std::endl;
        return true;
    }
    bool remove(const Element& element)
    {
        return false;
    }
};

class StackUiDelegate : public UIDelegate
{
public:
    typedef std::vector<const Element*> ElementList;
public:
    bool add(const Element& element)
    {
        ElementList::iterator it = findElement(element);
        if (it == elements_.end())
        {
            elements_.push_back(&element);
            return true;
        }
        return false;
    }
    /** Get user input from element */
    bool input(const Element& element, std::string& value)
    {
        bool retval = false;
        if (elements_.empty())
        {
            value = element.getDefault();
        }
        else
        {
            const Element* elValue = elements_.back();
            elements_.pop_back();
            value = elValue->getValue().empty() ? elValue->getDefault() : elValue->getValue();
            retval = true;
        }
        return retval;
    }
    /** Output the element to the user interface */
    bool output(const Element& element)
    {
        aftlog << "(Stack) Element " << element.getName() << std::endl;
        add(element);
        return true;
    }
    bool remove(const Element& element)
    {
        ElementList::iterator it = findElement(element);
        if (it == elements_.end())
        {
            elements_.erase(it);
            return true;
        }
        return false;
    }
private:
    ElementList::iterator findElement(const Element& element)
    {
        ElementList::iterator it = std::find(elements_.begin(), elements_.end(), &element);
        return it;
    }
private:
    ElementList elements_;
};


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

    
TEST_F(UiPackageTest, BaseElement)
{
    Element elOne("One");
    EXPECT_TRUE(runElementTests(elOne, "the first", "Enter first item"));
}

TEST_F(UiPackageTest, BaseElementWithDelegate)
{
    EmptyUiDelegate uiDelegate;
    Element elTwo("Two", &uiDelegate);
    EXPECT_TRUE(runElementTests(elTwo, "now with delegates", "Enter second value"));

    StackUiDelegate stackDelegate;
    Element elThree("Three", &stackDelegate);
    EXPECT_TRUE(runElementTests(elThree, "default value of 3", "Prompt for 3"));

    Element elFour("Four", &stackDelegate);
    elFour.setValue("My value came from 4");
    elFour.show();
    Element elFive("Five", &stackDelegate);
    elFive.setValue("I am five");
    elFive.show();

    aftlog << "=== elThree before: " << elThree.getValue() << std::endl;
    EXPECT_TRUE(elThree.input());
    aftlog << "=== elThree after:  " << elThree.getValue() << std::endl;
    EXPECT_TRUE(elThree.getValue() == "I am five");
    aftlog << "=== elThree pop 5:   " << elThree.getValue() << std::endl;

    EXPECT_TRUE(elThree.input());
    aftlog << "=== elThree pop 4:   " << elThree.getValue() << std::endl;
    EXPECT_TRUE(elThree.getValue() == "My value came from 4");

    // Delegate's stack is empty so we are back to elThree's default value
    EXPECT_FALSE(elThree.input());
    aftlog << "=== elThree default:   " << elThree.getValue() << std::endl;
    EXPECT_TRUE(elThree.getValue() == "default value of 3");
}

} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
