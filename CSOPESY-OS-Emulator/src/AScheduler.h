#pragma once
#include <sstream>
#include <unordered_map>

#include "IETThread.h"
#include "CPUCoreThread.h"
#include "Process.h"
#include "Common.h"


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

	virtual void addProcess(std::shared_ptr<Process> process); // adds a currentProcess to the scheduler
	std::shared_ptr<Process> findProcess(const String& processName);

	virtual void run() = 0;

protected:
	SchedulingAlgorithm schedulingAlgo;

	std::unordered_map<String, std::shared_ptr<Process>> processMap;
	std::vector<std::shared_ptr<Process>> queuedProcesses;
	std::vector<std::shared_ptr<Process>> finishedProcesses;
};