#pragma once

#include <algorithm>

#include "TypedefRepo.h"
#include "CPUCoreThread.h"
#include "AScheduler.h"
#include "GlobalScheduler.h"

class AScheduler;
class GlobalScheduler;



// First-Come-First-Serve Scheduler
class FCFSScheduler : public AScheduler
{
public:

	FCFSScheduler() : AScheduler(AScheduler::SchedulingAlgorithm::FCFS) {}

	//void sortProcessQueues();


protected:
	void run() override;
};
