#include "MemoryManagementUnit.h"

#include "FlatAllocator.h"
#include "PagingAllocator.h"


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

		// If max-overall-mem = mem-per-frame, use FlatAllocator
		if (maxOverallMem == memPerFrame) {
			sharedInstance->memoryAllocator = new FlatAllocator(maxOverallMem);
		}
		// Else, use PagingAllocator
		else
		{
			sharedInstance->memoryAllocator = new PagingAllocator(maxOverallMem, memPerFrame);
		}
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
	size_t numProcessesInMemory = MemoryManagementUnit::getNumUniqueAllocatedProcesses();
	size_t externalFragmentation = MemoryManagementUnit::getExternalFragmentation();
	size_t maxMemorySize = MemoryManagementUnit::getMaxMemorySize();

	// Get data from processes in memory
	std::vector<size_t> indicesVector;
	std::vector<size_t> memoryRequiredVector;
	std::vector<String> asciiVector;
	std::vector<std::vector<size_t>> framesVector;

	// Loop through the processes in memory in allocatedProcesses
	for (const auto& pair : MemoryManagementUnit::getAllocatedProcessesMap()) {
		size_t lastProcessIndex = pair.first;
		std::shared_ptr<Process> process = pair.second;

		indicesVector.push_back(lastProcessIndex);
		memoryRequiredVector.push_back(lastProcessIndex + process->getMemoryRequired());
		asciiVector.push_back(process->getName());
		framesVector.push_back(process->getFrameIndices());
	}

	
	std::stringstream ss;

	ss << "Timestamp: " << Common::formatTimeT(std::time(nullptr)) << "\n";
	ss << "Number of processes in memory: " << numProcessesInMemory << "\n";
	ss << "Total external fragmentation in KB: " << externalFragmentation << "\n";
	ss << "\n";
	ss << "----end---- = " << maxMemorySize << "\n";
	ss << "\n";

	// For each process in reverse order
	for (int i = static_cast<int>(numProcessesInMemory) - 1; i >= 0; i--) {
		ss << memoryRequiredVector[i] << "\n";
		ss << asciiVector[i] << "\n";
		ss << indicesVector[i] << "\n";
		for (size_t frame : framesVector[i]) {
			ss << frame << " ";
		}
		ss << "\n";
		ss << "\n";
	}

	ss << "----start----- = " << "0" << "\n";
	ss << "\n";

	return ss.str();
}
