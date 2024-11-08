#include "GlobalScheduler.h"

#include "Process.h"
#include "ProcessManager.h"
#include "RRScheduler.h"
#include "FCFSScheduler.h"


GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;


void GlobalScheduler::initialize()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new GlobalScheduler();
	}
}


void GlobalScheduler::destroy() 
{
	sharedInstance->running = false;
	delete sharedInstance;
	sharedInstance = nullptr;
}


/**
* @brief Set the configs from the configs.txt file
* @param configs - hash map of the configs from the configs.txt file
*/
void GlobalScheduler::setConfigs(std::unordered_map<String, String> configs)
{
	try {
		// Set the algo and time slice
		String schedulingAlgorithm = configs["scheduler"];
		int timeQuantum = 0;

		if (schedulingAlgorithm == "\"fcfs\"")
		{
			sharedInstance->scheduler = new FCFSScheduler();
		}
		else if (schedulingAlgorithm == "\"rr\"")
		{
			sharedInstance->scheduler = new RRScheduler();
			timeQuantum = std::stoul(configs["quantum-cycles"]);
		}

		// Set the cores and delays
		sharedInstance->numCores = std::stoul(configs["num-cpu"]);
		unsigned int delayPerExecution = std::stoul(configs["delay-per-exec"]);
		for (int i = 0; i < sharedInstance->numCores; ++i)
		{
			sharedInstance->coreThreads.push_back(std::make_shared<CPUCoreThread>(i, delayPerExecution, timeQuantum));
		}

		// Set the batch process frequency, min instructions, max instructions
		ProcessManager::sharedInstance->batchProcessFreq = std::stoul(configs["batch-process-freq"]);
		ProcessManager::sharedInstance->minIns = std::stoul(configs["min-ins"]);
		ProcessManager::sharedInstance->maxIns = std::stoul(configs["max-ins"]);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << ". Will set default configs." << std::endl;
		GlobalScheduler::setDefaultConfigs();
	}
}


/**
* @brief Set the default scheduler and configs
*/
void GlobalScheduler::setDefaultConfigs()
{
	sharedInstance->scheduler = new RRScheduler();
	// Set the cores and delays
	sharedInstance->numCores = 4;
	for (int i = 0; i < sharedInstance->numCores; ++i)
	{
		sharedInstance->coreThreads.push_back(std::make_shared<CPUCoreThread>(i, 100000000, 5));
	}
	// Set the batch process frequency, min instructions, max instructions
	ProcessManager::sharedInstance->batchProcessFreq = 1;
	ProcessManager::sharedInstance->minIns = 1000;
	ProcessManager::sharedInstance->maxIns = 2000;
}


void GlobalScheduler::startGlobalScheduler()
{
	sharedInstance->running = true;

	if (GlobalScheduler::MULTI_THREAD_MODE)
	{
		sharedInstance->run();
	}
	else
	{
		sharedInstance->start();
	}
}


void GlobalScheduler::run()
{
	try {
		// Run the scheduler
		this->scheduler->start();
	}
	catch (std::exception& e) {
		std::cerr << "Error (Scheduler): " << e.what() << std::endl;
	}

	// Run each core thread
	for (int i = 0; i < this->numCores; ++i)
	{
		try {
			this->coreThreads[i]->start();
		}
		catch (std::exception& e) {
			std::cerr << "Error (Core " << i << "): " << e.what() << std::endl;
		}
	}

	// Sleep delay
	// IETThread::sleep(this->delay);
		
	// Join each core thread
	for (int i = 0; i < this->numCores; ++i)
	{
		this->coreThreads[i]->join();
	}

	// Join the scheduler thread
	this->scheduler->join();

	// Print queue , running, and finished processes (For debugging purposes)
	//std::cout << "Queued Processes:\n" << this->makeQueuedProcessesString() << std::endl;
	//std::cout << "Running Processes:\n" << this->makeRunningProcessesString() << std::endl;
	//std::cout << "Finished Processes:\n" << this->makeFinishedProcessesString() << std::endl;
}


int GlobalScheduler::getRunningCoreCount()
{
	// Get the number of cores that are currently running a process
	int runningCores = 0;
	for (int i = 0; i < sharedInstance->numCores; ++i)
	{
		if (sharedInstance->coreThreads[i]->getCurrentProcess() != nullptr)
		{
			runningCores++;
		}
	}

	return runningCores;
}


std::vector<std::shared_ptr<CPUCoreThread>> GlobalScheduler::getFinishedCores()
{
	std::vector<std::shared_ptr<CPUCoreThread>> finishedCores;
	for (int i = 0; i < sharedInstance->numCores; ++i)
	{
		std::shared_ptr<Process> process = sharedInstance->coreThreads[i]->getCurrentProcess();
		if (process != nullptr &&
			process->getState() == Process::ProcessState::FINISHED)
		{
			finishedCores.push_back(sharedInstance->coreThreads[i]);
		}
	}
	return finishedCores;
}


std::vector<std::shared_ptr<CPUCoreThread>> GlobalScheduler::getEmptyCores()
{
	std::vector<std::shared_ptr<CPUCoreThread>> emptyCores;
	for (int i = 0; i < sharedInstance->numCores; ++i)
	{
		std::shared_ptr<Process> process = sharedInstance->coreThreads[i]->getCurrentProcess();
		if (process == nullptr)
		{
			emptyCores.push_back(sharedInstance->coreThreads[i]);
		}
	}
	return emptyCores;
}


std::vector<std::shared_ptr<CPUCoreThread>> GlobalScheduler::getRunningCores()
{
	std::vector<std::shared_ptr<CPUCoreThread>> runningCores;
	for (int i = 0; i < sharedInstance->numCores; ++i)
	{
		std::shared_ptr<Process> process = sharedInstance->coreThreads[i]->getCurrentProcess();
		if (process != nullptr &&
			process->getState() == Process::ProcessState::RUNNING)
		{
			runningCores.push_back(sharedInstance->coreThreads[i]);
		}
	}
	return runningCores;
}
