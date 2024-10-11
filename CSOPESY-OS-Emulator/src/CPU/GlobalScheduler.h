#pragma once

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

private:
	// Singleton
	// Private constructor so that no objects can be created.
	GlobalScheduler() = default;
	~GlobalScheduler() = default;
	GlobalScheduler& operator=(GlobalScheduler const&) {}; // assignment operator is private*

	static GlobalScheduler* sharedInstance;
	AScheduler* scheduler;

	int pidCounter = 0; // How many processes its created
};