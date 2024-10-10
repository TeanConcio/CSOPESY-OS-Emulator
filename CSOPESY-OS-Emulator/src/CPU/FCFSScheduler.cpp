#include "FCFSScheduler.h"
#include <algorithm>

FCFSScheduler::FCFSScheduler(int cores)
{
	this->numCores = cores;
	this->processQueues = std::vector<std::vector<Process>>(cores);
}

// Add a process to the scheduler
void FCFSScheduler::addProcess(const Process& process, int core)
{
	if (core >= 0 && core < this->numCores)
	{
		this->processQueues[core].push_back(process);
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
		std::sort(queue.begin(), queue.end(), [](const Process& a, const Process& b) {
			return a.getRemainingTime() > b.getRemainingTime();
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
				Process currentProcess = this->processQueues[core].back();
				processQueues[core].pop_back();

				while (!currentProcess.hasFinished())
				{
					currentProcess.executeCurrentCommand();
					currentProcess.moveToNextLine();
				}

				std::cout << "Process " << currentProcess.getRemainingTime() << " completed on Core " << core + 1 << ".\n";
			}
		}
	}
}