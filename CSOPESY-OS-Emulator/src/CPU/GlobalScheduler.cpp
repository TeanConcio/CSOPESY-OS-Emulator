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

std::shared_ptr<Process> GlobalScheduler::createUniqueProcess(String name) 
{
    std::shared_ptr<Process> existingProcess = this->findProcess(name);
	if (existingProcess != nullptr)
	{
		return existingProcess;
	}
	else
	{

		std::shared_ptr<Process> newProcess = std::make_shared<Process>(this->pidCounter, name, reqFlags);
		newProcess->test_generateRandomCommands(50);

		this->scheduler->addProcess(newProcess);
		this->pidCounter++;

		return newProcess;
	}
}