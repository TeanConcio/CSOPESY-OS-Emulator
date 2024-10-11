#include "Process.h"
#include "./Commands/ICommand.h"
#include "./Commands/IOCommand.h"
#include "./Commands/PrintCommand.h"

Process::Process(int pid, String name, RequirementFlags requirementFlags)
{
	this->pid = pid;
	this->name = name;
	this->requirementFlags = requirementFlags;
	this->currentState = ProcessState::READY;
	this->commandCounter = 0;
	this->lastCommandTime = std::time(nullptr);
}

void Process::addCommand(ICommand::CommandType commandType)
{
	if (commandType == ICommand::IO)
	{
		const std::shared_ptr<ICommand> io = std::make_shared<IOCommand>(this->pid);
		this->commandList.push_back(io);
	}
	else
	{
		String toPrint = "This is a sample print";
		const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
		this->commandList.push_back(print);
	}
}

void Process::executeCurrentCommand()
{
	if (this->commandCounter >= this->commandList.size())
		this->currentState = ProcessState::FINISHED;
	else {
		this->currentState = ProcessState::RUNNING;
		this->commandList[this->commandCounter]->execute();
		this->updateLastCommandTime();
		this->commandCounter++;
	}
}





void Process::updateLastCommandTime() {
	this->lastCommandTime = std::time(nullptr);
}


int Process::getLastCommandTime() const {

	return static_cast<int>(this->lastCommandTime);
}


int Process::getCommandCounter() const
{
	return this->commandCounter;
}

int Process::getLinesOfCode() const
{
	return this->commandList.size();
}

int Process::getPID() const
{
	return this->pid;
}

int Process::getCPUCoreID() const
{
	return this->cpuCoreID;
}

Process::ProcessState Process::getState() const
{
	return this->currentState;
}

String Process::getName() const
{
	return this->name;
}

void Process::test_generateRandomCommands(int limit)
{
	for (int i = 0; i < limit; i++)
	{
		this->addCommand(ICommand::PRINT);
	}
}



//#include "Process.h"
//
//Process::Process(String name, int totalCodeLines, int id)
//{
//    this->id = id;
//    this->name = name;
//    this->currInstructionLine = 0;
//    this->totalCodeLines = totalCodeLines;
//    this->createdTime = std::time(nullptr);
//}