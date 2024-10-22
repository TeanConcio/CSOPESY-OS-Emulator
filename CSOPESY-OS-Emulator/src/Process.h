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

	void updateLastCommandTime();
	int getArrivalTime() const;
	int getLastCommandTime() const;
	int getCommandCounter() const;
	int getLinesOfCode() const;
	int getPID() const;
	int getCPUCoreID() const;
	void setCPUCoreID(int coreID);
	ProcessState getState() const;
	void setState(ProcessState state) { currentState = state; }
	String getName() const;
	void setArrivalTime(std::time_t arrivalTime);

	void test_generateRandomCommands(unsigned int minInstructs, unsigned int maxInstructs); // TEMP: make random in the future

private: 
	int pid;
	String name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;

	int commandCounter; // determines index of what command you are in
	int cpuCoreID = -1; // -1 means not assigned to any core, identifies which core its attached to
	RequirementFlags requirementFlags; 
	ProcessState currentState;
	std::time_t arrivalTime;
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