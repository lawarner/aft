/*
 *   Copyright 2015 - 2017 Andy Warner
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

#include <base/result.h>
#include <core/logger.h>
#include <ui/element.h>
#include <ui/elementdelegate.h>
#include <ui/ui.h>
#include <ui/uidelegate.h>
#include <ui/uifacet.h>
#include <gtest/gtest.h>
using aft::base::Result;
using namespace aft::core;
using namespace aft::ui;
using std::endl;

class EmptyElementDelegate : public ElementDelegate {
public:
    EmptyElementDelegate() : focused_(nullptr) { }
    virtual ~EmptyElementDelegate() = default;

    virtual void flush(const Element* element) override {
        
    }
    virtual const std::string& getValue(const Element* element) const override {
        return value_;  // this is wrong, but for now...
    }

    virtual void setValue(Element* element, const std::string& value) override {
        value_ = value;  // wrong again
    }

    virtual bool getFocus(Element* element) const override {
        return element == focused_;
    }
    virtual bool setFocus(Element* element, bool hasFocus = true) override {
        if (hasFocus) {
            focused_ = element;
        }
        else if (focused_ == element) {
            focused_ = nullptr;
        }
        return true;
    }
    virtual bool getFacet(Element* element, const std::string& name, UIFacetCategory category,
                          UIFacet& facet) const override {
        return false;
    }
    virtual bool setFacet(Element* element, const UIFacet& facet) override {
        return false;
    }
    
    virtual bool input(const Element* element, std::string& value) override {
        value = value_;
        return true;
    }
    virtual bool output(const Element* element, bool showValue = false) override {
        if (element->hasValue()) {
            value_ = element->getValue();
        }
        else {
            value_ = element->getDefault();
        }
        return true;
    }
private:
    std::string value_;
    Element* focused_;
};

class EmptyUiDelegate : public UIDelegate {
public:
    bool add(const Element& element) override {
        return false;
    }
    void flush(const Element& element) override {
        
    }
    bool focus(const Element& element) override {
        return false;
    }
    const Element* get(const ElementHandle& handle) override {
        return nullptr;
    }
    const Element* get(const std::string& name) override {
        return nullptr;
    }
    bool hide(const Element& element) override {
        return false;
    }
    /** Get user input from element */
    bool input(const Element& element, std::string& value) override {
        value = element.getDefault();
        return true;
    }
    /** Output the element to the user interface */
    bool output(const Element& element, bool showValue = false) const override {
        aftlog << "(Custom) Element " << element.getName() << endl;
        return true;
    }
    bool remove(const Element& element) override {
        return false;
    }
    bool show(const Element& element) override {
        return false;
    }
};

