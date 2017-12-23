#pragma once
/*
 *   basicelements.h
 *   libaft
 *
 *   Copyright © 2016-2017 Andy Warner
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

#include "element.h"

namespace aft
{
namespace ui
{
#if 0
class BinaryElement : public Element
{
public:
    virtual bool validate();
};
    
class TrinaryElement : public Element
{
public:
    virtual bool validate();
};
#endif

class IntegerElement : public Element {
public:
    virtual bool validate();
};

class StringElement : public Element
{
        
};

class ContainerElement : public Element
{
public:
    virtual bool add(const Element& element);
    virtual bool remove(const Element& element);
    virtual int layout();
    // iterator interface for elements
    virtual bool first(Element& element) const;
    virtual bool next(Element& element) const;
};

class FormElement : public ContainerElement {
public:
    const std::string& getValue() const;
    virtual void setValue(const std::string& value);
};
    
} // namespace ui
} // namespace aft
