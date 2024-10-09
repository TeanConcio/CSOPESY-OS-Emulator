#pragma once
#include <ctime>
#include "TypedefRepo.h"
#include "../ICommand.h"

class Process
{
public:
    struct RequirementFlags 
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

    void test_generateRandomCommands(int limit);

private: 
    //int id;
    //String name;
    //int currInstructionLine;
    //int totalCodeLines;
    //std::time_t createdTime;

    int pid;
    String name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter;
	int cpuCoreID = -1;
	RequirementFlags requirementFlags;
	ProcessState currentState;

    // friend class ResourceEmulator;
};
