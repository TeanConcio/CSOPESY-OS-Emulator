#pragma once

#include <algorithm>

#include "Common.h"
#include "TypedefRepo.h"
#include "CPUCoreThread.h"
#include "AScheduler.h"
#include "GlobalScheduler.h"

class AScheduler;
class GlobalScheduler;



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
