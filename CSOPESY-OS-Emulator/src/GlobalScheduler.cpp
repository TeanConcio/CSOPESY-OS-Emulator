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

void GlobalScheduler::setScheduler()
{
	std::unordered_map<String, String> configs = sharedInstance->getConfigs();
	sharedInstance->setConfigs(configs);
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
		IETThread::sleep(this->delay);
		
		// Join each core thread
		for (int i = 0; i < this->numCores; ++i)
		{
			this->coreThreads[i]->join();
		}

		// Create a new process
		this->createTestProcess();

		// Join the scheduler thread
		this->scheduler->join();
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

			// Set the delays and cores
			sharedInstance->numCores = std::stoi(configs["num-cpu"]);
			sharedInstance->delay = std::stoi(configs["delay-per-exec"]);
			for (int i = 0; i < this->numCores; ++i)
			{
				this->coreThreads.push_back(std::make_shared<CPUCoreThread>(i, timeQuantum));
			}

			// Set the batch process frequency, min instructions, max instructions, and delay
			this->setBatchProcessFreq(std::stoi(configs["batch-process-freq"]));
			this->setMinIns(std::stoi(configs["min-ins"]));
			this->setMaxIns(std::stoi(configs["max-ins"]));
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
		this->setBatchProcessFreq(1);
		this->setMinIns(1000);
		this->setMaxIns(2000);
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


std::shared_ptr<Process> GlobalScheduler::createUniqueProcess(String& name)
{
	std::shared_ptr<Process> existingProcess = scheduler->findProcess(name);
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
		newProcess->test_generateRandomCommands(this->minIns, this->maxIns);
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

void GlobalScheduler::createTestProcess()
{
	if (this->isGeneratingProcesses) {
		this->currBatchProcessCycle++;

		// If the current cycle is a multiple of the batch process frequency, create a new process
		if (this->currBatchProcessCycle % this->batchProcessFreq == 0)
		{
			String processName = "process" + (this->pidCounter < 9 ? std::string("0") : "") + std::to_string(this->pidCounter + 1);
			std::shared_ptr<Process> process = this->createUniqueProcess(processName);
			this->scheduler->addProcess(process);
		}
	}
}

/**
* @brief Create a new process every batchProcessFreq cycles
*/
void GlobalScheduler::generateTestProcesses()
{
	this->isGeneratingProcesses = true;

	/*for (int i = 0; i < limit; ++i)
	{
		String processName = "screen_" + (this->pidCounter < 9 ? std::string("0") : "") + std::to_string(this->pidCounter + 1);
		std::shared_ptr<Process> process = this->createUniqueProcess(processName);
		this->scheduler->addProcess(process);
	}*/
}


void GlobalScheduler::stopGeneratingProcesses()
{
	this->isGeneratingProcesses = false;
}


String GlobalScheduler::makeQueuedProcessesString()
{
	if (sharedInstance->scheduler->queuedProcesses.empty())
		return "No Queued Processes\n";

	std::stringstream ss;

	for (const auto& process : sharedInstance->scheduler->queuedProcesses)
	{
		ss << Common::makeTextCell(11, process->getName(), 'l') << " ";

		// If process has set arrival time, display it
		if (process->getArrivalTime() != 0)
			ss << Common::formatTimeT(process->getArrivalTime()) << "    ";

		ss << "Queued" << "      ";

		ss << process->getCommandCounter() << " / " << process->getLinesOfCode();

		ss << "\n";
	}

	return ss.str();
}


String GlobalScheduler::makeRunningProcessesString()
{
	std::stringstream ss;

	for (const auto& coreThread : sharedInstance->coreThreads)
	{
		std::shared_ptr<Process> process = coreThread->getCurrentProcess();
		if (process != nullptr && process->getState() != Process::ProcessState::FINISHED)
		{
			ss << Common::makeTextCell(11, process->getName(), 'l') << " ";

			ss << Common::formatTimeT(process->getArrivalTime()) << "    ";

			ss << "Core: " << coreThread->getCoreNo() << "      ";

			ss << process->getCommandCounter() << " / " << process->getLinesOfCode();

			ss << "\n";
		}
	}

	if (ss.str().empty())
		return "No Running Processes\n";

	return ss.str();
}


String GlobalScheduler::makeFinishedProcessesString()
{
	if (sharedInstance->scheduler->finishedProcesses.empty())
		return "No Finished Processes\n";

	std::stringstream ss;

	for (const auto& process : sharedInstance->scheduler->finishedProcesses)
	{
		ss << Common::makeTextCell(11, process->getName(), 'l') << " ";

		ss << Common::formatTimeT(process->getArrivalTime()) << "    ";

		ss << "Finished" << "      ";

		ss << process->getCommandCounter() << " / " << process->getLinesOfCode();

		ss << "\n";
	}

	return ss.str();
}
