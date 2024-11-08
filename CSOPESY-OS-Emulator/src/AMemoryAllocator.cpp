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
