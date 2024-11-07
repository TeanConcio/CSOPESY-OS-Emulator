#pragma once

#include <fstream>

#include "Process.h"
#include "ProcessManager.h"
#include "AMemoryAllocator.h"
#include "FlatAllocator.h"

class AMemoryAllocator;



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
	static void* allocate(Process* processAddress) { return sharedInstance->memoryAllocator->allocate(processAddress); }
	static void deallocate(Process* ptr) { sharedInstance->memoryAllocator->deallocate(ptr); }

	// Print
	static String makeMemoryStampString();

	// Getters
	static size_t getMaxMemorySize() { return sharedInstance->memoryAllocator->maxMemorySize; }
	static size_t getExternalFragmentation() { return sharedInstance->memoryAllocator->getExternalFragmentation(); }
	static std::vector<size_t> getIndicesWithProcesses() { return sharedInstance->memoryAllocator->indicesWithProcesses; }
	static std::shared_ptr<Process> getProcessAt(size_t index) { return sharedInstance->memoryAllocator->getProcessAt(index); }
	static String getASCIIAt(size_t index, size_t size) { return sharedInstance->memoryAllocator->getASCIIAt(index, size); }

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
