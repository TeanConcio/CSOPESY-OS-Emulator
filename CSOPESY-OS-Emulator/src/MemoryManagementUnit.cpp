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
	// Get values to save state
	size_t noProcessesInMemory = MemoryManagementUnit::getIndicesWithProcesses().size();
	size_t externalFragmentation = MemoryManagementUnit::getExternalFragmentation();
	size_t maxMemorySize = MemoryManagementUnit::getMaxMemorySize();

	// Get data from processes in memory
	std::vector<size_t> memoryRequiredVector;
	std::vector<String> asciiVector;
	for (size_t i = noProcessesInMemory; i-- > 0;) {
		size_t lastProcessIndex = MemoryManagementUnit::getIndicesWithProcesses().at(i);
		std::shared_ptr<Process> process = sharedInstance->memoryAllocator->getProcessAt(lastProcessIndex);
		memoryRequiredVector.push_back(i + process->getMemoryRequired() - 1);
		asciiVector.push_back(MemoryManagementUnit::getASCIIAt(i, sizeof(void*) / sizeof(char)));
	}

	
	std::stringstream ss;

	ss << "Timestamp: " << Common::formatTimeT(std::time(nullptr)) << "\n";
	ss << "Number of processes in memory: " << noProcessesInMemory << "\n";
	ss << "Total external fragmentation in KB: " << externalFragmentation << "\n";
	ss << "\n";
	ss << "----end---- = " << maxMemorySize << "\n";
	ss << "\n";

	// For each process
	for (int i = noProcessesInMemory - 1; i >= 0; i--) {
		ss << memoryRequiredVector[i] << "\n";
		ss << asciiVector[i] << "\n";
		ss << i << "\n";
		ss << "\n";
	}

	ss << "----start----- = " << "0" << "\n";
	ss << "\n";

	return ss.str();
}
