#pragma once

#include "IETThread.h"
#include "Process.h"

class CPUCoreThread : public IETThread
{
public:
	CPUCoreThread(const unsigned int coreNo, const unsigned int delayPerExecution, const unsigned int quantumCycle = 0);

	void run() override;

	int getCoreNo() const { return this->coreNo; }
	std::shared_ptr<Process> getCurrentProcess() const;
	void setCurrentProcess(std::shared_ptr<Process> process);
	
	bool hasQuantumCyclesLeft() const { return this->quantumCycle >= this->currQuantumCycle; }
	void resetQuantumCycle() { this->currQuantumCycle = 0; }

	// CPU Ticks
	size_t getActiveCpuTicks() const { return this->activeCpuTicks; }
	size_t getIdleCpuTicks() const { return this->idleCpuTicks; }
	size_t getTotalCpuTicks() const { return this->activeCpuTicks + this->idleCpuTicks; }

private:
	int coreNo;

	unsigned int delayPerExecution; // Delay per execution
	unsigned int currQuantumCycle; // Current amount of quantum cycles used
	unsigned int quantumCycle; // Quantum slice

	// CPU Ticks
	size_t activeCpuTicks = 0;
	size_t idleCpuTicks = 0;

	std::shared_ptr<Process> currentProcess;
};

