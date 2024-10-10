#include "FCFSScheduler.h"
#include <algorithm>

FCFSScheduler::FCFSScheduler(int cores) 
	: AScheduler(SchedulingAlgorithm::FCFS, 0, "FCFS")
{
	this->numCores = cores;
	this->processQueues = std::vector<std::unordered_map<String, std::shared_ptr<Process>>>(cores);
}

// Add a process to the scheduler
void FCFSScheduler::addProcess(const Process& process, int core)
{
	if (core >= 0 && core < this->numCores)
	{
		this->processQueues[core][process.getName()] = std::make_shared<Process>(process);
	}
	else 
	{
		std::cerr << "Invalid core specified for process addition.\n";
	}
}

// Sort the process queues based on the remaining instructions (FCFS)
void FCFSScheduler::sortProcessQueues()
{
	for (auto& queue : this->processQueues)
	{
		std::vector<std::shared_ptr<Process>> processList;
		for (auto& entry : queue) {
			processList.push_back(entry.second);
		}

		std::sort(processList.begin(), processList.end(), [](const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) {
			return a->getRemainingTime() > b->getRemainingTime();
		});
	}
}

// Run the scheduler
void FCFSScheduler::runScheduler()
{
	while (!this->processQueues[0].empty())
	{
		for (int core = 0; core < this->numCores; ++core)
		{
			if (!this->processQueues[core].empty())
			{
				auto it = this->processQueues[core].begin();
				std::shared_ptr<Process> currentProcess = it->second;
				this->processQueues[core].erase(it);

				while (!currentProcess->isFinished())
				{
					currentProcess->executeCurrentCommand();
					currentProcess->moveToNextLine();
				}

				std::cout << "Process " << currentProcess->getRemainingTime() << " completed on Core " << core + 1 << ".\n";
			}
		}
	}
}