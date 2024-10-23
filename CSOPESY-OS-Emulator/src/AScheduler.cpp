#include "AScheduler.h"


AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo) : IETThread(false)
{
	this->schedulingAlgo = schedulingAlgo;
}


void AScheduler::addProcess(std::shared_ptr<Process> process)
{
	this->processMap[process->getName()] = process;
	this->queuedProcesses.push_back(process);
}


std::shared_ptr<Process> AScheduler::findProcess(const String& processName)
{
	// Find the process in the process map
	if (this->processMap.find(processName) != this->processMap.end())
	{
		return this->processMap[processName];
	}

	return nullptr;
}
