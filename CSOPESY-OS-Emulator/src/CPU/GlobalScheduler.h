#pragma once
#include "AScheduler.h"
#include "../Process.h"

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

private:
	AScheduler* scheduler;
	static GlobalScheduler* sharedInstance;
	int pidCounter;
};