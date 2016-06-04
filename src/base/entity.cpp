/*
 *  entity.cpp
 *  libaft
 *
 *  Copyright © 2016 Andy Warner. All rights reserved.
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

#include "entity.h"
#include "tobject.h"
#include "tobjecttype.h"
using namespace aft::base;


// Class to hold name, type, etc. used for matching LevelTOType or LevelTOName
class TObjectMatcher : public TObject
{
public:
    TObjectMatcher(const TObjectType& toType, const std::string& name)
    : TObject(toType, name)
    {
        
    }
    virtual ~TObjectMatcher()
    {
        
    }
};


Entity::Entity(const std::string& name, const TObject& tObject, MatchLevel matchLevel)
: name_(name)
, tObject_(tObject)
, matchLevel_(matchLevel)
{

}

Entity::Entity(const std::string& name, const TObjectType& toType, const std::string& toName)
: name_(name)
, tObject_(*new TObjectMatcher(toType, toName))
, matchLevel_(LevelTOType)
{
    if (!toName.empty())
    {
        matchLevel_ = LevelTOName;
    }
}

Entity::~Entity()
{

}

const TObject&
Entity::getTObject() const
{
    return tObject_;
}

bool Entity::matches(const TObject& other) const
{
    if (tObject_ == other) return true;

    bool ret = false;

    switch (matchLevel_) {
        case LevelNone:     // Never matches
            break;
        case LevelAny:     // Any TObject will do
            ret = true;
            break;
        case LevelTOType:
            ret = const_cast<TObjectType&>(tObject_.getType()) == other.getType();
            break;
        case LevelTOValue:
            ret = tObject_ == other;
            break;
        case LevelTOName:
            ret = tObject_.getName() == other.getName();
            break;
        default:
            break;
    }

    return ret;
}

bool Entity::matches(const Entity& other) const
{
    if (operator==(other)) return true;

    return matches(other.tObject_);
}

bool Entity::operator==(const Entity& other) const
{
    if (this == &other) return true;

    if (matchLevel_ == other.matchLevel_ &&
        name_ == other.name_ &&
        const_cast<TObjectType&>(tObject_.getType()) == other.tObject_.getType() &&
        tObject_ == other.tObject_)
    {
        return true;
    }

    return false;
}
