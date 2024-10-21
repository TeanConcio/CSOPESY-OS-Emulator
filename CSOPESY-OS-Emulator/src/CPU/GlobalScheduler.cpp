#include "GlobalScheduler.h"
#include "FCFSScheduler.h"

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;


GlobalScheduler* GlobalScheduler::getInstance()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new GlobalScheduler();
	}
	return GlobalScheduler::sharedInstance;
}


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
	delete sharedInstance;
	sharedInstance = nullptr;
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
		newProcess->test_generateRandomCommands(100);
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
		this->scheduler->addProcess(process);
	}
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
		// Set the algo, cores, and time slice
		String schedulingAlgorithm = configs["scheduler"];
		int numCores = std::stoi(configs["num-cpu"]);
		int timeQuantum = std::stoi(configs["quantum-cycles"]);

		AScheduler::SchedulingAlgorithm algo;
		if (schedulingAlgorithm == "\"fcfs\"")
		{
			algo = AScheduler::SchedulingAlgorithm::FCFS;
			this->scheduler = new FCFSScheduler(numCores, algo);
		}
		else
		{
			algo = AScheduler::SchedulingAlgorithm::ROUND_ROBIN;
			// TODO: Make an RR class
			//this->scheduler = new RoundRobinScheduler(numCores, timeQuantum);
		}

		// Set the batch process frequency, min instructions, max instructions, and delay
		this->setBatchProcessFreq(std::stoi(configs["batch-process-freq"]));
		this->setMinIns(std::stoi(configs["min-ins"]));
		this->setMaxIns(std::stoi(configs["max-ins"]));
		this->delay = std::stoi(configs["delay-per-exec"]);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}	
}