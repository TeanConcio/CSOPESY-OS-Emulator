#pragma once

#include <algorithm>

#include "Common.h"
#include "AScheduler.h"
#include "TypedefRepo.h"
#include "CPUCoreThread.h"
#include "GlobalScheduler.h"

class AScheduler;



// Round robin scheduler
class RRScheduler : public AScheduler
{
public:

	RRScheduler() : AScheduler(AScheduler::SchedulingAlgorithm::ROUND_ROBIN) {}

protected:
	void run() override;

	// Temp???
	unsigned int currentQuantumCycle = 0;
};
