#pragma once
/*
 *  entity.h
 *  libaft
 *
 *  Copyright © 2016-2017 Andy Warner. All rights reserved.
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
#include "tobject.h"
#include <string>

namespace aft {
namespace base {
        
// Forward references
class TObjectType;
    

/**
 *  Entity is a minimalist TObject container class used in Consume, Provide and
 *  Require expressions.
 */
class Entity
{
public:
    enum MatchLevel {
        LevelNone,
        LevelAny,
        LevelTOType,
        LevelTOValue,
        LevelTOName
    };
public:
    /** Construct a named entity wrapping a TObject */
    Entity(const std::string& name, TObject* tObject, MatchLevel matchLevel = LevelTOName);

    /** Construct a named entity used for matching other entities.
     *  A TObject is synthesized to hold the matching values. */
    Entity(const std::string& name, const TObjectType& toType,
           const std::string& toName = std::string());

    Entity(const Entity& other);

    virtual ~Entity() = default;

    const TObject* getTObject() const;

    /**
     *  Check if a given TObject matches this entity's requirements.
     */
    bool matches(const TObject& other) const;
    
    /**
     *  Check if a given entity matches this entity's requirements.
     */
    bool matches(const Entity& other) const;

    Entity& operator=(const Entity& other);

    /** Check if this entity is equal to another */
    bool operator==(const Entity& other) const;

private:
    std::string name_;
    TObject* tObject_;
    MatchLevel matchLevel_;
};

} // namespace base
} // namespace aft
