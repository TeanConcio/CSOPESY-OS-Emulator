#include "AScheduler.h"
#include "GlobalScheduler.h"
#include "TypedefRepo.h"
#include "Process.h"

class FCFSScheduler : public AScheduler
{
public:

	FCFSScheduler(int cores);

	void addProcess(const Process& process, int core = 0);
	void sortProcessQueues();
	void runScheduler();

private:
	int numCores;
	std::vector<std::vector<Process>> processQueues; // One queue per core
};