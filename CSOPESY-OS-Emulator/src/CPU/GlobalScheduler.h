#pragma once

#include <fstream>
#include "AScheduler.h"
#include "../Process.h"


// Manages throughout the OS, singleton
class GlobalScheduler 
{
public:
	static GlobalScheduler* getInstance();
	static void initialize();
	static void destroy();

	static void start() { GlobalScheduler::getInstance()->scheduler->start(); }
	
	std::shared_ptr<Process> createUniqueProcess(String& name);
	String generateProcessName() const;
	void createTestProcesses(const int limit);

	static String makeQueuedProcessesString() { return GlobalScheduler::getInstance()->scheduler->makeQueuedProcessesString(); }
	static String makeRunningProcessesString() { return GlobalScheduler::getInstance()->scheduler->makeRunningProcessesString(); }
	static String makeFinishedProcessesString() { return GlobalScheduler::getInstance()->scheduler->makeFinishedProcessesString(); }

	bool hasInitialized() { return this != nullptr || this->scheduler != nullptr; }
	static void setScheduler();
	std::unordered_map<String, String> getConfigs();
	void setConfigs(std::unordered_map<String, String> configs);

	void setBatchProcessFreq(unsigned int freq) { this->batchProcessFreq = freq; }
	void setMinIns(unsigned int minIns) { this->minIns = minIns; }
	void setMaxIns(unsigned int maxIns) { this->maxIns = maxIns; }

	unsigned int getBatchProcessFreq() { return this->batchProcessFreq; }
	unsigned int getMinIns() { return this->minIns; }
	unsigned int getMaxIns() { return this->maxIns; }
	unsigned int getDelay() { return this->delay; }


private:
	// Singleton
	// Private constructor so that no objects can be created.
	GlobalScheduler() = default;
	~GlobalScheduler() = default;
	GlobalScheduler& operator=(GlobalScheduler const&) {}; // assignment operator is private*

	static GlobalScheduler* sharedInstance;
	AScheduler* scheduler;

	int pidCounter = 0; // How many processes its created

	unsigned int batchProcessFreq = 1; // Range: [1, 2e32], determines # of test processes to make per cycle
	unsigned int minIns = 1; // Range: [1, 2e32], determines min # of instructions per test process
	unsigned int maxIns = 1; // Range: [1, 2e32], determines max # of instructions per test process
	unsigned int delay = 0; // Range: [0, 2e32], determines delay per command executed in ms
};