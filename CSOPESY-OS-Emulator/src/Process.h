#pragma once

#include <ctime>
#include <fstream>
//#include <chrono>

#include "TypedefRepo.h"
#include "ICommand.h"
#include "ConsoleManager.h"

class Process
{
public:
	struct RequirementFlags // TODO: Change to actual flags
	{
		bool requireFiles;
		int numFiles;
		bool requireMemory;
		int memoryRequired;
	};

	// States of when CPU is executing the process
	enum ProcessState
	{
		READY,
		RUNNING,
		WAITING,
		FINISHED
	};

	// Process(String name, int totalCodeLines, int id);
	Process(const int pid, const String& name, const size_t memoryRequired, RequirementFlags requirementFlags);
	void addCommand(ICommand::CommandType commandType);
	void executeCurrentCommand();
	void logCurrentCommand();

	// Getters
	int getPID() const { return this->pid; }
	String getName() const { return this->name; }
	int getLinesOfCode() const { return static_cast<int>(this->commandList.size()); }
	int getCommandCounter() const { return this->commandCounter; }
	size_t getMemoryRequired() const { return this->memoryRequired; }
	size_t getMemoryAddressIndex() const { return this->memoryAddressIndex; }
	std::vector<size_t> getFrameIndices() const { return this->frameIndices; }
	int getCPUCoreID() const { return this->cpuCoreID; }
	ProcessState getState() const { return this == nullptr || !this->currentState ? ProcessState::WAITING : this->currentState; }
	std::time_t getArrivalTime() const { return this->arrivalTime; }
	std::time_t getLastCommandTime() const { return this->lastCommandTime; }

	// Setters
	void setMemoryAddressIndex(size_t memoryAddressIndex) { this->memoryAddressIndex = memoryAddressIndex;}
	void setFrameIndices(std::vector<size_t> frameIndices) { this->frameIndices = frameIndices; }
	void setCPUCoreID(int coreID) { this->cpuCoreID = coreID; }
	void setState(ProcessState state) { currentState = state; }
	void setArrivalTime(std::time_t arrivalTime);
	void updateLastCommandTime() { this->lastCommandTime = std::time(nullptr); }
	//std::chrono::duration<double> getFirstCommandDuration() const;

	void test_generateRandomCommands(unsigned int minInstructs, unsigned int maxInstructs); // TODO: make random in the future

private: 
	int pid;
	String name;
	RequirementFlags requirementFlags; 

	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;
	int commandCounter; // determines index of what command you are in

	size_t memoryRequired;
	size_t memoryAddressIndex = -1; // -1 means not assigned to any memory, identifies which memory block its attached to (for flat allocation)
	std::vector<size_t> frameIndices; // empty means not assigned to any memory, identifies which memory block its attached to (for paging allocation)

	int cpuCoreID = -1; // -1 means not assigned to any core, identifies which core its attached to
	ProcessState currentState;
	std::time_t arrivalTime = 0;
	std::time_t lastCommandTime;
	//std::chrono::duration<double> firstCommandDuration;

	// friend class ResourceEmulator;
};
