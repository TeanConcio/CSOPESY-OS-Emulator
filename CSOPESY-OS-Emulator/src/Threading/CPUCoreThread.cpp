#include "CPUCoreThread.h"


CPUCoreThread::CPUCoreThread(const int coreNo) 
{
	this->coreNo = coreNo;
	this->currentProcess = nullptr;
}


void CPUCoreThread::run()
{
	while (true) {

		/*if (this->currentProcess == nullptr) {
			std::cout << this->coreNo << " current process is null" << std::endl;
		}*/

		if (this->currentProcess != nullptr) {
			if (this->currentProcess == nullptr)
				continue;
			else if (this->currentProcess->getState() == Process::ProcessState::READY ||
					this->currentProcess->getState() == Process::ProcessState::RUNNING)
				this->currentProcess->executeCurrentCommand();
		}
	}
}


Process::ProcessState CPUCoreThread::getCurrentProcessState() const
{
	if (this->currentProcess != nullptr)
		return this->currentProcess->getState();

	return Process::ProcessState::WAITING;
}
