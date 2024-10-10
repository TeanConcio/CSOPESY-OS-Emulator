#include "AScheduler.h"


AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, const String& processName) 
{
	this->schedulingAlgo = schedulingAlgo;
	this->currentProcessInfo.pid = pid;
	this->currentProcessInfo.name = processName;
	this->currentProcessInfo.cpuID = 0;
	this->currentProcessInfo.lineCounter = 0;
	this->currentProcessInfo.linesOfCode = 0;
	this->currentProcessInfo.remainingTime = 0;
}


void AScheduler::addProcess(std::shared_ptr<Process> process)
{
	this->processMap[process->getName()] = process;
}


std::shared_ptr<Process> AScheduler::findProcess(const String& processName)
{
	auto it = this->processMap.find(processName);
	if (it != this->processMap.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}
