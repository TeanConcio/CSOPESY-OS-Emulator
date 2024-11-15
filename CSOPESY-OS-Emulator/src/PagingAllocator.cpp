#include "PagingAllocator.h"

#include "Process.h"

PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t pageSize) 
	: AMemoryAllocator(maxMemorySize, AMemoryAllocator::AllocationAlgorithm::Paging), 
	pageSize(pageSize), numFrames(maxMemorySize / pageSize)
{
	// Initialize the frame map and free frame list 
	frameMap.reserve(numFrames);
	freeFrameList.reserve(numFrames);

	// Initialize free frame list
	for (size_t i = 0; i < numFrames; i++)
	{
		freeFrameList.push_back(i);
	}
}

PagingAllocator::~PagingAllocator()
{
	frameMap.clear();
	freeFrameList.clear();
}

size_t PagingAllocator::allocate(std::shared_ptr<Process> processAddress)
{
	// Calculate the number of frames required
	size_t numFramesRequired = processAddress->getMemoryRequired() / pageSize;
	if (processAddress->getMemoryRequired() % pageSize != 0)
	{
		numFramesRequired++;
	}

	// Allocate frames
	size_t allocatedFrames = allocateFrames(numFramesRequired, processAddress->getPID());

	// Add allocated size
	allocatedSize += allocatedFrames * pageSize;

	// Return the index of the memory
	return allocatedFrames * pageSize;
}

size_t PagingAllocator::allocateFrames(size_t numFrames, size_t pid)
{
	// Check if there are enough free frames
	if (freeFrameList.size() < numFrames)
	{
		throw std::runtime_error("Not enough memory to allocate");
	}

	// Allocate frames
	size_t allocatedFrames = 0;
	for (size_t i = 0; i < numFrames; i++)
	{
		size_t frame = freeFrameList.back();
		freeFrameList.pop_back();
		frameMap[frame] = pid;
		allocatedFrames++;
	}

	return allocatedFrames;
}

void PagingAllocator::deallocate(std::shared_ptr<Process> processAddress)
{
	// Calculate the number of frames required
	size_t numFramesRequired = processAddress->getMemoryRequired() / pageSize;
	if (processAddress->getMemoryRequired() % pageSize != 0)
	{
		numFramesRequired++;
	}

	// Deallocate frames
	deallocateFrames(numFramesRequired, processAddress->getPID());

	// Subtract allocated size
	allocatedSize -= numFramesRequired * pageSize;
}

void PagingAllocator::deallocateFrames(size_t numFrames, size_t pid)
{
	// Deallocate frames
	for (size_t i = 0; i < numFrames; i++)
	{
		for (auto it = frameMap.begin(); it != frameMap.end(); it++)
		{
			if (it->second == pid)
			{
				freeFrameList.push_back(it->first);
				frameMap.erase(it);
				break;
			}
		}
	}
}