#pragma once

#include <ctime>
#include <fstream>
#include "TypedefRepo.h"
#include "ICommand.h"

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
	Process(int pid, String name, RequirementFlags requirementFlags);
	void addCommand(ICommand::CommandType commandType);
	void executeCurrentCommand();
	void logCurrentCommand();

	// Getters and Setters
	String getName() const { return this->name; }
	int getArrivalTime() const { return this->arrivalTime; }
	int getLastCommandTime() const { return this->lastCommandTime; }
	int getCommandCounter() const { return this->commandCounter; }
	int getLinesOfCode() const { return this->commandList.size(); }
	int getPID() const { return this->pid; }
	int getCPUCoreID() const { return this->cpuCoreID; }
	ProcessState getState() const { return this == nullptr || !this->currentState ? ProcessState::FINISHED : this->currentState; }
	void updateLastCommandTime() { this->lastCommandTime = std::time(nullptr); }
	void setCPUCoreID(int coreID) { this->cpuCoreID = coreID; }
	void setState(ProcessState state) { currentState = state; }
	void setArrivalTime(std::time_t arrivalTime);

	void test_generateRandomCommands(unsigned int minInstructs, unsigned int maxInstructs); // TODO: make random in the future

private: 
	int pid;
	String name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;

	int commandCounter; // determines index of what command you are in
	int cpuCoreID = -1; // -1 means not assigned to any core, identifies which core its attached to
	RequirementFlags requirementFlags; 
	ProcessState currentState;
	std::time_t arrivalTime = 0;
	std::time_t lastCommandTime;

	// friend class ResourceEmulator;
};



//#pragma once
//
//#include <ctime>
//#include "TypedefRepo.h"
//
//class Process
//{
//public:
//    Process(String name, int totalCodeLines, int id);
//
//    int id;
//    String name;
//    int currInstructionLine;
//    int totalCodeLines;
//    std::time_t createdTime;
//};