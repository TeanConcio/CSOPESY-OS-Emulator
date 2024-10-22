#pragma once

#include "IETThread.h"
#include "Process.h"

class CPUCoreThread : public IETThread
{
public:
	explicit CPUCoreThread(const int coreNo, const unsigned int delay);
	virtual ~CPUCoreThread() = default;

	void run() override;

	std::shared_ptr<Process> getCurrentProcess() const;
	void setCurrentProcess(std::shared_ptr<Process> process);
	Process::ProcessState getCurrentProcessState() const;
	int getCoreNo() const { return this->coreNo; }

private:
	int coreNo;
	unsigned int delay;
	std::shared_ptr<Process> currentProcess;
};

