#pragma once

#include <fstream>
#include "AScheduler.h"
#include "Process.h"



// Manages throughout the OS, singleton
class GlobalScheduler : public IETThread
{
public:
	// Singleton initialization
	static GlobalScheduler* getInstance() { return GlobalScheduler::sharedInstance; }
	static void initialize();
	static void destroy();
	static void startGlobalScheduler();
	
	// CPU core management
	static int getCoreCount() { return sharedInstance->numCores; }
	static std::shared_ptr<CPUCoreThread> getCoreThread(int coreNum) { return sharedInstance->coreThreads[coreNum]; }
	static int getRunningCoreCount();

	static std::vector<std::shared_ptr<CPUCoreThread>> getFinishedCores();
	static std::vector<std::shared_ptr<CPUCoreThread>> getEmptyCores();
	static std::vector<std::shared_ptr<CPUCoreThread>> getRunningCores();

	// Configurations
	bool hasInitialized() { return this != nullptr && this->scheduler != nullptr; }
	static void setScheduler();
	std::unordered_map<String, String> getConfigs();
	void setConfigs(std::unordered_map<String, String> configs);
	bool areConfigsValid(std::unordered_map<String, String> configs);

	// Process creation
	std::shared_ptr<Process> createUniqueProcess(String& name);
	String generateProcessName() const;
	void generateTestProcesses();
	void createTestProcess();
	void stopGeneratingProcesses();

	// Process display
	static String makeQueuedProcessesString();
	static String makeRunningProcessesString();
	static String makeFinishedProcessesString();

	// Getters and setters
	unsigned int getBatchProcessFreq() { return this->batchProcessFreq; }
	void setBatchProcessFreq(unsigned int freq) { this->batchProcessFreq = freq; }
	unsigned int getMinIns() { return this->minIns; }
	void setMinIns(unsigned int minIns) { this->minIns = minIns; }
	unsigned int getMaxIns() { return this->maxIns; }
	void setMaxIns(unsigned int maxIns) { this->maxIns = maxIns; }



private:
	// Singleton private
	// Private constructor so that no objects can be created.
	GlobalScheduler() = default;
	~GlobalScheduler() = default;
	GlobalScheduler& operator=(GlobalScheduler const&) {}; // assignment operator is private*

	// Instance
	static GlobalScheduler* sharedInstance;

	// Scheduler and core threads
	AScheduler* scheduler;
	int numCores;
	std::vector<std::shared_ptr<CPUCoreThread>> coreThreads;
	bool running;
	unsigned int delay;

	// Override IETThread
	void run() override;

	// Configurations
	int pidCounter = 0; // How many processes its created
	unsigned int batchProcessFreq = 1; // Range: [1, 2e32], determines # of cycle between process creation
	unsigned int minIns = 1; // Range: [1, 2e32], determines min # of instructions per test process
	unsigned int maxIns = 1; // Range: [1, 2e32], determines max # of instructions per test process

	// Process generation
	int currBatchProcessCycle = 0;
	bool isGeneratingProcesses = false;
};