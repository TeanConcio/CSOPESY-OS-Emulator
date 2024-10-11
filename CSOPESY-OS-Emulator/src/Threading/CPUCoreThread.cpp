#include "CPUCoreThread.h"


CPUCoreThread::CPUCoreThread(const int coreNo) 
{
	this->coreNo = coreNo;
	this->currentProcess = nullptr;
}


void CPUCoreThread::run()
{
	while (currentProcess->getState() == Process::ProcessState::READY)
		currentProcess->executeCurrentCommand();
}


std::shared_ptr<Process> CPUCoreThread::getCurrentProcess() const
{
	return currentProcess;
}


void CPUCoreThread::setCurrentProcess(std::shared_ptr<Process> process)
{
	currentProcess = process;
}