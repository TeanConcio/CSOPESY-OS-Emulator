#include "FCFSScheduler.h"
#include <algorithm>
#include "GlobalScheduler.h"



void FCFSScheduler::run()
{
	for (int core = 0; core < GlobalScheduler::getCoreCount(); core++)
	{
		// If the core is not running a process, assign it one
		if (!ProcessManager::getQueuedProcesses().empty() &&
			GlobalScheduler::getCoreThread(core)->getCurrentProcess() == nullptr)
		{
			std::shared_ptr<Process> process = ProcessManager::getQueuedProcesses().front();
			ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());
			// Set the arrival time of the process to the current time
			process->setArrivalTime(std::time(nullptr));
			GlobalScheduler::getCoreThread(core)->setCurrentProcess(process);
		}
		// If the core is running a process, check if it has finished
		else if (GlobalScheduler::getCoreThread(core)->getCurrentProcess() != nullptr)
		{
			std::shared_ptr<Process> process = GlobalScheduler::getCoreThread(core)->getCurrentProcess();
			if (process->getState() == Process::ProcessState::FINISHED)
			{
				GlobalScheduler::getCoreThread(core)->setCurrentProcess(nullptr);
				ProcessManager::getFinishedProcesses().push_back(process);

				// If there are still processes in the queue, assign the core a new process
				if (!ProcessManager::getQueuedProcesses().empty())
				{
					std::shared_ptr<Process> newProcess = ProcessManager::getQueuedProcesses().front();
					ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());
					newProcess->setArrivalTime(std::time(nullptr));
					GlobalScheduler::getCoreThread(core)->setCurrentProcess(newProcess);
				}
			}
		}
	}
}



// Sort the currentProcess queues based on the remaining instructions (FCFS)
//void FCFSScheduler::sortProcessQueues() {
//	std::sort(ProcessManager::getQueuedProcesses().begin(), ProcessManager::getQueuedProcesses().end(), [](const std::shared_ptr<Process>& p1, const std::shared_ptr<Process>& p2) {
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
