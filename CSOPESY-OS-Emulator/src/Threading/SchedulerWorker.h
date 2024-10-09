#pragma once
#include "IEThread.h"

class SchedulerWorker : public IEThread
{ 
public:
	SchedulerWorker() = default;
	void update(bool isRunning);
	void run() override;

private:
	bool isRunning = false;
};