#include "ProcessManager.h"



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


void ProcessManager::addProcess(std::shared_ptr<Process> process)
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
	auto process = std::make_shared<Process>(ProcessManager::sharedInstance->pidCounter, name, reqFlags);
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
		if (process->getArrivalTime() != 0)
			ss << Common::formatTimeT(process->getArrivalTime()) << "    ";

		ss << "Queued" << "      ";

		ss << process->getCommandCounter() << " / " << process->getLinesOfCode();

		ss << "\n";
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
	std::stringstream ss;

	// TODO: Implement CPU utilization rate (current implementation is a placeholder)
	ss << "CPU utilization: " << "100%\n";
	ss << "Cores used: " << GlobalScheduler::getInstance()->getCoreCount() << "\n";
	ss << "Cores available: " << GlobalScheduler::getInstance()->getCoreCount() - GlobalScheduler::getInstance()->getRunningCoreCount() << "\n\n";

	ss << "--------------------------------------\n";
	ss << "Running processes :\n";
	ss << ProcessManager::makeRunningProcessesString();
	ss << "\n";

	ss << "Queued processes :\n";
	ss << ProcessManager::makeQueuedProcessesString();
	ss << "\n";

	ss << "Finished processes :\n";
	ss << ProcessManager::makeFinishedProcessesString();
	ss << "--------------------------------------\n";

	return ss.str();
}