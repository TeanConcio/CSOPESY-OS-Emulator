#pragma once
#include "AScheduler.h"
#include "../Process.h"


// Manages throughout the OS, singleton
class GlobalScheduler : public AScheduler 
{
public:
	static GlobalScheduler* getInstance();
	static void initialize();
	static void destroy();

	void run() override;
	void stop();

	std::shared_ptr<Process> createUniqueProcess(String& name);
	String generateProcessName() const;

	void createTestProcesses(const int limit);
	

private:
	// Singleton
	// Private constructor so that no objects can be created.
	GlobalScheduler();
	~GlobalScheduler() = default;
	GlobalScheduler& operator=(GlobalScheduler const&) {}; // assignment operator is private*

	static GlobalScheduler* sharedInstance;
	AScheduler* scheduler;

	int pidCounter = 0; // How many processes its created
};