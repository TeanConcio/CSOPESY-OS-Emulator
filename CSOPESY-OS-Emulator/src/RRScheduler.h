#pragma once

#include "AScheduler.h"
#include "TypedefRepo.h"
#include "CPUCoreThread.h"

// First-Come-First-Serve Scheduler
class RRScheduler : public AScheduler
{
public:

	RRScheduler() : AScheduler(AScheduler::SchedulingAlgorithm::ROUND_ROBIN) {}

protected:
	void run() override;
};
