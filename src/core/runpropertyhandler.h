#pragma once
//
//  runpropertyhandler.h
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include "base/propertyhandler.h"

namespace aft {
namespace base {
// Forward reference
class ConsumerContract;
class ProcContract;
class ProducerContract;
}
namespace core {
// Forward reference
class Outlet;
class RunPropertyHandlerImpl;
class TestCase;

    
class RunPropertyHandler : public aft::base::PropertyHandler {
public:
    RunPropertyHandler(const std::string& name, TestCase* testCase = nullptr);
    virtual ~RunPropertyHandler();

    /** Get the singleton global RunPropertyHandler.
     *
     *  This is used when no local Context or RunPropertyHandler is provided.
     */
    static RunPropertyHandler* global();

    void setTestCase(TestCase* testCase);

    void addOutlet(const std::string& name, Outlet* outlet);
    Outlet* getOutlet(const std::string& name);
    void removeOutlet(const std::string& name);
    
    void addConsumer(const std::string& name, base::ConsumerContract* consumer);
    void addProducer(const std::string& name, base::ProducerContract* producer);
    void addProcess(const std::string& name, base::ProcContract* process);
    
    base::ConsumerContract* getConsumer(const std::string& name);
    base::ProducerContract* getProducer(const std::string& name);
    base::ProcContract* getProcess(const std::string& name);
    
    /** Get the result of the last command */
    base::Result& getLastResult() const;
    /** Set the last result */
    void setLastResult(const base::Result& result);
    
    void removeConsumer(const std::string& name);
    void removeProducer(const std::string& name);
    void removeProcess(const std::string& name);
    
    void setupLogs(const std::string& logConfig);
    
    // gui, dispositions
    // Need RunVisitor
private:
    std::unique_ptr<RunPropertyHandlerImpl> impl_;
};

} // namespace core
} // namespace aft
