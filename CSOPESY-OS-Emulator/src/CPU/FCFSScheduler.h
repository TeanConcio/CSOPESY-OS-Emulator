#pragma once

#include "AScheduler.h"
#include "../TypedefRepo.h"
#include "../Threading/CPUCoreThread.h"

// First-Come-First-Serve Scheduler
class FCFSScheduler : public AScheduler
{
public:

	FCFSScheduler(int cores);

	//void addProcessToCPUQueue(const Process& process, int core = 0);
	//void sortProcessQueues();

	void startCoreThreads();

	void start() override;

protected:
	void run() override;

private:
	
	//std::vector<std::vector<std::shared_ptr<Process>>> processCPUQueues; // One queue per core
};