#include "CPUCoreThread.h"


CPUCoreThread::CPUCoreThread(const int coreNo) 
{
	this->coreNo = coreNo;
	this->currentProcess = nullptr;
	this->start();
}


void CPUCoreThread::run()
{
	while (this->currentProcess->getState() == Process::ProcessState::READY)
		this->currentProcess->executeCurrentCommand();
}


std::shared_ptr<Process> CPUCoreThread::getCurrentProcess() const
{
	return this->currentProcess;
}


void CPUCoreThread::setCurrentProcess(std::shared_ptr<Process> process)
{
	this->currentProcess = process;
}