#pragma once

#include "IETThread.h"
#include "Process.h"

class CPUCoreThread : public IETThread
{
public:
	explicit CPUCoreThread(const int coreNo, const unsigned int delay, const unsigned int quantumCycle = 0);
	virtual ~CPUCoreThread() = default;

	void run() override;

	int getCoreNo() const { return this->coreNo; }
	std::shared_ptr<Process> getCurrentProcess() const;
	void setCurrentProcess(std::shared_ptr<Process> process);
	
	bool hasQuantumCyclesLeft() const { return this->quantumCycle - this->currCycle > 0; }
	void resetQuantumCycle() { this->currCycle = 0; }

private:
	int coreNo;
	unsigned int delay;

	int unsigned currCycle;
	int unsigned quantumCycle;

	std::shared_ptr<Process> currentProcess;
};

