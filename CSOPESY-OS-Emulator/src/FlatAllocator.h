#pragma once

#include <algorithm>

#include "AMemoryAllocator.h"

class AMemoryAllocator;



// First-Come-First-Serve Scheduler
class FlatAllocator : public AMemoryAllocator
{
public:
	explicit FlatAllocator(size_t maxMemorySize);
	~FlatAllocator();

	bool canAllocateAt(size_t index, size_t size) const;
	void* allocateAt(std::shared_ptr<Process> processAddress, size_t index);
	void deallocateAt(size_t index);

	void* allocate(std::shared_ptr<Process> processAddress) override;
	void deallocate(std::shared_ptr<Process> processAddress) override;


private:
	std::vector<bool> allocationMap;
};