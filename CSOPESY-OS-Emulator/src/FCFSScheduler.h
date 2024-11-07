#pragma once

#include <algorithm>

#include "AScheduler.h"
#include "TypedefRepo.h"
#include "CPUCoreThread.h"
#include "GlobalScheduler.h"

class AScheduler;



// First-Come-First-Serve Scheduler
class FCFSScheduler : public AScheduler
{
public:

	FCFSScheduler() : AScheduler(AScheduler::SchedulingAlgorithm::FCFS) {}

	//void sortProcessQueues();


protected:
	void run() override;
};