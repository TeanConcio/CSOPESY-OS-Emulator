#pragma once

#include <ctime>
#include "TypedefRepo.h"
#include "Commands/ICommand.h"

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
	void executeCurrentCommand() const;
	void moveToNextLine();

	bool isFinished() const;
	int getRemainingTime() const;
	int getCommandCounter() const;
	int getLinesOfCode() const;
	int getPID() const;
	int getCPUCoreID() const;
	ProcessState getState() const;
	String getName() const;

	void test_generateRandomCommands(int limit); // TEMP: make random in the future

private: 
	int pid;
	String name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;

	int commandCounter; // determines index of what command you are in
	int cpuCoreID = -1; // -1 means not assigned to any core, identifies which core its attached to
	RequirementFlags requirementFlags; 
	ProcessState currentState;

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