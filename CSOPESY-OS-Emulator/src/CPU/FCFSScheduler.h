#pragma once

#include "AScheduler.h"
#include "GlobalScheduler.h"
#include "../TypedefRepo.h"

class FCFSScheduler : public AScheduler
{
public:

	FCFSScheduler(int cores);

	void addProcess(const Process& process, int core = 0);
	void sortProcessQueues();
	void runScheduler();

private:
	int numCores;
	std::vector<std::unordered_map<String, std::shared_ptr<Process>>> processQueues; // One queue per core
};