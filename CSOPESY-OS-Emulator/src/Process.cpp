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
		String toPrint = "Hello world from " + this->name + "!";
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
		this->logCurrentCommand();
		this->updateLastCommandTime();
		this->commandCounter++;
	}
}

void Process::logCurrentCommand()
{
	// Make a log file in txt
	String logFileName = this->name + "_log.txt";
	std::ofstream logFile(logFileName, std::ios::app);
	if (this->commandCounter == 0)
	{
		std::ofstream logFile(logFileName, std::ios::out);
	}

	if (logFile.is_open())
	{
		// If first command, assumed that it is the start of the process
		if (this->commandCounter == 0)
		{
			
			logFile << "Process name: " << this->name << std::endl;
			logFile << "Logs:" << std::endl << std::endl;
		}

		// Log as "(<timestamp>) Core:<coreID> "<output>"
		if (this->commandList[this->commandCounter]->getCommandType() == ICommand::IO)
		{

		}
		else
		{
			PrintCommand* printCommand = dynamic_cast<PrintCommand*>(this->commandList[this->commandCounter].get());
			
			// It creates a timestamp in the format - MM/DD/YYYY HH:MM:SSAM/PM
			char buffer[26];
			std::tm now = std::tm();
			std::time_t t = std::time(nullptr);
			localtime_s(&now, &t);
			std::strftime(buffer, sizeof(buffer), "%m/%d/%Y %H:%M:%S%p", &now);

			logFile << "(" << buffer << ") Core:" << this->cpuCoreID << " " << "\"" << printCommand->getPrintMessage() << "\"" << std::endl;
		}
		logFile.close();
	}
	else
	{
		std::cerr << "Unable to open log file " << logFileName << std::endl;
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

void Process::setCPUCoreID(int coreID)
{
	this->cpuCoreID = coreID;
}

Process::ProcessState Process::getState() const
{
	if (this == nullptr || !this->currentState)
		return ProcessState::FINISHED; // or some default state
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