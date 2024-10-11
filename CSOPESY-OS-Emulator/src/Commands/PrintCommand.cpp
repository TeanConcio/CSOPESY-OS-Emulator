#include "PrintCommand.h"
#include "../Threading/IETThread.h"

PrintCommand::PrintCommand(int pid, String& toPrint) 
	: ICommand(pid, PRINT)
{
	this->toPrint = toPrint;
}

// prints the message to the console
void PrintCommand::execute()
{
	std::cout << "PID " << this->pid << ": " << this->toPrint << std::endl;
	// std::stringstream msg; msg << "PID " << this->pid << ": " << this->toPrint;
	// MessageBuffer::log(msg.str());
}

String PrintCommand::getPrintMessage() const
{
	return this->toPrint;
}