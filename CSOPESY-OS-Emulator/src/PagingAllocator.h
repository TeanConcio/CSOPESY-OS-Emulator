#pragma once

#include "AMemoryAllocator.h"
#include <numeric>
#include <set>

class PagingAllocator : public AMemoryAllocator
{
public:
	explicit PagingAllocator(size_t maxMemorySize, size_t memPerFrame);
	~PagingAllocator();

	int allocate(std::shared_ptr<Process> processAddress) override;
	void deallocate(std::shared_ptr<Process> processAddress) override;

	size_t getFrameSize() const override { return frameSize; }

	size_t getPagesPagedIn() const override { return pagesPagedIn; }
	size_t getPagesPagedOut() const override { return pagesPagedOut; }

private:
	size_t numFrames; // Number of frames in the memory
	size_t frameSize; // Size of each frame

	std::vector<size_t> freeFrameList; // Indices of free frames in the memory

	size_t pagesPagedIn;
	size_t pagesPagedOut;
};
