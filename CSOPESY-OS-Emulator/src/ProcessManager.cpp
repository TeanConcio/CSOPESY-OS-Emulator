#include "ProcessManager.h"

#include "MemoryManagementUnit.h"

ProcessManager* ProcessManager::sharedInstance = nullptr;


void ProcessManager::initialize()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new ProcessManager();
	}
}


void ProcessManager::destroy()
{
	delete sharedInstance;
	sharedInstance = nullptr;
}


/**
* @brief Set the configs from the configs.txt file
* @param configs - hash map of the configs from the configs.txt file
*/
void ProcessManager::setConfigs(std::unordered_map<String, String> configs)
{
	try {
		// Set the batch process frequency, min instructions, max instructions
		ProcessManager::sharedInstance->batchProcessFreq = std::stoul(configs["batch-process-freq"]);
		ProcessManager::sharedInstance->minIns = std::stoul(configs["min-ins"]);
		ProcessManager::sharedInstance->maxIns = std::stoul(configs["max-ins"]);

		// Get the memory per process
		size_t minMemPerProc = std::stoul(configs["min-mem-per-proc"]);
		size_t maxMemPerProc = std::stoul(configs["max-mem-per-proc"]);

		// Get their 2^x values
		ProcessManager::sharedInstance->minMemPerProcExp = static_cast<size_t>(std::log2(minMemPerProc));
		ProcessManager::sharedInstance->maxMemPerProcExp = static_cast<size_t>(std::log2(maxMemPerProc));
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << ". Will set default configs." << std::endl;
		ProcessManager::setDefaultConfigs();
	}
}


/**
* @brief Set the default scheduler and configs
*/
void ProcessManager::setDefaultConfigs()
{
	// Set the batch process frequency, min instructions, max instructions
	ProcessManager::sharedInstance->batchProcessFreq = 1;
	ProcessManager::sharedInstance->minIns = 1000;
	ProcessManager::sharedInstance->maxIns = 2000;
	ProcessManager::sharedInstance->minMemPerProcExp = 12;
	ProcessManager::sharedInstance->maxMemPerProcExp = 32;
}



void ProcessManager::addProcess(std::shared_ptr<Process> process) const
{
	sharedInstance->processMap[process->getName()] = process;
	sharedInstance->queuedProcesses.push_back(process);
}


std::shared_ptr<Process> ProcessManager::findProcess(const String& processName)
{
	// Find the process in the process map
	if (sharedInstance->processMap.find(processName) != sharedInstance->processMap.end())
	{
		return sharedInstance->processMap[processName];
	}

	return nullptr;
}


String ProcessManager::generateProcessName() const
{
	std::stringstream ss;
	ss << "Process" << this->pidCounter;
	return ss.str();
}

size_t ProcessManager::generateMemorySize() const
{
	// Generate a random memory size between min and max memory per process
	size_t memSize = static_cast<size_t>(std::pow(2, minMemPerProcExp + (rand() % (maxMemPerProcExp - minMemPerProcExp + 1))));
	return memSize;
}


std::shared_ptr<Process> ProcessManager::createUniqueProcess(String& name)
{
	// Check if process with the same name already exists
	if (ProcessManager::findProcess(name) != nullptr)
		return nullptr;

	// Requirement flags for the process
	//Process::RequirementFlags reqFlags = { 
	// ProcessRequirementFlags_CONFIG::REQUIRE_FILES, 
	// ProcessRequirementFlags_CONFIG::NUM_FILES,
	//	ProcessRequirementFlags_CONFIG::REQUIRE_MEMORY, 
	// ProcessRequirementFlags_CONFIG::MEMORY_REQUIRED };
	Process::RequirementFlags reqFlags = { false, 1, false, 0 }; // Placeholder values

	// If name is empty, generate a new name
	if (name == "")
		name = ProcessManager::sharedInstance->generateProcessName();
	
	// Create Process
	auto process = std::make_shared<Process>(ProcessManager::sharedInstance->pidCounter, name, ProcessManager::sharedInstance->generateMemorySize(), reqFlags);
	process->test_generateRandomCommands(ProcessManager::sharedInstance->minIns, ProcessManager::sharedInstance->maxIns);

	// Create and register screen for the process
	auto screen = std::make_shared<BaseScreen>(process);
	if (ConsoleManager::getInstance()->registerScreen(screen) == nullptr)
		return nullptr;

	// Add process to the process manager
	ProcessManager::sharedInstance->addProcess(process);
	ProcessManager::sharedInstance->pidCounter++;
	
	return process;
}


