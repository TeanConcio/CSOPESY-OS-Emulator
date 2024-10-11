#include "AScheduler.h"


AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, const String& processName) 
{

}


void AScheduler::addProcess(std::shared_ptr<Process> process)
{
	this->queuedProcesses.push_back(process);
}


std::shared_ptr<Process> AScheduler::findProcess(const String& processName)
{
	// Find the process in the queuedProcesses & finishedProcesses
	auto it = std::find_if(this->queuedProcesses.begin(), this->queuedProcesses.end(), [&processName](const std::shared_ptr<Process>& process) {
		return process->getName() == processName;
		});
	if (it == this->queuedProcesses.end())
	{
		it = std::find_if(this->finishedProcesses.begin(), this->finishedProcesses.end(), [&processName](const std::shared_ptr<Process>& process) {
			return process->getName() == processName;
			});
	}

	return nullptr;
}

void AScheduler::printQueuedProcesses()
{
	for (const auto& process : this->queuedProcesses)
	{
		std::cout << process->getName() << "\n";
	}
}