#include "AMemoryAllocator.h"


AMemoryAllocator::AMemoryAllocator(size_t maxMemorySize, AllocationAlgorithm allocationAlgo)
{
	this->maxMemorySize = maxMemorySize;
	this->allocationAlgo = allocationAlgo;
}


size_t AMemoryAllocator::getExternalFragmentation()
{
	size_t externalFragmentation = 0;
	size_t lastAllocatedIndex = 0;
	for (size_t i = 0; i < indicesWithProcesses.size(); ++i)
	{
		externalFragmentation += indicesWithProcesses[i] - lastAllocatedIndex;
		lastAllocatedIndex = indicesWithProcesses[i] + 1;
	}
	externalFragmentation += maxMemorySize - lastAllocatedIndex;
	return externalFragmentation;
}


std::shared_ptr<Process> AMemoryAllocator::getProcessAt(size_t index) const
{
	// Retrieve the stored process address from the memory
	uintptr_t address = 0;
	for (size_t i = 0; i < sizeof(uintptr_t); ++i)
	{
		address |= (static_cast<uintptr_t>(memory[index + i]) << (i * 8));
	}
	Process* processAddress = reinterpret_cast<Process*>(address);

	return std::shared_ptr<Process>(processAddress);
}


String AMemoryAllocator::getASCIIAt(size_t index, size_t size) const
{
	std::stringstream ss;
	for (size_t i = 0; i < size; ++i)
	{
		ss << memory[index + i];
	}
	return ss.str();
}
