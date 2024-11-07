#include "FCFSScheduler.h"



void FCFSScheduler::run()
{
	do {
		// Get the cores that have finished processes
		std::vector<std::shared_ptr<CPUCoreThread>> finishedCores = GlobalScheduler::getFinishedCores();
		// For each core that has finished a process
		for (std::shared_ptr<CPUCoreThread> core : finishedCores)
		{
			// Move the process in the core to the finished processes list
			this->moveProcessToFinish(core);
		}

		// Variable for checking if there are runnable processes
		bool hasRunnableProcesses = true;

		// Get the cores that are not running a process
		std::vector<std::shared_ptr<CPUCoreThread>> emptyCores = GlobalScheduler::getEmptyCores();
		// For each core that is not running a process
		for (std::shared_ptr<CPUCoreThread> core : emptyCores)
		{
			// If there are still processes in ready queue
			if (!ProcessManager::getQueuedProcesses().empty() && hasRunnableProcesses)
			{
				// Assign the next runnable process in the queued processes list to the core
				// If no process can be assigned, break the loop
				hasRunnableProcesses = this->assignNextQueuedProcess(core);
				if (hasRunnableProcesses == false)
					break;
			}
		}

		// Create a new process
		ProcessManager::generateTestProcessesLoop();

	} while (GlobalScheduler::isRunning() && GlobalScheduler::MULTI_THREAD_MODE);
}


// Sort the currentProcess queues based on the remaining instructions (FCFS)
//void FCFSScheduler::sortProcessQueues() {
//	std::sort(ProcessManager::getQueuedProcesses().begin(), ProcessManager::getQueuedProcesses().end(), [](const std::shared_ptr<Process>& p1, const std::shared_ptr<Process>& p2) {
//		return p1->getLinesOfCode()-p1->getCommandCounter() < p2->getLinesOfCode() - p2->getCommandCounter();
//	});
//}