class StackUiDelegate : public UIDelegate
{
public:
    using ElementList = std::vector<Element*>;
public:
    bool add(const Element& element) {
        ElementList::iterator it = findElement(element);
        if (it == elements_.end())
        {
            elements_.push_back(new Element(element));
            return true;
        }
        return false;
    }
    void flush() {
        
    }
    bool focus(const Element& element) {
        return false;
    }
    bool hide(const Element& element) {
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
        aftlog << "(Stack) Element " << element.getName() << endl;
        add(element);
        return true;
    }
    bool remove(const Element& element)
    {
        auto it = findElement(element);
        if (it != elements_.end()) {
            elements_.erase(it);
            return true;
        }
        return false;
    }
    bool setFacet(const Element& element, const UIFacet& facet) {
        auto it = findElement(element);
        if (it != elements_.end()) {
            (*it)->apply(facet);
            return true;
        }
        return false;
    }
private:
    ElementList::iterator findElement(const Element& element) {
        auto it = elements_.begin();
        for ( ; it != elements_.end(); ++it) {
            //TODO find element by ID
            if ((*it)->getName() == element.getName()) {
                break;
            }
        }
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

    bool runElementTests(Element& element, const std::string& defaultValue, const std::string& prompt) {
        aftlog << std::endl << "++++ Start tests for element " << element.getName() << " ++++" << endl;
#if 0
        Element(const std::string& name);
        Element(const Element& other);
        Element& operator=(const Element& other);
        
        // work methods
        virtual void apply(const UIFacet& facet);
        /** True if value has already been read/updated. */
        virtual bool hasValue() const;
        virtual void remove(const UIFacet& facet);
        /** Validate the value of this element.
         *  This base class implementation always returns true.
         */
        virtual bool validate();
        
        // getters and setters
        const ElementId getId() const;
        const std::string& getName() const;
        /** Get the value stored for this element. */
        const std::string& getValue() const;
        /** Set the value for this element. */
        virtual void setValue(const std::string& value);
        /** Get the default value for this element */
        virtual const std::string& getDefault() const;
        /** Set the default value for this element */
        virtual void setDefault(const std::string& value);
        virtual const std::string& getPrompt() const;
        virtual void setPrompt(const std::string& prompt);
        virtual bool getEnabled() const;
        virtual void setEnabled(bool isEnabled);
        virtual bool getVisible() const;
        EXPECT_FALSE(element.getVisible());
        virtual void setVisible(bool isVisible);
        /////////////////////////
        EXPECT_TRUE(element.getVisible());
        aftlog << "++ Show element with prompt:" << std::endl;
        element.setPrompt(prompt);
        EXPECT_TRUE(element.getPrompt() == prompt);
        aftlog << "++ Show element with prompt and default value:" << std::endl;
        element.setDefault(defaultValue);

        element.setEnabled(true);
        EXPECT_TRUE(element.getDefault() == defaultValue);
        EXPECT_FALSE(element.getValue().empty());
#endif
        EXPECT_TRUE(element.getValue() == defaultValue);
        element.setValue("to another");
        EXPECT_TRUE(element.getValue() == "to another");
//        EXPECT_TRUE(element.getValue(true) == "to another");

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
    
    bool runUiTests(UI& ui) {
#if 0
        aftlog << "++ Show element:" << endl;
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
        EXPECT_FALSE(element.getValue().empty());
        EXPECT_TRUE(element.input());
#endif
        return true;
    }
};

///////////// Tests here :

TEST_F(UiPackageTest, ElementConstructorTests) {
    Element element("One");
    element.setDefault("Default value");
    element.setPrompt("prompt: ");
    element.setValue("Real value");

    Element otherElement(element);
    EXPECT_TRUE(element == otherElement);
    Element copyElement("at first");
    copyElement = element;
    EXPECT_TRUE(element == copyElement);
}
    
TEST_F(UiPackageTest, ElementMemberTests) {
    const std::string elName("MyElement");
    const std::string elPrompt("Enter value");
    const std::string elDefault("Default value");
    const std::string elValue("Real value");
    Element element(elName);
    UIFacet facet("facet1", "abc");
    element.apply(facet);
    std::string facetValue;
    EXPECT_TRUE(element.getFacet(facetValue, "facet1"));
    EXPECT_EQ("abc", facetValue);
    element.remove(facet);
    EXPECT_FALSE(element.getFacet(facetValue, "facet1"));

    EXPECT_FALSE(element.hasValue());
    element.setValue(elValue);
    EXPECT_TRUE(element.hasValue());
    EXPECT_TRUE(element.validate());
    
    Element someElement("another");
    EXPECT_NE(element.getId(), someElement.getId());
    EXPECT_EQ(elName, element.getName());
    EXPECT_EQ(elValue, element.getValue());
    EXPECT_TRUE(someElement.validate());

    EXPECT_TRUE(element.getDefault().empty());
    element.setDefault(elDefault);
    EXPECT_EQ(elDefault, element.getDefault());
    EXPECT_TRUE(element.getPrompt().empty());
    element.setPrompt(elPrompt);
    EXPECT_EQ(elPrompt, element.getPrompt());
    EXPECT_FALSE(element.getEnabled());
    element.setEnabled(true);
    EXPECT_TRUE(element.getEnabled());
    EXPECT_FALSE(element.getVisible());
    element.setVisible(true);
    EXPECT_TRUE(element.getVisible());
    
    element.setValue("to another");
    EXPECT_EQ("to another", element.getValue());
    EXPECT_TRUE(element.validate());
}
    
TEST_F(UiPackageTest, UIFacet) {
    UIFacet facet("facet", "other");
    std::vector<UIFacetCategory> categories = facet.getCategories();
    EXPECT_EQ(1, categories.size());
    EXPECT_EQ(UIFacetCategory::Other, categories[0]);
    std::string strValue;
    EXPECT_TRUE(facet.get("facet", strValue));
    EXPECT_EQ("other", strValue);
    EXPECT_FALSE(facet.getMandatory());
    facet.setMandatory(true);
    EXPECT_TRUE(facet.getMandatory());

    UIFacet colorFacet("Colors", "Facets for Widget Colors", UIFacetCategory::Color);
    categories = colorFacet.getCategories();
    EXPECT_EQ(1, categories.size());
    EXPECT_EQ(UIFacetCategory::Color, categories[0]);
    EXPECT_TRUE(colorFacet.set("foreground", 0xfefefe));
    EXPECT_TRUE(colorFacet.set("background", 0x000000));
    EXPECT_TRUE(colorFacet.set("dialog.text", 0xee0000));
    int result;
    EXPECT_TRUE(colorFacet.get("foreground", result));
    EXPECT_EQ(0xfefefe, result);
    EXPECT_TRUE(colorFacet.get("background", result));
    EXPECT_EQ(0, result);
    EXPECT_TRUE(colorFacet.get("dialog.text", result));
    EXPECT_EQ(0xee0000, result);
    
    UIFacet copyFacet = colorFacet;
    //EXPECT_EQ(copyFacet, colorFacet);
    EXPECT_TRUE(copyFacet.get("foreground", result));
    EXPECT_EQ(0xfefefe, result);
    EXPECT_TRUE(copyFacet.get("background", result));
    EXPECT_EQ(0, result);
    EXPECT_TRUE(copyFacet.get("dialog.text", result));
    EXPECT_EQ(0xee0000, result);
}

TEST_F(UiPackageTest, UIWithEmptyDelegate) {
    EmptyUiDelegate* uiDelegate = new EmptyUiDelegate;
    UI emptyUi(0, uiDelegate);
    EmptyElementDelegate eelDelegate;
    Element element("element");
    ElementHandle handle = emptyUi.addElement(&element);
    EXPECT_FALSE(handle.isValid());
    // It is equal to emptyElement
    EXPECT_NE(nullptr, emptyUi.currentElement());
}

TEST_F(UiPackageTest, UIWithBaseDelegate) {
    UI baseUi;
    EXPECT_EQ(Result(true), baseUi.init());

    Element element("element");
    element.setPrompt("Enter your name");
    element.setDefault("(unknown)");
    ElementHandle handle = baseUi.addElement(&element);
    EXPECT_TRUE(handle.isValid());
    Element* el1 = baseUi.currentElement();
    EXPECT_NE(nullptr, el1);
    EXPECT_TRUE(el1->getName() == "element");
    ssize_t idx = baseUi.findElement(&element);
    EXPECT_EQ(0, idx);

    baseUi.erase();
    baseUi.draw();
    baseUi.output();
    baseUi.input();

    Element element2("element2");
    idx = baseUi.findElement(&element2);
    EXPECT_EQ(-1, idx);
    handle = baseUi.addElement(&element2);
    EXPECT_TRUE(handle.isValid());
    idx = baseUi.findElement(&element2);
    EXPECT_NE(-1, idx);
    
    EXPECT_EQ(&element, baseUi.currentElement());
    EXPECT_EQ(&element, baseUi.getElement(0));
    EXPECT_EQ(1, baseUi.nextElement());
    EXPECT_EQ(&element2, baseUi.currentElement());
    
    EXPECT_TRUE(baseUi.removeElement(&element));
    EXPECT_TRUE(baseUi.firstElement());
    EXPECT_EQ(&element2, baseUi.currentElement());
    
    EXPECT_TRUE(baseUi.removeElement(&element2));
    EXPECT_FALSE(baseUi.firstElement());
    handle = baseUi.addElement(&element2);
    EXPECT_TRUE(handle.isValid());
    idx = baseUi.findElement(&element2);
    EXPECT_EQ(0, idx);
    EXPECT_TRUE(baseUi.firstElement());

    EXPECT_EQ(Result(true), baseUi.deinit());
}

TEST_F(UiPackageTest, UIWithBaseDelegateListener) {
    UI baseUi;
    EXPECT_EQ(Result(true), baseUi.init());

    UI::CallbackFunction listener = [](UiEventType eventType, Element* element)->bool {
        aftlog << "Got a callback with " << static_cast<int>(eventType)
               << " element: " << element->getName() << endl;
        return true;
    };
    baseUi.registerListener(&listener);
    Element element("element");
    element.setPrompt("Enter your name");
    element.setDefault("(unknown)");
    ElementHandle handle = baseUi.addElement(&element);
    EXPECT_TRUE(handle.isValid());
    handle = baseUi.addElement(&element);
    EXPECT_FALSE(handle.isValid());

    baseUi.unregisterListener(&listener);
    Element element2("element2");
    handle = baseUi.addElement(&element2);
    EXPECT_TRUE(handle.isValid());

    EXPECT_EQ(Result(true), baseUi.deinit());
}
#if 0
    // Producer contract
    virtual bool read(base::TObject& object) override;
    virtual bool read(base::Result& result) override;
    virtual bool read(base::Blob& blob) override;
    virtual bool hasData() override;
    virtual bool hasObject(base::ProductType productType) override;
    
    // Consumer contract
    virtual bool canAcceptData() override;
    virtual bool write(const base::TObject& object) override;
    virtual bool write(const base::Result& result) override;
    virtual bool write(const base::Blob& blob) override;

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
#endif
    
} // namespace

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
