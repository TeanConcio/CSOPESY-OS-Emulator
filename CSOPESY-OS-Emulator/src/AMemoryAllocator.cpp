#include "AMemoryAllocator.h"


AMemoryAllocator::AMemoryAllocator(size_t maxMemorySize, AllocationAlgorithm allocationAlgo)
{
	this->maxMemorySize = maxMemorySize;
	this->allocationAlgo = allocationAlgo;
}


size_t AMemoryAllocator::getExternalFragmentation()
{
	return maxMemorySize - allocatedSize;
}


std::shared_ptr<Process> AMemoryAllocator::getProcessAt(size_t index) const
{
	// Retrieve the stored process address from the memory
	uintptr_t address = 0;
	for (size_t i = 0; i < sizeof(uintptr_t); ++i)
	{
		address |= (static_cast<uintptr_t>(static_cast<unsigned char>(memory[index + i])) << (i * 8));
	}
	//Process* processAddress = reinterpret_cast<Process*>(address);

	// Debug print
	//std::cout << "Retrieved Process at index " << index << " with address " << processAddress << std::endl;

	return std::shared_ptr<Process>(reinterpret_cast<Process*>(address));
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
