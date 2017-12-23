/*
 *  outlet.cpp
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
#include "outlet.h"
#include "base/blob.h"
#include "base/structureddata.h"

using namespace aft::base;
using namespace aft::core;

// Implementation of ProcContract where all methods fail.
class DummyProc : public ProcContract {
    virtual Result read(TObject& object) override {
        return false;
    }
    virtual Result read(Result& result) override {
        return false;
    }
    virtual Result read(Blob& blob) override {
        return false;
    }
    virtual bool hasData() override {
        return false;
    }
    virtual bool hasObject(ProductType productType) override {
        return false;
    }
    virtual bool registerDataCallback(const ReaderContract* reader) override {
        return false;
    }
    virtual bool unregisterDataCallback(const ReaderContract* reader) override {
        return false;
    }
    
    virtual bool canAcceptData() override {
        return false;
    }
    virtual Result write(const TObject& object) override {
        return false;
    }
    virtual Result write(const Result& result) override {
        return false;
    }
    virtual Result write(const Blob& blob) override {
        return false;
    }
    virtual int write(const std::vector<TObject>& objects) override {
        return false;
    }
    virtual int write(const std::vector<Result>& results) override {
        return false;
    }
    virtual int write(const std::vector<Blob>& blobs) override {
        return false;
    }
    virtual bool registerWriteCallback(const WriterContract* writer) override {
        return false;
    }
    virtual bool unregisterWriteCallback(const WriterContract* writer) override {
        return false;
    }
};

class aft::core::OutletImpl {
public:
    OutletImpl()
    : type(OutletType::None)
    , consumer(nullptr)
    , producer(nullptr)
    , proc(nullptr) {  }
    
    bool okForInput() const {
        return (OutletType::In == type && nullptr != producer) ||
        (OutletType::InOut == type && nullptr != proc);
    }
    
    ProducerContract& reader() {
        if (nullptr != producer && OutletType::In == type) {
            return *producer;
        }
        else if (nullptr != proc && OutletType::InOut == type) {
            return *proc;
        }
        return dummyProc;
    }
    ConsumerContract& writer() {
        if (nullptr != consumer && OutletType::Out == type) {
            return *consumer;
        }
        else if (nullptr != proc && OutletType::InOut == type) {
            return *proc;
        }
        return dummyProc;
    }
public:
    OutletType type;
    ConsumerContract* consumer;
    ProducerContract* producer;
    ProcContract* proc;
    
    Outlet::EntityList consumed;
    Outlet::EntityList produced;
    Outlet::EntityList required;
private:
    DummyProc dummyProc;
};

///////////////////////////////////////////////////////////

Outlet::Outlet(const std::string& name)
: impl_(*new OutletImpl)
, name_(name) {
}

Outlet::~Outlet() {
    delete &impl_;
}

const std::string&
Outlet::name() const {
    return name_;
}

OutletType
Outlet::type() const {
    return impl_.type;
}

bool Outlet::plugin(ProducerContract* producer) {
    if (OutletType::None != type()) return false;
    
    impl_.producer = producer;
    impl_.type = OutletType::In;
    return true;
}

bool Outlet::plugin(ProcContract* proc) {
    if (OutletType::None != type()) return false;
    
    impl_.proc = proc;
    impl_.type = OutletType::InOut;
    return true;
}

bool Outlet::plugin(ConsumerContract* consumer) {
    if (OutletType::None != type()) return false;
    
    impl_.consumer = consumer;
    impl_.type = OutletType::Out;
    return true;
}

bool Outlet::unplug() {
    impl_.type = OutletType::None;
    impl_.consumer = nullptr;
    impl_.proc = nullptr;
    impl_.producer = nullptr;
    return true;
}

bool Outlet::unplug(ProducerContract* producer) {
    if (OutletType::In != type() || producer != impl_.producer) {
        return false;
    }
    impl_.type = OutletType::None;
    impl_.producer = nullptr;
    return true;
}

bool Outlet::unplug(ProcContract* proc) {
    if (OutletType::InOut != type()) return false;
    if (nullptr != proc && proc != impl_.proc) return false;
    
    impl_.type = OutletType::None;
    impl_.proc = nullptr;
    return true;
}

bool Outlet::unplug(ConsumerContract* consumer) {
    if (OutletType::Out != type()) return false;
    if (nullptr != consumer && consumer != impl_.consumer) return false;
    
    impl_.type = OutletType::None;
    impl_.consumer = nullptr;
    return true;
}

const Outlet::EntityList& Outlet::consumes() const {
    return impl_.consumed;
}

const Outlet::EntityList& Outlet::provides() const {
    return impl_.produced;
}

const Outlet::EntityList& Outlet::requires() const {
    return impl_.required;
}

void Outlet::consumes(const EntityList& entities) {
    impl_.consumed = entities;
}

void Outlet::provides(const EntityList& entities) {
    impl_.produced = entities;
}

void Outlet::requires(const EntityList& entities) {
    
}

Result Outlet::read(TObject& object) {
    return impl_.reader().read(object);
}

Result Outlet::read(Result& result) {
    return impl_.reader().read(result);
}

Result Outlet::read(Blob& blob) {
    return impl_.reader().read(blob);
}

bool Outlet::hasData() {
    return impl_.reader().hasData();
}

bool Outlet::hasObject(ProductType productType) {
    return impl_.reader().hasObject(productType);
}

bool Outlet::canAcceptData() {
    return impl_.writer().canAcceptData();
}

Result Outlet::write(const TObject& object) {
    return impl_.writer().write(object);
}

Result Outlet::write(const Result& result) {
    return impl_.writer().write(result);
}

Result Outlet::write(const Blob& blob) {
    return impl_.writer().write(blob);
}

bool Outlet::operator==(const Outlet& other) const {
    //TODO for now this is simple name matching for the purpose of finding Outlets
    return name() == other.name();
}

bool Outlet::serialize(base::Blob& blob) {
    StructuredData sd("Outlet");

    sd.add("name", name_);
    
    return sd.serialize(blob);
}

bool Outlet::deserialize(const base::Blob& blob) {
    StructuredData sd("Outlet", blob.getString());
    std::string name;
    if (!sd.get("name", name)) {
        return false;
    }
    
    name_ = name;
    return true;
}
