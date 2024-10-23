#include "RRScheduler.h"
#include <algorithm>
#include "GlobalScheduler.h"



void RRScheduler::run()
{
	for (int core = 0; core < GlobalScheduler::getCoreCount(); core++)
	{
		// If the core is not running a process, assign it one
		if (!this->queuedProcesses.empty() &&
			GlobalScheduler::getCoreThread(core)->getCurrentProcess() == nullptr)
		{
			std::shared_ptr<Process> process = this->queuedProcesses.front();
			this->queuedProcesses.erase(this->queuedProcesses.begin());

			// Set the arrival time of the process to the current time
			process->setArrivalTime(std::time(nullptr));
			GlobalScheduler::getCoreThread(core)->resetQuantumCycle();
			GlobalScheduler::getCoreThread(core)->setCurrentProcess(process);
		}
		// If the core is running a process, check if it has finished or if it has reached the quantum cycle limit
		else if (GlobalScheduler::getCoreThread(core)->getCurrentProcess() != nullptr)
		{
			std::shared_ptr<Process> process = GlobalScheduler::getCoreThread(core)->getCurrentProcess();

			// If the process has finished, preempt a new process to the core
			if (process->getState() == Process::ProcessState::FINISHED)
			{
				// Remove the process from the core and add it to the finished processes list
				GlobalScheduler::getCoreThread(core)->setCurrentProcess(nullptr);
				this->finishedProcesses.push_back(process);
				// If there are queued processes, assign the core a new process and reset the quantum cycle
				if (!this->queuedProcesses.empty())
				{
					std::shared_ptr<Process> newProcess = this->queuedProcesses.front();
					this->queuedProcesses.erase(this->queuedProcesses.begin());
					newProcess->setArrivalTime(std::time(nullptr));
					GlobalScheduler::getCoreThread(core)->resetQuantumCycle();
					GlobalScheduler::getCoreThread(core)->setCurrentProcess(newProcess);
				}
			}

			// If a process is running but quantum cycles has been reached 
			else if (process->getState() == Process::ProcessState::RUNNING &&
				GlobalScheduler::getCoreThread(core)->hasQuantumCyclesLeft() == false)
			{
				// Reset the quantum cycle no matter what
				GlobalScheduler::getCoreThread(core)->resetQuantumCycle();

				// Only preempt if no other cores are available and there are queued processes
				if (GlobalScheduler::getRunningCoreCount() == GlobalScheduler::getCoreCount() &&
					!this->queuedProcesses.empty())
				{
					// Remove the current process from the core and add it to the queued processes list
					GlobalScheduler::getCoreThread(core)->getCurrentProcess()->setState(Process::ProcessState::READY);
					this->queuedProcesses.push_back(GlobalScheduler::getCoreThread(core)->getCurrentProcess());
					GlobalScheduler::getCoreThread(core)->setCurrentProcess(nullptr);

					// Assign the process in the front of the queued processes list to the core
					std::shared_ptr<Process> newProcess = this->queuedProcesses.front();
					this->queuedProcesses.erase(this->queuedProcesses.begin());
					newProcess->setArrivalTime(std::time(nullptr));
					GlobalScheduler::getCoreThread(core)->resetQuantumCycle();
					GlobalScheduler::getCoreThread(core)->setCurrentProcess(newProcess);
				}
			}

			//// If the process has finished, remove it from the core and add it to the finished processes list
			//if (process->getState() == Process::ProcessState::FINISHED)
			//{
			//	GlobalScheduler::getCoreThread(core)->setCurrentProcess(nullptr);
			//	this->finishedProcesses.push_back(process);
			//}

			//// If there are more CPU cores than queued processes, just reset the CPU quantum cycle
			///*else if (process->getState() == Process::ProcessState::RUNNING && 
			//	this->queuedProcesses.size() <= this->coreThreads.size())
			//{
			//	GlobalScheduler::getCoreThread(core)->resetQuantumCycle();
			//}*/

			//// If the process has reached the quantum cycle limit, remove it from the core and add it to the queued processes list
			//else if (process->getState() == Process::ProcessState::RUNNING &&
			//	GlobalScheduler::getCoreThread(core)->hasQuantumCyclesLeft() == false)
			//{
			//	GlobalScheduler::getCoreThread(core)->getCurrentProcess()->setState(Process::ProcessState::READY);
			//	this->queuedProcesses.push_back(GlobalScheduler::getCoreThread(core)->getCurrentProcess());
			//	GlobalScheduler::getCoreThread(core)->setCurrentProcess(nullptr);
			//}
		}
	}
}
