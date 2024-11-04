#include "AScheduler.h"


AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo) : IETThread(GlobalScheduler::MULTI_THREAD_MODE)
{
	this->schedulingAlgo = schedulingAlgo;
}
