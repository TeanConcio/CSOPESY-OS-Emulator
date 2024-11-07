#include "AScheduler.h"


AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo) : IETThread(GlobalScheduler::MULTI_THREAD_MODE)
{
	this->schedulingAlgo = schedulingAlgo;
}


bool AScheduler::assignNextQueuedProcess(std::shared_ptr<CPUCoreThread> core) const
{
	// Get next runnable process in the ready queue
	// For each process in the ready queue
	for (int i = 0; i < ProcessManager::getQueuedProcesses().size(); i++)
	{
		// Get the process in the front of the queued processes list
		std::shared_ptr<Process> process = ProcessManager::getQueuedProcesses().front();

		// Check if the process has enough memory to run
		if (MemoryManagementUnit::allocate(process.get()) != nullptr)
		{
			// Remove the process from the queued processes list and assign it to the core
			ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());
			process->setArrivalTime(std::time(nullptr));
			core->setCurrentProcess(process);
			return true;
		}
		else
		{
			// Remove the process from the start of the queued processes list and add it to the end
			ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());
			ProcessManager::getQueuedProcesses().push_back(process);
		}
	}

	return false;
}


void AScheduler::moveProcessToQueue(std::shared_ptr<CPUCoreThread> core) const
{
	// Get the process that is running
	std::shared_ptr<Process> process = core->getCurrentProcess();

	// Remove process from memory
	MemoryManagementUnit::deallocate(process.get());

	// Remove the process from the core and add it to the back of the queued processes list
	process->setState(Process::ProcessState::READY);
	ProcessManager::getQueuedProcesses().push_back(process);
	core->setCurrentProcess(nullptr);
}


void AScheduler::moveProcessToFinish(std::shared_ptr<CPUCoreThread> core) const
{
	// Get the process that has finished
	std::shared_ptr<Process> process = core->getCurrentProcess();

	// Remove process from memory
	MemoryManagementUnit::deallocate(process.get());

	// Remove the process from the core and add it to the finished processes list
	core->setCurrentProcess(nullptr);
	ProcessManager::getFinishedProcesses().push_back(process);
}
