#pragma once
#include <sstream>
#include <unordered_map>

#include "../Threading/IETThread.h"
#include "../Process.h"
#include "../Common.h"


static const String DEBUG_SCHEDULER_NAME = "DebugScheduler";
static const String FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const String SJF_NOPREEMPT_SCHEDULER_NAME = "SJF-NoPreempt-Scheduler";
static const String SJF_PREEMPT_SCHEDULER_NAME = "SJF-Preempt-Scheduler";


// Abstract class for all schedulers
class AScheduler : public IETThread // have a thread for each scheduler
{
public:
	enum SchedulingAlgorithm
	{
		DEBUG,
		FCFS,
		SHORTEST_JOB_FIRST_NO_NONPREEMPTIVE,
		SHORTEST_JOB_FIRST_PREEMPTIVE,
		ROUND_ROBIN
	};

	AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, const String& processName);

	void addProcess(std::shared_ptr<Process> process); // adds a process to the scheduler
	std::shared_ptr<Process> findProcess(const String& processName);

	struct ProcessInfo
	{
		int pid;
		String name;
		int cpuID;
		int lineCounter;
		int linesOfCode;
		int remainingTime;
	};

	virtual void run() = 0;

protected:
	SchedulingAlgorithm schedulingAlgo;

	std::unordered_map<String, std::shared_ptr<Process>> processMap;
	ProcessInfo currentProcessInfo;
};