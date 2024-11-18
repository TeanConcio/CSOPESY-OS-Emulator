#include "PagingAllocator.h"

#include "Process.h"

/**
* Constructor for PagingAllocator class
* @param maxMemorySize Maximum memory size
* @param memPerFrame Memory size per frame
*/
PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t memPerFrame)
	: AMemoryAllocator(maxMemorySize, AllocationAlgorithm::Paging)
{
	this->numFrames = maxMemorySize / memPerFrame;
	this->memPerFrame = memPerFrame;
	// Initialize the free frame list (each value = frame index)
	this->freeFrameList.resize(numFrames);
	std::iota(freeFrameList.begin(), freeFrameList.end(), 0);
}

/**
* Destructor for PagingAllocator class
*/
PagingAllocator::~PagingAllocator()
{
	freeFrameList.clear();
}

size_t PagingAllocator::allocate(std::shared_ptr<Process> processAddress)
{
	// Get the frame indices currently allocated to the process
	std::vector<size_t> frameIndices = processAddress->getFrameIndices();
	size_t numFramesRequired = processAddress->getMemoryRequired() / memPerFrame;

	// Check if the process is already allocated the required frames
	if (!frameIndices.empty() &&
		frameIndices.size() == numFramesRequired)
	{
		return frameIndices[0] * memPerFrame;
	}

	// Check if there are enough free frames to allocate to the process
	if (freeFrameList.size() < numFramesRequired)
	{
		return -1;
	}

	// Allocate frames to the process
	while (numFramesRequired > 0)
	{
		// Allocate the frame to the process
		size_t frameIndex = freeFrameList.back();
		frameIndices.push_back(frameIndex);
		--numFramesRequired;

		// Update the memory allocation map 
		freeFrameList.pop_back();
		allocatedProcesses[frameIndex] = processAddress;
		allocatedSize += memPerFrame;
	}

	// Update the process frame indices
	processAddress->setFrameIndices(frameIndices);

	// Return the starting memory address of the process
	return frameIndices[0] * memPerFrame;
}

/**
* Deallocate memory from the process
* @param processAddress Process address
*/
void PagingAllocator::deallocate(std::shared_ptr<Process> processAddress)
{
	// Get the frame indices currently allocated to the process
	std::vector<size_t> frameIndices = processAddress->getFrameIndices();
	size_t numFramesRequired = processAddress->getMemoryRequired() / memPerFrame;

	// Check if the process is already deallocated
	if (frameIndices.empty())
	{
		return;
	}

	// Deallocate frames from the process
	while (numFramesRequired > 0)
	{
		// Deallocate the frame from the process
		size_t frameIndex = frameIndices.back();
		frameIndices.pop_back();
		--numFramesRequired;

		// Update the memory allocation map
		freeFrameList.push_back(frameIndex);
		allocatedProcesses.erase(frameIndex);
		allocatedSize -= memPerFrame;
	}

	// Update the process frame indices
	processAddress->setFrameIndices(frameIndices);

	// Reset the memory address index of the process
	processAddress->setMemoryAddressIndex(-1);
}