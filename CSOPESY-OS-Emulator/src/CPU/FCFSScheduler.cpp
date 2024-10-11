#include "FCFSScheduler.h"
#include <algorithm>


FCFSScheduler::FCFSScheduler(int cores)  
{
	this->numCores = cores;
	this->schedulingAlgo = FCFS;
	
	// Initialize the Core Threads
	for (int i = 0; i < cores; ++i)
	{
		this->coreThreads.push_back(std::make_shared<CPUCoreThread>(i));
	}
}


void FCFSScheduler::addProcess(std::shared_ptr<Process> process)
{
	if (this->findProcess(process->getName()) == nullptr)
	{
		this->queuedProcesses.push_back(process);
	}
	else
	{
		std::cerr << "Process " << process->getName() << " already exists in the scheduler.\n";
	}
}


//// Add a currentProcess to the scheduler
//void FCFSScheduler::addProcessToCPUQueue(const Process& process, int core)
//{
//	if (core >= 0 && core < this->numCores)
//	{
//		this->processCPUQueues[core][process.getName()] = std::make_shared<Process>(process);
//	}
//	else 
//	{
//		std::cerr << "Invalid core specified for process addition.\n";
//	}
//}


// Sort the currentProcess queues based on the remaining instructions (FCFS)
void FCFSScheduler::sortProcessQueues() {
	std::sort(this->queuedProcesses.begin(), this->queuedProcesses.end(), [](const std::shared_ptr<Process>& p1, const std::shared_ptr<Process>& p2) {
		return p1->getLinesOfCode()-p1->getCommandCounter() < p2->getLinesOfCode() - p2->getCommandCounter();
	});
}

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



void FCFSScheduler::startCoreThreads()
{
	for (int i = 0; i < this->numCores; ++i)
	{
		std::cout << "core " << i << " started" << std::endl;
		this->coreThreads[i]->start();
	}
}

void FCFSScheduler::start()
{
	this->startCoreThreads();
	this->run();
}


void FCFSScheduler::run()
{
	while (!this->queuedProcesses.empty())
	{
		for (int core = 0; core < this->numCores; ++core)
		{
			if (!this->coreThreads[core]->getCurrentProcess() && this->coreThreads[core]->getCurrentProcess()->getState() == Process::ProcessState::FINISHED) {
				this->coreThreads[core]->setCurrentProcess(this->queuedProcesses.front());
				this->queuedProcesses.erase(this->queuedProcesses.begin());
			}
		}
	}
}