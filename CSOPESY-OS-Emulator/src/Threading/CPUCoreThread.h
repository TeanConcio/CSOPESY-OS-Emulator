#pragma once

#include "./IETThread.h"
#include "./../Process.h"


class CPUCoreThread : public IETThread
{
public:
	explicit CPUCoreThread(const int coreNo);
	virtual ~CPUCoreThread() = default;

	void run() override;

	std::shared_ptr<Process> getCurrentProcess() const;
	void setCurrentProcess(std::shared_ptr<Process> process);

private:
	int coreNo;
	std::shared_ptr<Process> currentProcess;
};

