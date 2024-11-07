#pragma once

#include <algorithm>

#include "AMemoryAllocator.h"
#include "TypedefRepo.h"

class AMemoryAllocator;



// First-Come-First-Serve Scheduler
class FlatAllocator : public AMemoryAllocator
{
public:
	explicit FlatAllocator(size_t maxMemorySize);
	~FlatAllocator();

	bool canAllocateAt(size_t index, size_t size) const;
	void* allocateAt(Process* processAddress, size_t index);
	void deallocateAt(size_t index);

	void* allocate(Process* processAddress) override;
	void deallocate(Process* processAddress) override;


private:
	std::vector<bool> allocationMap;
};