#pragma once

#include <algorithm>

#include "AScheduler.h"

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
