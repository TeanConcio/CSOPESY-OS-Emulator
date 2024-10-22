#include "GlobalScheduler.h"
#include "FCFSScheduler.h"


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

void GlobalScheduler::start()
{
	sharedInstance->running = true;
	sharedInstance->scheduler->start();
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
	this->isGeneratingProcesses = true;
}

/**
* @brief Create a new process every batchProcessFreq cycles
*/
void GlobalScheduler::generateTestProcesses()
{
	this->isGeneratingProcesses = true;

	this->processGenerationThread = std::thread([this]() {
		while (this->isGeneratingProcesses)
		{
			// Sleep for a second to simulate a cycle
			for (int i = 0; i < this->getBatchProcessFreq(); i++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}

			String processName = "process" + (this->pidCounter < 9 ? std::string("0") : "") + std::to_string(this->pidCounter + 1);
			std::shared_ptr<Process> process = this->createUniqueProcess(processName);
			this->scheduler->addProcess(process);
			
		}
	});

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
	if (this->processGenerationThread.joinable())
	{
		this->processGenerationThread.join();
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
		// Check if the configs are valid
		if (areConfigsValid(configs))
		{
			// Set the algo, cores, and time slice
			String schedulingAlgorithm = configs["scheduler"];
			int numCores = std::stoi(configs["num-cpu"]);
			int timeQuantum = std::stoi(configs["quantum-cycles"]);
			int delay = std::stoi(configs["delay-per-exec"]);

			AScheduler::SchedulingAlgorithm algo;
			if (schedulingAlgorithm == "\"fcfs\"")
			{
				algo = AScheduler::SchedulingAlgorithm::FCFS;
				this->scheduler = new FCFSScheduler(numCores, algo, delay);
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
		this->scheduler = new FCFSScheduler(4, AScheduler::SchedulingAlgorithm::FCFS, 0);
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