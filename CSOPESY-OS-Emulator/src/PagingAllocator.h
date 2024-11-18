#pragma once

#include "AMemoryAllocator.h"
#include <numeric>

class PagingAllocator : public AMemoryAllocator
{
public:
	explicit PagingAllocator(size_t maxMemorySize, size_t memPerFrame);
	~PagingAllocator();

	size_t allocate(std::shared_ptr<Process> processAddress) override;
	void deallocate(std::shared_ptr<Process> processAddress) override;

private:
	size_t numFrames; // Number of frames in the memory
	size_t memPerFrame; // Size of each frame
	std::vector<size_t> freeFrameList; // Indices of free frames in the memory
};

