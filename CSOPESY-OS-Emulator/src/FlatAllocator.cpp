#include "FlatAllocator.h"

#include "Process.h"


FlatAllocator::FlatAllocator(size_t maxMemorySize) : AMemoryAllocator(maxMemorySize, AMemoryAllocator::AllocationAlgorithm::Flat)
{
	// Initialize the memory and allocation map with the maximum memory size
	//memory.resize(maxMemorySize, '.');
	allocationMap.resize(maxMemorySize, false);
}


FlatAllocator::~FlatAllocator()
{
	allocationMap.clear();
}


bool FlatAllocator::canAllocateAt(size_t index, size_t size) const
{
	// Check if the index is within the memory bounds
	if (index + size - 1 >= maxMemorySize)
	{
		return false;
	}

	// Check if the memory is available
	for (size_t i = index; i < index + size; ++i)
	{
		if (allocationMap[i])
		{
			return false;
		}
	}

	return true;
}


size_t FlatAllocator::allocateAt(std::shared_ptr<Process> processAddress, size_t index)
{
	// Mark the memory as allocated
	std::fill(allocationMap.begin() + index, allocationMap.begin() + index + processAddress->getMemoryRequired(), true);

	// Add allocated size
	allocatedSize += processAddress->getMemoryRequired();

	// Return the index of the memory
	return index;
}


void FlatAllocator::deallocateAt(size_t index, size_t size)
{
	// Mark the memory as deallocated
	std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, false);

	// Subtract allocated size
	allocatedSize -= size;
}



size_t FlatAllocator::allocate(std::shared_ptr<Process> processAddress)
{
	// Get the index of the memory
	size_t index = processAddress->getMemoryAddressIndex();

	// Check that process has a valid memory address allocated for it
	if (index != -1 &&
		allocationMap[index] &&
		allocatedProcesses[index] == processAddress)
		return index;

	// FIRST-FIT

	// Find the first available memory
	for (size_t i = 0; i < maxMemorySize - processAddress->getMemoryRequired() + 1; ++i)
	{
		if (canAllocateAt(i, processAddress->getMemoryRequired()))
		{
			processAddress->setMemoryAddressIndex(i);
			this->allocatedProcesses[i] = processAddress;
			return allocateAt(processAddress, i);
		}
	}

	return -1;
}


void FlatAllocator::deallocate(std::shared_ptr<Process> processAddress)
{
	// Get the index of the memory
	//size_t index = static_cast<char*>(processAddress->getMemoryAddressIndex()) - &memory[0];
	size_t index = processAddress->getMemoryAddressIndex();

	// Check that process doesn't have a valid memory address allocated for it
	if (index == -1)
		return;

	if (!allocationMap[index])
		return;

	if (allocatedProcesses[index] != processAddress)
		return;

	// FIRST-FIT

	// Deallocate the memory
	deallocateAt(index, processAddress->getMemoryRequired());
	processAddress->setMemoryAddressIndex(-1);
	this->allocatedProcesses.erase(index);
}
