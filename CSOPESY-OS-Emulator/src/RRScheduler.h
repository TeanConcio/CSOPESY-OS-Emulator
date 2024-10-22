#pragma once

#include "AScheduler.h"
#include "TypedefRepo.h"
#include "CPUCoreThread.h"

// First-Come-First-Serve Scheduler
class RRScheduler : public AScheduler
{
public:

	RRScheduler(int cores, unsigned int delay, unsigned int quantumCycles = 0);

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