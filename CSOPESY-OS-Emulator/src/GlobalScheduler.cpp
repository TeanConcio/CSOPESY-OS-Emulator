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
		// Run the scheduler
		this->scheduler->start();

		// Run each core thread
		for (int i = 0; i < this->numCores; ++i)
		{
			this->coreThreads[i]->start();
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
		if (areConfigsValid(configs))
		{
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
				timeQuantum = std::stoi(configs["quantum-cycles"]);
			}

			// Set the cores and delays
			sharedInstance->numCores = std::stoi(configs["num-cpu"]);
			unsigned int delayPerExecution = std::stoi(configs["delay-per-exec"]);
			for (int i = 0; i < this->numCores; ++i)
			{
				this->coreThreads.push_back(std::make_shared<CPUCoreThread>(i, delayPerExecution, timeQuantum));
			}

			// Set the batch process frequency, min instructions, max instructions, and delay
			ProcessManager::sharedInstance->batchProcessFreq = std::stoi(configs["batch-process-freq"]);
			ProcessManager::sharedInstance->minIns = std::stoi(configs["min-ins"]);
			ProcessManager::sharedInstance->maxIns = std::stoi(configs["max-ins"]);
		}
		else
		{
			// Throws an exception if the configs are invalid and sets default values
			throw std::invalid_argument("Invalid configs.");
		}
		
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
		this->scheduler = new FCFSScheduler();
		ProcessManager::sharedInstance->batchProcessFreq = 1;
		ProcessManager::sharedInstance->minIns = 1000;
		ProcessManager::sharedInstance->maxIns = 2000;
	}	
}

bool GlobalScheduler::areConfigsValid(std::unordered_map<String, String> configs)
{
	// Check if the only components are num-cpu, scheduler, quantum-cycles, batch-process-freq, min-ins, max-ins, and delay-per-exec
	if (configs.size() != 7)
	{
		return false;
	}
	// Check if the configs are valid
	if (configs.find("num-cpu") == configs.end() || configs.find("scheduler") == configs.end() || configs.find("quantum-cycles") == configs.end() ||
		configs.find("batch-process-freq") == configs.end() || configs.find("min-ins") == configs.end() || configs.find("max-ins") == configs.end() ||
		configs.find("delay-per-exec") == configs.end())
	{
		return false;
	}
	// Check if the values are valid
	// num-cpu: [1, 128]
	// scheduler: "fcfs" or "rr"
	// quantum-cycles: [1, 2e32]
	// batch-process-freq: [1, 2e32]
	// min-ins: [1, 2e32]
	// max-ins: [1, 2e32]
	// delay-per-exec: [0, 2e32]
	if (std::stoi(configs["num-cpu"]) < 1 || std::stoi(configs["num-cpu"]) > 128)
	{
		return false;
	}
	if (configs["scheduler"] != "\"fcfs\"" && configs["scheduler"] != "\"rr\"")
	{
		return false;
	}
	if (std::stoi(configs["quantum-cycles"]) < 1)
	{
		return false;
	}
	if (std::stoi(configs["batch-process-freq"]) < 1)
	{
		return false;
	}
	if (std::stoi(configs["min-ins"]) < 1)
	{
		return false;
	}
	if (std::stoi(configs["max-ins"]) < 1)
	{
		return false;
	}
	if (std::stoi(configs["delay-per-exec"]) < 0)
	{
		return false;
	}

	return true;
}
