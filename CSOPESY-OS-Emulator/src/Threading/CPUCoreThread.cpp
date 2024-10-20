#include "CPUCoreThread.h"


CPUCoreThread::CPUCoreThread(const int coreNo) 
{
	this->coreNo = coreNo;
	this->currentProcess = nullptr;
}


void CPUCoreThread::run()
{
	while (true) {
		if (this->currentProcess != nullptr &&
			(this->currentProcess->getState() == Process::ProcessState::READY ||
				this->currentProcess->getState() == Process::ProcessState::RUNNING))
			this->currentProcess->executeCurrentCommand();

		this->sleep(100);
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

Process::ProcessState CPUCoreThread::getCurrentProcessState() const
{
	if (this->currentProcess != nullptr)
		return this->currentProcess->getState();

	return Process::ProcessState::FINISHED;
}
