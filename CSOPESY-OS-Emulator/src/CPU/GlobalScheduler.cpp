#include "GlobalScheduler.h"

GlobalScheduler::GlobalScheduler(AScheduler* scheduler)
    : AScheduler(SchedulingAlgorithm::ROUND_ROBIN, 0, "GlobalScheduler"), scheduler(scheduler) {}

void GlobalScheduler::initialize() 
{
    sharedInstance = new GlobalScheduler();
}

void GlobalScheduler::destory() 
{
    delete sharedInstance;
}

void GlobalScheduler::tick() const 
{
	this->scheduler->execute();
}

std::shared_ptr<Process> GlobalScheduler::createUniqueProcess(String name) {
	return std::make_shared<Process>(0, name, RequirementFlags::NONE);
}
