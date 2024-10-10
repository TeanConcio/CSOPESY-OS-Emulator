#pragma once
#include "AScheduler.h"
#include "../Process.h"

// Manages throughout the OS, singleton
class GlobalScheduler : public AScheduler {
public:
	GlobalScheduler();
	void init() override;
	void execute() override;
	void destroy();
	void tick() const;
	std::shared_ptr<Process> createUniqueProcess(String name);
	std::shared_ptr<Process> findProcess(String name) const;
	String generateProcessName() const;
	static GlobalScheduler* getInstance();
	void createTestProcesses();

private:
	AScheduler* scheduler;
	static GlobalScheduler* sharedInstance;
	int pidCounter; // How many processes its created
};