#pragma once

#include "IETThread.h"
#include "Process.h"

class CPUCoreThread : public IETThread
{
public:
	explicit CPUCoreThread(const int coreNo, const unsigned int quantumCycle = 0);
	virtual ~CPUCoreThread() = default;

	void run() override;

	int getCoreNo() const { return this->coreNo; }
	std::shared_ptr<Process> getCurrentProcess() const;
	void setCurrentProcess(std::shared_ptr<Process> process);
	
	bool hasQuantumCyclesLeft() const { return this->quantumCycle > this->currQuantumCycle; }
	void resetQuantumCycle() { this->currQuantumCycle = 0; }

private:
	int coreNo;

	unsigned int currCycle; // Current amount of cycles
	unsigned int delayPerExecution; // Delay per execution
	unsigned int currQuantumCycle; // Current amount of quantum cycles used
	unsigned int quantumCycle; // Quantum slice

	std::shared_ptr<Process> currentProcess;
};

