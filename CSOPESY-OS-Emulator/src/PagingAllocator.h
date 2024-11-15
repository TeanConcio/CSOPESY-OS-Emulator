#pragma once

#include "AMemoryAllocator.h"

class PagingAllocator : public AMemoryAllocator
{
public:
	explicit PagingAllocator(size_t maxMemorySize, size_t pageSize);
	~PagingAllocator();

	size_t allocate(std::shared_ptr<Process> processAddress) override;
	void deallocate(std::shared_ptr<Process> processAddress) override;

private:
	size_t numFrames;
	size_t pageSize;
	std::unordered_map<size_t, size_t> frameMap;
	std::vector<size_t> freeFrameList;

	size_t allocateFrames(size_t numFrames, size_t pid);
	void deallocateFrames(size_t numFrames, size_t pid);
};

