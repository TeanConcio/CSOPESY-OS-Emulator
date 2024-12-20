#pragma once

#include <unordered_map>

#include "Process.h"
#include "Common.h"
#include "GlobalScheduler.h"
#include "CPUCoreThread.h"
#include "ConsoleManager.h"
#include "BaseScreen.h"



class ProcessManager
{
	friend class GlobalScheduler;



public:
	// Singleton initialization
	static ProcessManager* getInstance() { return ProcessManager::sharedInstance; }
	static void initialize();
	static void destroy();
	static void setConfigs(std::unordered_map<String, String> configs);
	static void setDefaultConfigs();

	// Process management
	void addProcess(std::shared_ptr<Process> process) const;
	static std::shared_ptr<Process> findProcess(const String& processName);

	// Process creation
	String generateProcessName() const;
	size_t generateMemorySize() const;
	static std::shared_ptr<Process> createUniqueProcess(String& name);
	static void generateTestProcessesLoop();
	static void startGeneratingProcessesLoop() { sharedInstance->isGeneratingProcesses = true; }
	static void stopGeneratingProcessesLoop() { sharedInstance->isGeneratingProcesses = false; }

	// Process display
	static String makeRunningProcessesString();
	static String makeQueuedProcessesString();
	static String makeFinishedProcessesString();
	static String makeListProcessesString();
	//static String getMemoryUsagePerProcess();
	//size_t getTotalMemoryUsed() const { return totalMemoryUsed; }

	// Getters and setters
	static std::vector<std::shared_ptr<Process>>& getQueuedProcesses() { return sharedInstance->queuedProcesses; }
	static std::vector<std::shared_ptr<Process>>& getFinishedProcesses() { return sharedInstance->finishedProcesses; }



private:
	// Singleton private
	// Private constructor so that no objects can be created.
	ProcessManager() = default;
	~ProcessManager() = default;
	ProcessManager& operator=(ProcessManager const&) {}; // assignment operator is private*

	// Instance
	static ProcessManager* sharedInstance;

	// Process storage
	std::unordered_map<String, std::shared_ptr<Process>> processMap;
	std::vector<std::shared_ptr<Process>> queuedProcesses;
	std::vector<std::shared_ptr<Process>> finishedProcesses;

	// Configurations
	unsigned int batchProcessFreq = 1; // Range: [1, 2e32], determines # of cycle between process creation
	unsigned int minIns = 1; // Range: [1, 2e32], determines min # of instructions per test process
	unsigned int maxIns = 1; // Range: [1, 2e32], determines max # of instructions per test process
	size_t minMemPerProcExp = 12; // Range: [0, 32], determines min memory per process (exponent)
	size_t maxMemPerProcExp = 32; // Range: [0, 32], determines max memory per process (exponent)

	// Process generation
	int pidCounter = 0; // How many processes its created
	int currBatchProcessCycle = 0;
	bool isGeneratingProcesses = false;
	//size_t totalMemoryUsed = 0;
};
