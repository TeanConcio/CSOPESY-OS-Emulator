#pragma once

#include <algorithm>

#include "AScheduler.h"

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
