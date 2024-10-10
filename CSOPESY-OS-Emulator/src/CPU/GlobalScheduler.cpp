#include "GlobalScheduler.h"
#include "FCFSScheduler.h"


GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;


GlobalScheduler::GlobalScheduler() : AScheduler(SchedulingAlgorithm::FCFS, 0, "global") {}


GlobalScheduler* GlobalScheduler::getInstance()
{
	if (sharedInstance == nullptr) {
		initialize();
	}
	return GlobalScheduler::sharedInstance;
}


void GlobalScheduler::initialize()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new GlobalScheduler();
		sharedInstance->scheduler = new FCFSScheduler(4);
		sharedInstance->createTestProcesses(10);
	}
}


void GlobalScheduler::destroy() 
{
	delete sharedInstance;
	sharedInstance = nullptr;
}


void GlobalScheduler::run() 
{
	this->scheduler->run();
}


void GlobalScheduler::stop() {}


std::shared_ptr<Process> GlobalScheduler::createUniqueProcess(String& name) 
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
		auto newProcess = std::make_shared<Process>(this->pidCounter, name, reqFlags);
		newProcess->test_generateRandomCommands(100);

		this->scheduler->addProcess(newProcess);
		this->pidCounter++;

		return newProcess;
	}
}


String GlobalScheduler::generateProcessName() const
{
	std::stringstream ss;
	ss << "Process" << this->pidCounter;
	return ss.str();
}


void GlobalScheduler::createTestProcesses(const int limit)
{
	for (int i = 0; i < limit; ++i)
	{
		String processName = "screen_" + (i < 9 ? std::string("0") : "") + std::to_string(i + 1);
		std::shared_ptr<Process> process = this->createUniqueProcess(processName);
	}
}