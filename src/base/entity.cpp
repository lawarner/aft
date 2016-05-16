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


Entity::Entity(const std::string& name, const TObject& tObject, MatchLevel matchLevel)
: name_(name)
, tObject_(tObject)
, matchLevel_(matchLevel)
{

}

Entity::~Entity()
{

}

bool Entity::operator==(const Entity& other) const
{
    bool ret = false;

    if (this == &other) return true;

    if (matchLevel_ == other.matchLevel_)
    {
        switch (matchLevel_)
        {
            case LevelNone:     // Never matches
                break;
            case LevelAny:     // Any TObject will do
                ret = true;
                break;
            case LevelTOType:
                ret = const_cast<TObjectType&>(tObject_.getType()) == other.tObject_.getType();
                break;
            case LevelTOValue:
                ret = tObject_ == other.tObject_;
                break;
            case LevelTOName:
                ret = tObject_.getName() == other.tObject_.getName();
                break;
            default:
                break;
        }
    }
    return ret;
}
