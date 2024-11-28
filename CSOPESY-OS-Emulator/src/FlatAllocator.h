#pragma once

#include <algorithm>
#include "AMemoryAllocator.h"

// First-Come-First-Serve Scheduler
class FlatAllocator : public AMemoryAllocator
{
public:
	explicit FlatAllocator(size_t maxMemorySize);
	~FlatAllocator();

	bool canAllocateAt(size_t index, size_t size) const;
	size_t allocateAt(std::shared_ptr<Process> processAddress, size_t index);
	void deallocateAt(size_t index, size_t size);

	int allocate(std::shared_ptr<Process> processAddress) override;
	void deallocate(std::shared_ptr<Process> processAddress) override;

	size_t getFrameSize() const override { return maxMemorySize; }


private:
	std::vector<bool> allocationMap;
};