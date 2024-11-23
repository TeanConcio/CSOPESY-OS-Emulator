#include "PagingAllocator.h"

#include "Process.h"
#include "GlobalScheduler.h"


/**
* Constructor for PagingAllocator class
* @param maxMemorySize Maximum memory size
* @param frameSize Memory size per frame
*/
PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t memPerFrame)
	: AMemoryAllocator(maxMemorySize, AllocationAlgorithm::Paging),
	pagesPagedIn(0),
	pagesPagedOut(0),
	totalCpuTicks(0)
{
	this->numFrames = maxMemorySize / memPerFrame;
	this->frameSize = memPerFrame;
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
	// Increment the CPU ticks counter
	totalCpuTicks++;

	// Get the frame indices currently allocated to the process
	std::vector<size_t> frameIndices = processAddress->getFrameIndices();
	size_t numFramesRequired = processAddress->getMemoryRequired() / frameSize;

	// Check if the process is already allocated the required frames
	if (!frameIndices.empty() &&
		frameIndices.size() == numFramesRequired)
	{
		return frameIndices[0] * frameSize;
	}

	// Check if there are not enough free frames to allocate to the process
	if (freeFrameList.size() < numFramesRequired)
	{
		// For each oldest process
		for (size_t i = 0; i < allocatedProcessesAge.size(); ++i)
		{
			// Get the oldest process
			std::shared_ptr<Process> oldestProcess = allocatedProcessesAge[i];

			// Check if the oldest process is not being used
			if (GlobalScheduler::isCoreUsingProcess(oldestProcess) == false)
			{
				// Put oldest process into backing store
				this->moveToBackingStore(oldestProcess);
				pagesPagedOut += static_cast<int>(frameIndices.size());

				// Deallocate the oldest process
				deallocate(oldestProcess);

				// Allocate the memory
				return allocate(processAddress);
			}
		}
	}

	// Allocate frames to the process
	for (size_t i = 0; i < numFramesRequired; ++i)
	{
		// Allocate the frame to the process
		size_t frameIndex = freeFrameList.front();
		frameIndices.push_back(frameIndex);

		// Update the memory allocation map 
		freeFrameList.erase(freeFrameList.begin());
		allocatedProcesses[frameIndex] = processAddress;
		allocatedSize += frameSize;
	}

	// Update the process frame indices
	processAddress->setFrameIndices(frameIndices);
	
	// Add process to process age vector
	allocatedProcessesAge.push_back(processAddress);

	// Remove if process is in backing store
	this->getFromBackingStore(processAddress);
	pagesPagedIn += static_cast<int>(numFramesRequired);

	// Return the starting memory address of the process
	return frameIndices[0] * frameSize;
}


/**
* Deallocate memory from the process
* @param processAddress Process address
*/
void PagingAllocator::deallocate(std::shared_ptr<Process> processAddress)
{
	// Get the frame indices currently allocated to the process
	std::vector<size_t> frameIndices = processAddress->getFrameIndices();
	size_t numFramesRequired = processAddress->getMemoryRequired() / frameSize;

	// Check if the process is already deallocated
	if (frameIndices.empty())
	{
		return;
	}

	// Deallocate frames from the process
	for (size_t i = 0; i < numFramesRequired; ++i)
	{
		// Deallocate the frame from the process
		size_t frameIndex = frameIndices.front();
		frameIndices.erase(frameIndices.begin());

		// Update the memory allocation map
		freeFrameList.push_back(frameIndex);
		allocatedProcesses.erase(frameIndex);
		allocatedSize -= frameSize;
	}

	// Update the process frame indices
	processAddress->setFrameIndices(frameIndices);

	// Remove process to process age vector
	allocatedProcessesAge.erase(std::remove(allocatedProcessesAge.begin(), allocatedProcessesAge.end(), processAddress), allocatedProcessesAge.end());

	// Reset the memory address index of the process
	processAddress->setMemoryAddressIndex(-1);
}

int PagingAllocator::getPagesPagedIn() const {
	return pagesPagedIn;
}

int PagingAllocator::getPagesPagedOut() const {
	return pagesPagedOut;
}

int PagingAllocator::getTotalCpuTicks() const {
	return totalCpuTicks;
}
