/*
 *  outlet.cpp
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
#include "outlet.h"
#include "prodcons.h"


using namespace aft::base;


class aft::base::OutletImpl
{
public:
    OutletImpl()
    : consumer(0)
    , producer(0)
    , prodcons(0)
    {  }

    ConsumerContract* consumer;
    ProducerContract* producer;
    ProdConsContract* prodcons;
};
        
///////////////////////////////////////////////////////////

Outlet::Outlet(const std::string& name)
: impl_(*new OutletImpl)
, name_(name)
, direction_(DirectionNone)
{
    
}

Outlet::~Outlet()
{
    delete &impl_;
}


bool Outlet::openIn(ProducerContract& producer)
{
    if (direction_ != DirectionNone) return false;

    impl_.producer = &producer;
    direction_ = DirectionIn;
    return true;
}

bool Outlet::openInOut(ProdConsContract& prodcons)
{
    if (direction_ != DirectionNone) return false;
    
    impl_.prodcons = &prodcons;
    direction_ = DirectionInOut;
    return true;
}

bool Outlet::openOut(ConsumerContract& consumer)
{
    if (direction_ != DirectionNone) return false;
    
    impl_.consumer = &consumer;
    direction_ = DirectionOut;
    return true;
}

void Outlet::consumes(const Entity& entity)
{
    
}

void Outlet::provides(const Entity& entity)
{
    
}

void Outlet::requires(const Entity& entity)
{
    
}

