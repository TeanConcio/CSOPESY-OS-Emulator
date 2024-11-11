#include "RRScheduler.h"

#include "AScheduler.h"
#include "GlobalScheduler.h"
#include "ProcessManager.h"
#include "MemoryManagementUnit.h"



void RRScheduler::run()
{
	do {
		// Get the cores that have finished processes
		std::vector<std::shared_ptr<CPUCoreThread>> finishedCores = GlobalScheduler::getFinishedCores();
		// For each core that has finished a process
		for (std::shared_ptr<CPUCoreThread> core : finishedCores)
		{
			// Move the process in the core to the finished processes list
			this->moveProcessToFinish(core);

			//core->resetQuantumCycle();
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

				core->resetQuantumCycle();
			}
		}

		// Get the cores that are running a process
		std::vector<std::shared_ptr<CPUCoreThread>> runningCores = GlobalScheduler::getRunningCores();
		// For each core that is running a process
		for (std::shared_ptr<CPUCoreThread> core : runningCores)
		{
			// If quantum cycles has been reached
			if (!core->hasQuantumCyclesLeft())
			{
				// If there are queued processes, assign the core a new process
				if (!ProcessManager::getQueuedProcesses().empty() && hasRunnableProcesses)
				{
					// Remove the process from the core and add it to the back of the queued processes list
					this->moveProcessToQueue(core);

					// Assign the process in the front of the queued processes list to the core
					hasRunnableProcesses = this->assignNextQueuedProcess(core);
				}

				// Temp???
				if (core->getCoreNo() == 0) {
					currentQuantumCycle++;
					String memoryStampFilename = "memory_stamp_" + std::to_string(currentQuantumCycle) + ".txt";
					Common::writeToFile(memoryStampFilename, MemoryManagementUnit::makeMemoryStampString());
				}

				// Reset the quantum cycle
				core->resetQuantumCycle();
			}
		}

		// Create a new process
		ProcessManager::generateTestProcessesLoop();

	} while (GlobalScheduler::isRunning() && GlobalScheduler::MULTI_THREAD_MODE);
}
