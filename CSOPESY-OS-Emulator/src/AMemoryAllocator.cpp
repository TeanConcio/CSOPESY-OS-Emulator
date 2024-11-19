#include "AMemoryAllocator.h"

#include <set>


AMemoryAllocator::AMemoryAllocator(size_t maxMemorySize, AllocationAlgorithm allocationAlgo)
{
	this->maxMemorySize = maxMemorySize;
	this->allocationAlgo = allocationAlgo;
}


size_t AMemoryAllocator::getExternalFragmentation()
{
	return maxMemorySize - allocatedSize;
}


size_t AMemoryAllocator::getNumUniqueAllocatedProcesses()
{
	std::set<std::shared_ptr<Process>> uniqueProcesses;
	for (const auto& entry : allocatedProcesses)
	{
		uniqueProcesses.insert(entry.second);
	}
	return uniqueProcesses.size();
}