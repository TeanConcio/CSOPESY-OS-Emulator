#pragma once

#include "AScheduler.h"
#include "../TypedefRepo.h"
#include "../Threading/CPUCoreThread.h"

// First-Come-First-Serve Scheduler
class FCFSScheduler : public AScheduler
{
public:

	FCFSScheduler(int cores);

	void startCoreThreads();
	void start() override;

	String makeQueuedProcessesString() override;
	String makeRunningProcessesString() override;
	String makeFinishedProcessesString() override;

	//void sortProcessQueues();


protected:
	void run() override;

private:
	
	//std::vector<std::vector<std::shared_ptr<Process>>> processCPUQueues; // One queue per core
};