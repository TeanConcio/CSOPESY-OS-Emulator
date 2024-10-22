#include "FCFSScheduler.h"
#include <algorithm>
#include "GlobalScheduler.h"

FCFSScheduler::FCFSScheduler(int cores, AScheduler::SchedulingAlgorithm schedulingAlgo, unsigned int delay) 
	: AScheduler(schedulingAlgo)
{
	this->numCores = cores;
	
	// Initialize the Core Threads
	for (int i = 0; i < cores; ++i)
	{
		this->coreThreads.push_back(std::make_shared<CPUCoreThread>(i, delay));
	}
}


void FCFSScheduler::startCoreThreads()
{
	for (int i = 0; i < this->numCores; ++i)
	{
		//std::cout << "core " << i << " started" << std::endl;
		this->coreThreads[i]->start();
	}
}


void FCFSScheduler::start()
{
	AScheduler::start();
	this->startCoreThreads();
}


void FCFSScheduler::run()
{
	// bool allProcessesFinished = false;

	while (GlobalScheduler::isRunning())
	{
		// allProcessesFinished = true;

		for (int core = 0; core < this->numCores; core++)
		{
			// If the core is not running a process, assign it one
			if (!this->queuedProcesses.empty() && 
				this->coreThreads[core]->getCurrentProcess() == nullptr)
			{
				std::shared_ptr<Process> process = this->queuedProcesses.front();
				this->queuedProcesses.erase(this->queuedProcesses.begin());
				// Set the arrival time of the process to the current time
				process->setArrivalTime(std::time(nullptr));
				this->coreThreads[core]->setCurrentProcess(process);
				// allProcessesFinished = false;
			}
			// If the core is running a process, check if it has finished
			else if (this->coreThreads[core]->getCurrentProcess() != nullptr)
			{
				std::shared_ptr<Process> process = this->coreThreads[core]->getCurrentProcess();
				if (process->getState() == Process::ProcessState::FINISHED)
				{
					this->coreThreads[core]->setCurrentProcess(nullptr);
					this->finishedProcesses.push_back(process);
				}
				//else
				//{
				//	allProcessesFinished = false;
				//}
			}
		}
	}
}


String FCFSScheduler::makeQueuedProcessesString()
{
	if (this->queuedProcesses.empty()) 
		return "No Queued Processes\n";
	
	std::stringstream ss;

	for (const auto& process : this->queuedProcesses)
	{
		ss << Common::makeTextCell(11, process->getName(), 'l') << " ";
		
		ss << Common::formatTimeT(process->getArrivalTime()) << "    ";

		ss << "Queued" << "      ";

		ss << process->getCommandCounter() << " / " << process->getLinesOfCode();

		ss << "\n";
	}

	return ss.str();
}


String FCFSScheduler::makeRunningProcessesString()
{
	std::stringstream ss;

	for (const auto& coreThread : this->coreThreads)
	{
		std::shared_ptr<Process> process = coreThread->getCurrentProcess();
		if (process != nullptr && coreThread->getCurrentProcessState() != Process::ProcessState::FINISHED)
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


String FCFSScheduler::makeFinishedProcessesString()
{
	if (this->finishedProcesses.empty())
		return "No Finished Processes\n";
	
	std::stringstream ss;

	for (const auto& process : this->finishedProcesses)
	{
		ss << Common::makeTextCell(11, process->getName(), 'l') << " ";

		ss << Common::formatTimeT(process->getArrivalTime()) << "    ";

		ss << "Finished" << "      ";

		ss << process->getCommandCounter() << " / " << process->getLinesOfCode();

		ss << "\n";
	}

	return ss.str();
}



// Sort the currentProcess queues based on the remaining instructions (FCFS)
//void FCFSScheduler::sortProcessQueues() {
//	std::sort(this->queuedProcesses.begin(), this->queuedProcesses.end(), [](const std::shared_ptr<Process>& p1, const std::shared_ptr<Process>& p2) {
//		return p1->getLinesOfCode()-p1->getCommandCounter() < p2->getLinesOfCode() - p2->getCommandCounter();
//	});
//}

//// Run the scheduler
//void FCFSScheduler::run()
//{
//	while (!this->processCPUQueues[0].empty())
//	{
//		for (int core = 0; core < this->numCores; ++core)
//		{
//			if (!this->processCPUQueues[core].empty())
//			{
//				auto it = this->processCPUQueues[core].begin();
//				std::shared_ptr<Process> currentProcess = it->second;
//				this->processCPUQueues[core].erase(it);
//
//				// Run every possible currentProcess in the scheduler
//				while (!currentProcess->isFinished())
//				{
//					currentProcess->executeCurrentCommand();
//					currentProcess->moveToNextLine();
//				}
//
//				std::cout << "Process " << currentProcess->getLastCommandTime() << " completed on Core " << core + 1 << ".\n";
//			}
//		}
//	}
//}
