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
	std::shared_ptr<Process> existingProcess = ProcessManager::findProcess(name);
	if (existingProcess != nullptr)
	{
		return nullptr;
	}
	else
	{
		//Process::RequirementFlags reqFlags = { 
		// ProcessRequirementFlags_CONFIG::REQUIRE_FILES, 
		// ProcessRequirementFlags_CONFIG::NUM_FILES,
		//	ProcessRequirementFlags_CONFIG::REQUIRE_MEMORY, 
		// ProcessRequirementFlags_CONFIG::MEMORY_REQUIRED };

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


void ProcessManager::generateTestProcessesLoop()
{
	if (ProcessManager::sharedInstance == nullptr)
		return;

	if (ProcessManager::sharedInstance->isGeneratingProcesses) {
		// If the current cycle is a multiple of the batch process frequency, create a new process
		if (ProcessManager::sharedInstance->currBatchProcessCycle % ProcessManager::sharedInstance->batchProcessFreq == 0)
		{
			String processName = "process" + (ProcessManager::sharedInstance->pidCounter < 9 ? std::string("0") : "") + std::to_string(ProcessManager::sharedInstance->pidCounter + 1);
			std::shared_ptr<Process> process = ProcessManager::sharedInstance->createUniqueProcess(processName);

			if (process != nullptr)
				ProcessManager::addProcess(process);
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