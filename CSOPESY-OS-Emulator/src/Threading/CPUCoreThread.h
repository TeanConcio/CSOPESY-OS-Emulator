#pragma once

#include "./IETThread.h"
#include "./../Process.h"


class CPUCoreThread : public IETThread
{
public:
	explicit CPUCoreThread(const int coreNo);
	virtual ~CPUCoreThread() = default;

	void run() override;

	std::shared_ptr<Process> getCurrentProcess() const { return this->currentProcess; }
	void setCurrentProcess(std::shared_ptr<Process> process) { this->currentProcess = process; }
	Process::ProcessState getCurrentProcessState() const;
	int getCoreNo() const { return this->coreNo; }

private:
	int coreNo;
	std::shared_ptr<Process> currentProcess;
};