void ProcessManager::generateTestProcessesLoop()
{
	if (ProcessManager::sharedInstance == nullptr)
		return;

	if (ProcessManager::sharedInstance->isGeneratingProcesses) {
		// If the current cycle is a multiple of the batch process frequency, create a new process
		if (ProcessManager::sharedInstance->currBatchProcessCycle % ProcessManager::sharedInstance->batchProcessFreq == 0)
		{
			String processName = "process" + (ProcessManager::sharedInstance->pidCounter < 9 ? std::string("0") : "") + std::to_string(ProcessManager::sharedInstance->pidCounter + 1);
			ProcessManager::createUniqueProcess(processName);

			std::this_thread::sleep_for(std::chrono::microseconds(1000));
		}

		// Increment the current batch process cycle
		ProcessManager::sharedInstance->currBatchProcessCycle++;
	}
}


String ProcessManager::makeRunningProcessesString()
{
	std::stringstream ss;

	for (int coreNo = 0; coreNo < GlobalScheduler::getCoreCount(); ++coreNo)
	{
		std::shared_ptr<CPUCoreThread> coreThread = GlobalScheduler::getCoreThread(coreNo);
		if (coreThread == nullptr)
			continue;

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


String ProcessManager::makeQueuedProcessesString()
{
	if (ProcessManager::sharedInstance->queuedProcesses.empty())
		return "No Queued Processes\n";

	std::stringstream ss;

	for (const auto& process : ProcessManager::sharedInstance->queuedProcesses)
	{
		ss << Common::makeTextCell(11, process->getName(), 'l') << " ";

		// If process has set arrival time, display it
		try {
			if (process->getArrivalTime() != 0)
				ss << Common::formatTimeT(process->getArrivalTime()) << "    ";

			ss << "Queued" << "      ";

			ss << process->getCommandCounter() << " / " << process->getLinesOfCode();

			ss << "\n";
		}
		catch (std::exception& e) 
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}

	return ss.str();
}


String ProcessManager::makeFinishedProcessesString()
{
	if (ProcessManager::sharedInstance->finishedProcesses.empty())
		return "No Finished Processes\n";

	std::stringstream ss;

	for (const auto& process : ProcessManager::sharedInstance->finishedProcesses)
	{
		ss << Common::makeTextCell(11, process->getName(), 'l') << " ";

		ss << Common::formatTimeT(process->getArrivalTime()) << "    ";

		ss << "Finished" << "      ";

		ss << process->getCommandCounter() << " / " << process->getLinesOfCode();

		ss << "\n";
	}

	return ss.str();
}


String ProcessManager::makeListProcessesString()
{
	// If there is no GlobalScheduler instance, return an empty string
	if (GlobalScheduler::getInstance() == nullptr)
		return "";
	
	std::stringstream ss;

	//ss << MemoryManagementUnit::makeMemoryStampString();
	//ss << "\n";

	//ss << "--------------------------------------\n";
	
	ss << "CPU utilization: " << (GlobalScheduler::getRunningCoreCount() * 100 / GlobalScheduler::getCoreCount()) << "%\n";
	ss << "Cores used: " << GlobalScheduler::getCoreCount() << "\n";
	ss << "Cores available: " << GlobalScheduler::getCoreCount() - GlobalScheduler::getRunningCoreCount() << "\n\n";

	ss << "--------------------------------------\n";
	ss << "Running processes :\n";
	ss << ProcessManager::makeRunningProcessesString();
	ss << "\n";

	//ss << "Queued processes :\n";
	//ss << ProcessManager::makeQueuedProcessesString();
	//ss << "\n";

	ss << "Finished processes :\n";
	ss << ProcessManager::makeFinishedProcessesString();
	ss << "--------------------------------------\n";

	return ss.str();
}

//String ProcessManager::getMemoryUsagePerProcess() {
//	std::stringstream ss;
//	sharedInstance->totalMemoryUsed = 0; // Reset total memory used
//
//	for (int coreNo = 0; coreNo < GlobalScheduler::getCoreCount(); ++coreNo) {
//		std::shared_ptr<CPUCoreThread> coreThread = GlobalScheduler::getCoreThread(coreNo);
//		if (coreThread == nullptr)
//			continue;
//
//		std::shared_ptr<Process> process = coreThread->getCurrentProcess();
//		if (process != nullptr && process->getState() != Process::ProcessState::FINISHED) {
//			// Get memory usage in bytes for the current process using its name
//			size_t memoryUsageBytes = MemoryManagementUnit::getProcessMemoryUsage(process->getName());
//			double memoryUsageMiB = static_cast<double>(memoryUsageBytes) / 1024.0;
//
//			// Add to total memory used
//			sharedInstance->totalMemoryUsed += memoryUsageBytes;
//
//			// Format and append the process name and memory usage
//			ss << process->getName() << " " << std::fixed << std::setprecision(0) << memoryUsageMiB << " MiB\n";
//		}
//	}
//
//	if (ss.str().empty())
//		return "No Running Processes\n";
//
//	return ss.str();
//}