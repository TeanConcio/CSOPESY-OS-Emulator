#include "IOCommand.h"

IOCommand::IOCommand(int pid) : ICommand(pid, ICommand::CommandType::IO)
{
}

void IOCommand::execute()
{
	// Do nothing
}