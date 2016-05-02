#pragma once
/*
 *  entity.h
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
#include <string>

namespace aft
{
namespace base
{
        
// Forward references
class TObject;
    

/**
 *  Entity is a minimalist TObject container class used in Consume, Provide and Require expressions.
 */
class Entity
{
public:
    enum MatchLevel
    {
        LevelNone,
        LevelAny,
        LevelTOType,
        LevelTOValue,
        LevelTOName
    };
public:
    Entity(const std::string& name, const TObject& tObject, MatchLevel matchLevel = LevelTOName);
    virtual ~Entity();

    bool operator==(const Entity& other) const;

private:
    std::string name_;
    const TObject& tObject_;
    MatchLevel matchLevel_;
};

} // namespace base
} // namespace aft
