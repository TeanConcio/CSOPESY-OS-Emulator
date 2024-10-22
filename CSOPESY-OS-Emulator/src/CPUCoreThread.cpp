#include "CPUCoreThread.h"
#include "GlobalScheduler.h"


CPUCoreThread::CPUCoreThread(const int coreNo, const unsigned int delay, const unsigned int quantumCycle)
{
	this->coreNo = coreNo;
	this->delay = delay;

	this->currCycle = 0;
	this->quantumCycle = quantumCycle;

	this->currentProcess = nullptr;
}


void CPUCoreThread::run()
{
	while (GlobalScheduler::isRunning()) {

		if (this->currentProcess != nullptr &&
			(this->quantumCycle == 0 || this->hasQuantumCyclesLeft()) &&
			(this->currentProcess->getState() == Process::ProcessState::READY ||
			 this->currentProcess->getState() == Process::ProcessState::RUNNING)) {

			this->currentProcess->executeCurrentCommand();
			this->currCycle++;
		}

		this->sleep(this->delay);
	}
}


std::shared_ptr<Process> CPUCoreThread::getCurrentProcess() const
{
	return this->currentProcess;
}


void CPUCoreThread::setCurrentProcess(std::shared_ptr<Process> process)
{
	if (this->currentProcess != nullptr) {
		this->currentProcess->setCPUCoreID(-1);
	}
	this->currentProcess = process;
	
	if (process != nullptr) {
		this->currentProcess->setCPUCoreID(this->coreNo);
		this->currentProcess->setState(Process::ProcessState::RUNNING);
	}
}

