#pragma once
#include "../Threading//IETThread.h"

// interface blueprint for all commands
class ICommand
{
public:
	enum CommandType
	{
		IO,
		PRINT
	};

	ICommand(int pid, CommandType commandType);
	CommandType getCommandType();
	virtual void execute();

protected:
	int pid; // process id to which the command belongs
	CommandType commandType; // type of command
};

inline ICommand::CommandType ICommand::getCommandType()
{
	return this->commandType;
}

inline ICommand::ICommand(int pid, CommandType commandType)
{
	this->pid = pid;
	this->commandType = commandType;
}

inline void ICommand::execute()
{
	// Do nothing
}