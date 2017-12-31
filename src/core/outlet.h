#pragma once
/*
 *  outlet.h
 *  libaft
 *
 *  Copyright © 2016, 2017 Andy Warner. All rights reserved.
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
#include "base/entity.h"
#include "base/proc.h"
#include "base/serialize.h"
#include <string>
#include <vector>

namespace aft {

namespace base {
// Forward references
class ConsumerContract;
class ProducerContract;
}

namespace core {
// Forward references
class OutletImpl;

/** Type of Outlet. This is determined by which prod, cons or proc is plugged in. */
enum class OutletType {
    None,
    In,
    Out,
    InOut
};
        
/**
 *  An Outlet is a placeholder for Producer and Consumer in execution models and pipelines.
 *  Outlets can be placed at input/output positions within the model and later,
 *  at run-time, the outlet is hooked up with the consumers and providers that
 *  perform the I/O. Thus this class allows dynamic, late binding of I/O for models.
 */
class Outlet : public base::BaseProc, public base::SerializeContract {
public:
    using EntityList = std::vector<base::Entity>;
            
public:
    Outlet(const std::string& name);
    virtual ~Outlet();

    const std::string& name() const;
    OutletType type() const;

    bool plugin(base::ProducerContract* producer);
    bool plugin(base::ProcContract* proc);
    bool plugin(base::ConsumerContract* consumer);
    /** Unplug everything from the Outlet.
     *  @return True if successful, otherwise false.
     */
    bool unplug();
    /** Unplug a producer from the Outlet.
     *  @param producer The producer to unplug. If not specified or nullptr then
     *                  unplugs whatever producer is plugged in.
     *  @return True if successful, otherwise false.
     */
    bool unplug(base::ProducerContract* producer);
    bool unplug(base::ProcContract* proc);
    bool unplug(base::ConsumerContract* consumer);

    // An Entity can be at TObject type level, or an actual named TObject.
    const EntityList& consumes() const;
    const EntityList& provides() const;
    const EntityList& requires() const;
    void consumes(const EntityList& entities);
    void provides(const EntityList& entities);
    void requires(const EntityList& entities);

    // Implement ProcContract (ProviderContract)
    virtual base::Result read(base::TObject& object) override;
    virtual base::Result read(base::Result& result) override;
    virtual base::Result read(base::Blob& blob) override;
    virtual bool hasData() override;
    virtual bool hasObject(base::ProductType productType) override;

    // Implement ProcContract (ConsumerContract)
    virtual bool canAcceptData() override;
    virtual base::Result write(const base::TObject& object) override;
    virtual base::Result write(const base::Result& result) override;
    virtual base::Result write(const base::Blob& blob) override;

    bool operator==(const Outlet& other) const;

    virtual bool serialize(base::Blob& blob) override;
    virtual bool deserialize(const base::Blob& blob) override;

private:
    OutletImpl& impl_;
    std::string name_;
};
        
} // namespace core
} // namespace aft
