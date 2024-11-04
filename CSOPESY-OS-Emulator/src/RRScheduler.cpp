#include "RRScheduler.h"
#include <algorithm>
#include "GlobalScheduler.h"



void RRScheduler::run()
{
	do {
		// Get the cores that have finished processes
		std::vector<std::shared_ptr<CPUCoreThread>> finishedCores = GlobalScheduler::getFinishedCores();
		// For each core that has finished a process
		for (std::shared_ptr<CPUCoreThread> core : finishedCores)
		{
			// Remove the process from the core and add it to the finished processes list
			std::shared_ptr<Process> process = core->getCurrentProcess();
			core->setCurrentProcess(nullptr);
			ProcessManager::getFinishedProcesses().push_back(process);
			core->resetQuantumCycle();
		}

		// Get the cores that are not running a process
		std::vector<std::shared_ptr<CPUCoreThread>> emptyCores = GlobalScheduler::getEmptyCores();
		// For each core that is not running a process
		for (std::shared_ptr<CPUCoreThread> core : emptyCores)
		{
			// Assign the core a process from the front of the queued processes list
			if (!ProcessManager::getQueuedProcesses().empty()) 
			{
				std::shared_ptr<Process> process = ProcessManager::getQueuedProcesses().front();
				ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());
				process->setArrivalTime(std::time(nullptr));
				core->setCurrentProcess(process);
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
				// Reset the quantum cycle
				core->resetQuantumCycle();
				// If there are queued processes, assign the core a new process
				if (!ProcessManager::getQueuedProcesses().empty())
				{
					// Remove the process from the core and add it to the queued processes list
					core->getCurrentProcess()->setState(Process::ProcessState::READY);
					ProcessManager::getQueuedProcesses().push_back(core->getCurrentProcess());
					core->setCurrentProcess(nullptr);
					// Assign the process in the front of the queued processes list to the core
					std::shared_ptr<Process> process = ProcessManager::getQueuedProcesses().front();
					ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());
					process->setArrivalTime(std::time(nullptr));
					core->setCurrentProcess(process);
				}
			}
		}

		// Create a new process
		ProcessManager::generateTestProcessesLoop();

	} while (GlobalScheduler::isRunning() && GlobalScheduler::MULTI_THREAD_MODE);

	//for (int core = 0; core < GlobalScheduler::getCoreCount(); core++)
	//{
	//	// If the core is not running a process, assign it one
	//	if (!ProcessManager::getQueuedProcesses().empty() &&
	//		GlobalScheduler::getCoreThread(core)->getCurrentProcess() == nullptr)
	//	{
	//		std::shared_ptr<Process> process = ProcessManager::getQueuedProcesses().front();
	//		ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());

	//		// Set the arrival time of the process to the current time
	//		process->setArrivalTime(std::time(nullptr));
	//		GlobalScheduler::getCoreThread(core)->resetQuantumCycle();
	//		GlobalScheduler::getCoreThread(core)->setCurrentProcess(process);
	//	}
	//	// If the core is running a process, check if it has finished or if it has reached the quantum cycle limit
	//	else if (GlobalScheduler::getCoreThread(core)->getCurrentProcess() != nullptr)
	//	{
	//		std::shared_ptr<Process> process = GlobalScheduler::getCoreThread(core)->getCurrentProcess();

	//		// If the process has finished, preempt a new process to the core
	//		if (process->getState() == Process::ProcessState::FINISHED)
	//		{
	//			// Remove the process from the core and add it to the finished processes list
	//			GlobalScheduler::getCoreThread(core)->setCurrentProcess(nullptr);
	//			ProcessManager::getFinishedProcesses().push_back(process);
	//			// If there are queued processes, assign the core a new process and reset the quantum cycle
	//			if (!ProcessManager::getQueuedProcesses().empty())
	//			{
	//				std::shared_ptr<Process> newProcess = ProcessManager::getQueuedProcesses().front();
	//				ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());
	//				newProcess->setArrivalTime(std::time(nullptr));
	//				GlobalScheduler::getCoreThread(core)->resetQuantumCycle();
	//				GlobalScheduler::getCoreThread(core)->setCurrentProcess(newProcess);
	//			}
	//		}

	//		// If a process is running but quantum cycles has been reached 
	//		else if (process->getState() == Process::ProcessState::RUNNING &&
	//			GlobalScheduler::getCoreThread(core)->hasQuantumCyclesLeft() == false)
	//		{
	//			// Reset the quantum cycle no matter what
	//			GlobalScheduler::getCoreThread(core)->resetQuantumCycle();

	//			// Only preempt if no other cores are available and there are queued processes
	//			if (GlobalScheduler::getRunningCoreCount() == GlobalScheduler::getCoreCount() &&
	//				!ProcessManager::getQueuedProcesses().empty())
	//			{
	//				// Remove the current process from the core and add it to the queued processes list
	//				GlobalScheduler::getCoreThread(core)->getCurrentProcess()->setState(Process::ProcessState::READY);
	//				ProcessManager::getQueuedProcesses().push_back(GlobalScheduler::getCoreThread(core)->getCurrentProcess());
	//				GlobalScheduler::getCoreThread(core)->setCurrentProcess(nullptr);

	//				// Assign the process in the front of the queued processes list to the core
	//				std::shared_ptr<Process> newProcess = ProcessManager::getQueuedProcesses().front();
	//				ProcessManager::getQueuedProcesses().erase(ProcessManager::getQueuedProcesses().begin());
	//				newProcess->setArrivalTime(std::time(nullptr));
	//				GlobalScheduler::getCoreThread(core)->resetQuantumCycle();
	//				GlobalScheduler::getCoreThread(core)->setCurrentProcess(newProcess);
	//			}
	//		}
	//	}
	//}
}
