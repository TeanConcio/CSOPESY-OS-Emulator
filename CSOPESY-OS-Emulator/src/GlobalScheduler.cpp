#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"


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


void GlobalScheduler::startGlobalScheduler()
{
	sharedInstance->running = true;
	sharedInstance->start();
}


void GlobalScheduler::run()
{
	while (this->running)
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

		// Create a new process
		ProcessManager::generateTestProcessesLoop();

		// Print queue , running, and finished processes (For debugging purposes)
		//std::cout << "Queued Processes:\n" << this->makeQueuedProcessesString() << std::endl;
		//std::cout << "Running Processes:\n" << this->makeRunningProcessesString() << std::endl;
		//std::cout << "Finished Processes:\n" << this->makeFinishedProcessesString() << std::endl;
	}
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


void GlobalScheduler::setScheduler()
{
	std::unordered_map<String, String> configs = sharedInstance->getConfigs();
	sharedInstance->setConfigs(configs);
}


/**
* @brief Get the configs from the configs.txt file
* This includes the ff:
* - Number of CPU cores
* - Scheduler algorithm (FCFS or RR)
* - Time quantum
* - Number of processes to create in scheduler-test per cycle
* - Minimum number of instructions per process
* - Maximum number of instructions per process
* - Delay before executing the next instruction (busy-waiting style)
* 
* @return std::unordered_map<String, String>
*/ 
std::unordered_map<String, String> GlobalScheduler::getConfigs()
{
	try {
		std::ifstream file("config.txt");
		std::unordered_map<String, String> configs;
		if (file.is_open())
		{
			String line;
			while (std::getline(file, line))
			{
				std::istringstream iss(line);
				String parameter, value;
				iss >> parameter >> value;
				configs[parameter] = value;
			}
			file.close();
		}
		return configs;
	} 
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

/**
* @brief Set the configs from the configs.txt file
* 
* @param configs - hash map of the configs from the configs.txt file
* 
*/
void GlobalScheduler::setConfigs(std::unordered_map<String, String> configs)
{
	try {
		// Check if the configs are valid
		validateConfigs(configs);

		// Set the algo and time slice
		String schedulingAlgorithm = configs["scheduler"];
		int timeQuantum = 0;

		AScheduler::SchedulingAlgorithm algo;
		if (schedulingAlgorithm == "\"fcfs\"")
		{
			this->scheduler = new FCFSScheduler();
		}
		else if (schedulingAlgorithm == "\"rr\"")
		{
			this->scheduler = new RRScheduler();
			timeQuantum = std::stoul(configs["quantum-cycles"]);
		}

		// Set the cores and delays
		sharedInstance->numCores = std::stoul(configs["num-cpu"]);
		unsigned int delayPerExecution = std::stoul(configs["delay-per-exec"]);
		for (int i = 0; i < this->numCores; ++i)
		{
			this->coreThreads.push_back(std::make_shared<CPUCoreThread>(i, delayPerExecution, timeQuantum));
		}

		// Set the batch process frequency, min instructions, max instructions
		ProcessManager::sharedInstance->batchProcessFreq = std::stoul(configs["batch-process-freq"]);
		ProcessManager::sharedInstance->minIns = std::stoul(configs["min-ins"]);
		ProcessManager::sharedInstance->maxIns = std::stoul(configs["max-ins"]);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << ". Will set default configs." << std::endl;
		// Set default values
		// num-cpu: 4
		// scheduler: rr
		// quantum-cycles: 5
		// batch-process-freq: 1
		// min-ins: 1000
		// max-ins: 2000
		// delay-per-exec: 0

		this->scheduler = new RRScheduler();
		// Set the cores and delays
		sharedInstance->numCores = 4;
		for (int i = 0; i < this->numCores; ++i)
		{
			this->coreThreads.push_back(std::make_shared<CPUCoreThread>(i, 100000000, 5));
		}
		ProcessManager::sharedInstance->batchProcessFreq = 1;
		ProcessManager::sharedInstance->minIns = 1000;
		ProcessManager::sharedInstance->maxIns = 2000;
	}	
}

void GlobalScheduler::validateConfigs(std::unordered_map<String, String>& configs)
{
	try {
		// Check if num-cpu is not in range [1, 128] or does not exist
		if (configs.find("num-cpu") == configs.end() || std::stoul(configs["num-cpu"]) < 1 || std::stoul(configs["num-cpu"]) > 128)
		{
			configs["num-cpu"] = "4";
		}
	}
	catch (const std::exception&) {
		configs["num-cpu"] = "4";
	}

	try {
		// Check if the scheduler is not "fcfs" or "rr"
		if (configs.find("scheduler") == configs.end() || (configs["scheduler"] != "\"fcfs\"" && configs["scheduler"] != "\"rr\""))
		{
			configs["scheduler"] = "rr";
		}
	}
	catch (const std::exception&) {
		configs["scheduler"] = "rr";
	}

	try {
		// Check if quantum-cycles is not in range [1, 2e32] or does not exist
		if (configs.find("quantum-cycles") == configs.end() || std::stoul(configs["quantum-cycles"]) < 1)
		{
			configs["quantum-cycles"] = "5";
		}
	}
	catch (const std::exception&) {
		configs["quantum-cycles"] = "5";
	}

	try {
		// Check if batch-process-freq is not in range [1, 2e32] or does not exist
		if (configs.find("batch-process-freq") == configs.end() || std::stoul(configs["batch-process-freq"]) < 1)
		{
			configs["batch-process-freq"] = "1";
		}
	}
	catch (const std::exception&) {
		configs["batch-process-freq"] = "1";
	}

	try {
		// Check if min-ins is not in range [1, 2e32] or does not exist
		if (configs.find("min-ins") == configs.end() || std::stoul(configs["min-ins"]) < 1)
		{
			configs["min-ins"] = "1000";
		}
	}
	catch (const std::exception&) {
		configs["min-ins"] = "1000";
	}

	try {
		// Check if max-ins is not in range [1, 2e32] or does not exist
		if (configs.find("max-ins") == configs.end() || std::stoul(configs["max-ins"]) < 1)
		{
			configs["max-ins"] = "2000";
		}
	}
	catch (const std::exception&) {
		configs["max-ins"] = "2000";
	}

	try {
		// Check if delay-per-exec is not in range [0, 2e32] or does not exist
		if (configs.find("delay-per-exec") == configs.end() || std::stoul(configs["delay-per-exec"]) < 0)
		{
			configs["delay-per-exec"] = "0";
		}
	}
	catch (const std::exception&) {
		configs["delay-per-exec"] = "0";
	}
}

