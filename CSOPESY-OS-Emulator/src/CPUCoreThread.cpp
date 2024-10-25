#include "CPUCoreThread.h"
#include "GlobalScheduler.h"


CPUCoreThread::CPUCoreThread(const unsigned int coreNo, const unsigned int delayPerExecution, const unsigned int quantumCycle) : IETThread(false)
{
	this->coreNo = coreNo;

	this->currCycle = 0;
	this->delayPerExecution = delayPerExecution;

	this->currQuantumCycle = 0;
	this->quantumCycle = quantumCycle;

	this->currentProcess = nullptr;
}


void CPUCoreThread::run()
{
	if (this->delayPerExecution == 0 || this->currCycle % this->delayPerExecution == 0) {
	// if (this->currCycle % this->delayPerExecution == 0) {
		if (this->currentProcess != nullptr &&
			(this->quantumCycle == 0 || this->hasQuantumCyclesLeft()) &&
			(this->currentProcess->getState() == Process::ProcessState::READY ||
				this->currentProcess->getState() == Process::ProcessState::RUNNING)) {

			this->currentProcess->executeCurrentCommand();
			this->currQuantumCycle++;
		}
	}

	this->currCycle++;
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

