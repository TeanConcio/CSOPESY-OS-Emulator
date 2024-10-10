#pragma once
#include "../Threading//IETThread.h"

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
	int pid;
	CommandType commandType;
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