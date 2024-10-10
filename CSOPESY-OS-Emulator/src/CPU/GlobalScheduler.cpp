#include "GlobalScheduler.h"
#include "FCFSScheduler.h"

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;

GlobalScheduler::GlobalScheduler() 
	: AScheduler(SchedulingAlgorithm::FCFS, 0, "global")
{

	
}

void GlobalScheduler::init()
{
    sharedInstance = new GlobalScheduler();
}

void GlobalScheduler::execute()
{
	this->scheduler->run();
}

void GlobalScheduler::destroy() 
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
		//Process::RequirementFlags reqFlags = { ProcessRequirementFlags_CONFIG::REQUIRE_FILES, ProcessRequirementFlags_CONFIG::NUM_FILES,
		//	ProcessRequirementFlags_CONFIG::REQUIRE_MEMORY, ProcessRequirementFlags_CONFIG::MEMORY_REQUIRED };
		Process::RequirementFlags reqFlags = { false, 1, false, 0 }; // Placeholder values

		if (name == "")
		{
			name = this->generateProcessName();
		}
		std::shared_ptr<Process> newProcess = std::make_shared<Process>(this->pidCounter, name, reqFlags);
		newProcess->test_generateRandomCommands(100);

		this->scheduler->addProcess(newProcess);
		this->pidCounter++;

		return newProcess;
	}
}

std::shared_ptr<Process> GlobalScheduler::findProcess(String name) const
{
	return this->scheduler->findProcess(name);
}

String GlobalScheduler::generateProcessName() const
{
	std::stringstream ss;
	ss << "Process" << this->pidCounter;
	return ss.str();
}

GlobalScheduler* GlobalScheduler::getInstance()
{
	return sharedInstance;
}

void GlobalScheduler::createTestProcesses()
{
	for (int i = 0; i < 10; ++i)
	{
		String processName = "screen_" + (i < 9 ? std::string("0") : "") + std::to_string(i + 1);
		std::shared_ptr<Process> process = this->createUniqueProcess(processName);

		for (int j = 0; j < 100; ++j)
		{
			process->addCommand(ICommand::PRINT);
		}
	}
}