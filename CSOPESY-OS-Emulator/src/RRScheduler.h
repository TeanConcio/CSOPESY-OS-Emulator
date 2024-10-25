#pragma once

#include "AScheduler.h"
#include "TypedefRepo.h"
#include "CPUCoreThread.h"

// Round robin scheduler
class RRScheduler : public AScheduler
{
public:

	RRScheduler() : AScheduler(AScheduler::SchedulingAlgorithm::ROUND_ROBIN) {}

protected:
	void run() override;
};
