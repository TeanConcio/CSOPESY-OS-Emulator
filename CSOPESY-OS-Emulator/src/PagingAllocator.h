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
	size_t pageSize;
	std::vector<bool> allocationMap;
	std::map<size_t, size_t> processPageMap;
};

