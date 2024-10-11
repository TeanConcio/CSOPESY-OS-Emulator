#include "PrintCommand.h"
#include "../Threading/IETThread.h"

PrintCommand::PrintCommand(const int pid, const String& toPrint) 
	: ICommand(pid, PRINT)
{
	this->toPrint = toPrint;
}

// prints the message to the console
void PrintCommand::execute()
{
	// Put delay
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));

	//std::cout << "PID " << this->pid << ": " << this->toPrint << std::endl;
	// std::stringstream msg; msg << "PID " << this->pid << ": " << this->toPrint;
	// MessageBuffer::log(msg.str());
}