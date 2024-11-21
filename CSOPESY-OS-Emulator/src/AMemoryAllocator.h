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

	// Backing store
	virtual int checkExistsInBackingStore(std::shared_ptr<Process> processAddress, bool deleteStore) const;
	virtual void moveToBackingStore(std::shared_ptr<Process> processAddress) const;
	virtual std::shared_ptr<Process> getFromBackingStore(std::shared_ptr<Process> processAddress) const;

	// Getters
	size_t getMaxMemorySize() const { return maxMemorySize; }
	virtual size_t getFrameSize() const = 0;
	size_t getExternalFragmentation() const { return maxMemorySize - allocatedSize; }
	size_t getNumUniqueAllocatedProcesses() const;
	size_t getUsedMemorySize() const { return allocatedSize; }
	
protected:
	AllocationAlgorithm allocationAlgo;

	size_t maxMemorySize;
	size_t allocatedSize = 0;

	std::map<size_t, std::shared_ptr<Process>> allocatedProcesses;
	std::vector<std::shared_ptr<Process>> allocatedProcessesAge;
};
