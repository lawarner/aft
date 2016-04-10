#pragma once
/*
 *  outlet.h
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
class ConsumerContract;
class Entity;
class ProducerContract;
class ProdConsContract;
class OutletImpl;


class Outlet
{
public:
    enum Direction
    {
        DirectionNone,
        DirectionIn,
        DirectionOut,
        DirectionInOut
    };
public:
    Outlet(const std::string& name);
    virtual ~Outlet();

    bool openIn(ProducerContract& producer);
    bool openInOut(ProdConsContract& prodcons);
    bool openOut(ConsumerContract& consumer);

    // An Entity can be at TObject type level, or an actual named TObject.
    void consumes(const Entity& entity);
    void provides(const Entity& entity);
    void requires(const Entity& entity);

private:
    OutletImpl& impl_;
    std::string name_;
    Direction direction_;
};
    
} // namespace base
} // namespace aft
