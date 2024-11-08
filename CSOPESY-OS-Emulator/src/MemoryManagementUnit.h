#pragma once

#include <fstream>

#include "Process.h"
#include "AMemoryAllocator.h"

// Manages throughout the OS, singleton
class MemoryManagementUnit
{
public:
	// Singleton initialization
	static MemoryManagementUnit* getInstance() { return MemoryManagementUnit::sharedInstance; }
	static void initialize();
	static void destroy();
	static void setConfigs(std::unordered_map<String, String> configs);
	static void setDefaultConfigs();

	// Memory management
	static size_t allocate(std::shared_ptr<Process> processAddress) { return sharedInstance->memoryAllocator->allocate(processAddress); }
	static void deallocate(std::shared_ptr<Process> ptr) { sharedInstance->memoryAllocator->deallocate(ptr); }

	// Print
	static String makeMemoryStampString();

	// Getters
	static size_t getMaxMemorySize() { return sharedInstance->memoryAllocator->maxMemorySize; }
	static size_t getExternalFragmentation() { return sharedInstance->memoryAllocator->getExternalFragmentation(); }
	static std::map<size_t, std::shared_ptr<Process>> getAllocatedProcessesMap() { return sharedInstance->memoryAllocator->allocatedProcesses; }

private:
	// Singleton private
	// Private constructor so that no objects can be created.
	MemoryManagementUnit() = default;
	~MemoryManagementUnit() = default;
	MemoryManagementUnit& operator=(MemoryManagementUnit const&) {}; // assignment operator is private*

	// Instance
	static MemoryManagementUnit* sharedInstance;

	// Memory Allocator and Variables
	AMemoryAllocator* memoryAllocator = nullptr;
};
