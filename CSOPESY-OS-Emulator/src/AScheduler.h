#pragma once

#include <sstream>
#include <unordered_map>

#include "TypedefRepo.h"
#include "IETThread.h"

class IEThread;
class CPUCoreThread;
class GlobalScheduler;
class MemoryManagementUnit;



static const String DEBUG_SCHEDULER_NAME = "DebugScheduler";
static const String FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const String SJF_NOPREEMPT_SCHEDULER_NAME = "SJF-NoPreempt-Scheduler";
static const String SJF_PREEMPT_SCHEDULER_NAME = "SJF-Preempt-Scheduler";



// Abstract class for all schedulers
class AScheduler : public IETThread // have a thread for each scheduler
{
	friend class GlobalScheduler;

public:
	enum SchedulingAlgorithm
	{
		DEBUG,
		FCFS,
		SHORTEST_JOB_FIRST_NO_NONPREEMPTIVE,
		SHORTEST_JOB_FIRST_PREEMPTIVE,
		ROUND_ROBIN
	};

	AScheduler(SchedulingAlgorithm schedulingAlgo);
	virtual ~AScheduler() = default;

	virtual void run() = 0;

	// Scheduler operations
	bool assignNextQueuedProcess(std::shared_ptr<CPUCoreThread> core) const;
	void moveProcessToQueue(std::shared_ptr<CPUCoreThread> core) const;
	void moveProcessToFinish(std::shared_ptr<CPUCoreThread> core) const;


protected:
	SchedulingAlgorithm schedulingAlgo;
};