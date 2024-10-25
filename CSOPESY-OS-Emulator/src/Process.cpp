#include "Process.h"
#include "ICommand.h"
#include "IOCommand.h"
#include "PrintCommand.h"

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
	this->currentState = ProcessState::RUNNING;

	// // Measure the time for the first command
	//if (this->commandCounter == 0) {
	//	auto start = std::chrono::high_resolution_clock::now();

		this->commandList[this->commandCounter]->execute();

	//	auto end = std::chrono::high_resolution_clock::now();
	//	this->firstCommandDuration = std::chrono::duration<double, std::milli>(end - start);
	//}
	//else {
	//	this->commandList[this->commandCounter]->execute();
	//}

	// this->logCurrentCommand();
	this->updateLastCommandTime();
	this->commandCounter++;

	// If all commands are executed, set state to finished and unregister screen
	if (this->commandCounter >= this->commandList.size())
	{
		this->currentState = ProcessState::FINISHED;
		ConsoleManager::getInstance()->unregisterScreen(this->name);
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


void Process::setArrivalTime(std::time_t arrivalTime)
{
	if (this == nullptr) {
		return;
	}

	// If arrival time is not set, set it
	if (this->arrivalTime == 0)
		this->arrivalTime = arrivalTime;
}


void Process::test_generateRandomCommands(unsigned int minInstructs, unsigned int maxInstructs)
{
	// Generate a number of commands between minInstructs and maxInstructs (inclusive)
	unsigned int numCommands = minInstructs + (rand() % (maxInstructs - minInstructs + 1));
	for (unsigned int i = 0; i < numCommands; i++)
	{
		this->addCommand(ICommand::CommandType::PRINT);
	}
}

//std::chrono::duration<double> Process::getFirstCommandDuration() const {
//	return firstCommandDuration;
//}



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