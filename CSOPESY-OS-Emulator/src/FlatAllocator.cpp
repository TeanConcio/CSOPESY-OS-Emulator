#include "FlatAllocator.h"


FlatAllocator::FlatAllocator(size_t maxMemorySize) : AMemoryAllocator(maxMemorySize, AMemoryAllocator::AllocationAlgorithm::Flat)
{
	std::fill(memory.begin(), memory.end(), '.');
	std::fill(allocationMap.begin(), allocationMap.end(), false);
}


FlatAllocator::~FlatAllocator()
{
	memory.clear();
	allocationMap.clear();
}


bool FlatAllocator::canAllocateAt(size_t index, size_t size) const
{
	// Check if the index is within the memory bounds
	if (index + size > maxMemorySize)
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


void* FlatAllocator::allocateAt(Process* processAddress, size_t index)
{
	// Mark the memory as allocated
	std::fill(allocationMap.begin() + index, allocationMap.begin() + index + processAddress->getMemoryRequired(), true);

	// Store the process address in the memory
	uintptr_t address = reinterpret_cast<uintptr_t>(processAddress);
	for (size_t i = 0; i < sizeof(uintptr_t); ++i)
	{
		memory[index + i] = (address >> (i * 8)) & 0xFF;
	}

	// Set the process address in the process
	processAddress->setMemoryAddress(&memory[index]);

	// Add the index to the list of indices with processes
	indicesWithProcesses.push_back(index);

	// Sort the list of indices with processes
	std::sort(indicesWithProcesses.begin(), indicesWithProcesses.end());

	// Return the pointer to the memory
	return &memory[index];
}


void FlatAllocator::deallocateAt(size_t index)
{
	// Retrieve the stored process address from the memory
	uintptr_t address = 0;
	for (size_t i = 0; i < sizeof(uintptr_t); ++i)
	{
		address |= (static_cast<uintptr_t>(memory[index + i]) << (i * 8));
	}
	Process* processAddress = reinterpret_cast<Process*>(address);

	// Remove the process address from the process
	processAddress->setMemoryAddress(nullptr);

	// Remove the process address from the memory
	for (size_t i = 0; i < sizeof(uintptr_t); ++i)
	{
		memory[index + i] = '.';
	}

	// Remove the index from the list of indices with processes
	indicesWithProcesses.erase(std::remove(indicesWithProcesses.begin(), indicesWithProcesses.end(), index), indicesWithProcesses.end());

	// Mark the memory as deallocated
	std::fill(allocationMap.begin() + index, allocationMap.begin() + index + processAddress->getMemoryRequired(), false);
}



void* FlatAllocator::allocate(Process* processAddress)
{
	// FIRST-FIT

	// Find the first available memory
	for (size_t i = 0; i < maxMemorySize - processAddress->getMemoryRequired() + 1; ++i)
	{
		if (canAllocateAt(i, processAddress->getMemoryRequired()))
		{
			return allocateAt(processAddress, i);
		}
	}

	return nullptr;
}


void FlatAllocator::deallocate(Process* processAddress)
{
	// Ensure the process has a valid memory address
	if (processAddress->getMemoryAddress() == nullptr)
		return;

	// Find the index of the memory
	size_t index = static_cast<char*>(processAddress->getMemoryAddress()) - &memory[0];

	// Check if the index is within bounds
	if (index >= memory.size())
		return;

	// Check if the memory is allocated
	if (!allocationMap[index])
		return;

	// Deallocate the memory
	deallocateAt(index);
}
