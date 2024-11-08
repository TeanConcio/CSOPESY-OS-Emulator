#pragma once

#include <sstream>
#include <map>

#include "Process.h"

// Abstract class for all schedulers
class AMemoryAllocator
{
	friend class MemoryManagementUnit;

public:
	enum AllocationAlgorithm
	{
		Flat,
		Paging
	};

	AMemoryAllocator(size_t maxMemorySize, AllocationAlgorithm allocationAlgo);
	virtual ~AMemoryAllocator() = default;

	virtual size_t allocate(std::shared_ptr<Process> processAddress) = 0;
	virtual void deallocate(std::shared_ptr<Process> processAddress) = 0;

	// Getters
	size_t getMaxMemorySize() { return maxMemorySize; }
	size_t getExternalFragmentation();
	String getASCIIAt(size_t index, size_t size) const;

	
protected:
	AllocationAlgorithm allocationAlgo;

	size_t maxMemorySize;
	size_t allocatedSize = 0;
	//std::vector<char> memory;

	std::map<size_t, std::shared_ptr<Process>> allocatedProcesses;
};