#include "AScheduler.h"
#include "../Process.h"

class GlobalScheduler : AScheduler {
public:
	GlobalScheduler(AScheduler* scheduler);

	void initialize();
	void destory();
	void tick() const;
	std::shared_ptr<Process> createUniqueProcess(String name);
private:
	AScheduler* scheduler;
	static GlobalScheduler* sharedInstance;
	int pidCounter = 0;
};