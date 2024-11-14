#include "PagingAllocator.h"

#include "Process.h"

PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t pageSize) : AMemoryAllocator(maxMemorySize, AMemoryAllocator::AllocationAlgorithm::Paging), pageSize(pageSize)
{
	allocationMap.resize(maxMemorySize / pageSize, false);
}

PagingAllocator::~PagingAllocator()
{
	allocationMap.clear();
}