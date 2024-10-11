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
	}
}


std::shared_ptr<Process> CPUCoreThread::getCurrentProcess() const
{
	return this->currentProcess;
}


void CPUCoreThread::setCurrentProcess(std::shared_ptr<Process> process)
{
	this->currentProcess = process;
}

Process::ProcessState CPUCoreThread::getCurrentProcessState() const
{
	if (this->currentProcess != nullptr)
		return this->currentProcess->getState();

	return Process::ProcessState::FINISHED;
}
