#pragma once

#include <fstream>
#include "AScheduler.h"
#include "Process.h"
#include "ProcessManager.h"



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
	unsigned int numCores;
	std::vector<std::shared_ptr<CPUCoreThread>> coreThreads;
	bool running;

	// Override IETThread
	void run() override;
};