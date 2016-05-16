#pragma once
/*
 *   Copyright 2016 Andy Warner
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

namespace aft
{
namespace base
{
// Forward reference

/**
 *  Interface used for predicates.
 */
class PredicateContract
{
public:
    virtual bool isTrue() const = 0;
    virtual bool operator==(const PredicateContract& other) const = 0;
};

/** Base implementation of a PredicateContract based on TObjectContainer */
class BasePredicate : public TObjectContainer, public PredicateContract
{
public:
    BasePredicate();
    BasePredicate(const TObject* condition);
    virtual ~BasePredicate();

    // implement PredicateContract
    virtual bool isTrue() const;
    virtual bool operator==(const PredicateContract& other) const;

    /** Type name for Predicate TObjects */
    static const std::string TOTypeName;
    /** Cached common reference for Predicate type */
    static const TObjectType& TOType;

protected:
    //TObject& condition_;
};
    
} // namespace base
} // namespace aft
