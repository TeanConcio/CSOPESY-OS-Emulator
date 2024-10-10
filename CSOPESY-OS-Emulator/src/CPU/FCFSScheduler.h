#pragma once

#include "AScheduler.h"
#include "GlobalScheduler.h"
#include "../TypedefRepo.h"

// First-Come-First-Serve Scheduler
class FCFSScheduler : public AScheduler
{
public:

	FCFSScheduler(int cores);

	void addProcess(const Process& process, int core = 0);
	void sortProcessQueues();

protected:
	void run() override;

private:
	int numCores;
	std::vector<std::unordered_map<String, std::shared_ptr<Process>>> processQueues; // One queue per core
};