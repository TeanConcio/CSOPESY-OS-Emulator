#include "SchedulerWorker.h"

#include "../CPU/GlobalScheduler.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::run()
{
	while (this->isRunning)
	{
		GlobalScheduler::getInstance()->run();
	}
}