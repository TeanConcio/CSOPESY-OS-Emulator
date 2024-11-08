#include "MemoryManagementUnit.h"

#include "FlatAllocator.h"


MemoryManagementUnit* MemoryManagementUnit::sharedInstance = nullptr;


void MemoryManagementUnit::initialize()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new MemoryManagementUnit();
	}
}


void MemoryManagementUnit::destroy()
{
	delete sharedInstance;
	sharedInstance = nullptr;
}


/**
* @brief Set the configs from the configs.txt file
* @param configs - hash map of the configs from the configs.txt file
*/
void MemoryManagementUnit::setConfigs(std::unordered_map<String, String> configs)
{
	try {
		// Set the overall memory, memory per frame
		size_t maxOverallMem = std::stoul(configs["max-overall-mem"]);
		size_t memPerFrame = std::stoul(configs["mem-per-frame"]);

		sharedInstance->memoryAllocator = new FlatAllocator(maxOverallMem);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << ". Will set default configs." << std::endl;
		MemoryManagementUnit::setDefaultConfigs();
	}
}


/**
* @brief Set the default scheduler and configs
*/
void MemoryManagementUnit::setDefaultConfigs()
{
	// Set the overall memory, memory per frame
	sharedInstance->memoryAllocator = new FlatAllocator(16384);
}


String MemoryManagementUnit::makeMemoryStampString()
{
	std::stringstream ss;

	ss << "Timestamp: " << Common::formatTimeT(std::time(nullptr)) << "\n";
	ss << "Number of processes in memory: " << MemoryManagementUnit::getIndicesWithProcesses().size() << "\n";
	ss << "Total external fragmentation in KB: " << MemoryManagementUnit::getExternalFragmentation() << "\n";
	ss << "\n";
	ss << "----end---- = " << MemoryManagementUnit::getMaxMemorySize() << "\n";
	ss << "\n";

	// For each process
	for (int i = MemoryManagementUnit::getIndicesWithProcesses().size() - 1; i >= 0; i--) {
		// Get the process
		std::shared_ptr<Process> process = sharedInstance->memoryAllocator->getProcessAt(MemoryManagementUnit::getIndicesWithProcesses()[i]);

		ss << i + process->getMemoryRequired() << "\n";
		ss << MemoryManagementUnit::getASCIIAt(i, sizeof(void*)/sizeof(char)) << "\n";
		ss << i << "\n";
		ss << "\n";
	}
	
	ss << "----start----- = " << "0" << "\n";
	ss << "\n";

	return ss.str();
}
