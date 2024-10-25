#include "BaseScreen.h"

#include <iostream>
#include <ctime>

#include "ConsoleManager.h"



// Default screen for the attached Process
/**
 * @brief Constructs a new BaseScreen object.
 * 
 * @param currentProcess A shared pointer to the attached Process.
 */
BaseScreen::BaseScreen(std::shared_ptr<Process> process) : AConsole(process->getName())
{
	this->attachedProcess = process;
	this->refreshed = false;
}

/**
 * @brief Called when the screen is enabled.
 * 
 * Sets the refreshed flag to false.
 */
void BaseScreen::onEnabled()
{
	this->refreshed = false;
}

/**
 * @brief Processes the input commands.
 * 
 * If the screen is not refreshed, it prints the currentProcess info.
 * Then it waits for user input and processes the command.
 */
void BaseScreen::process()
{
	if (this->refreshed == false)
	{
		this->refreshed = true;
		
		if (this->history.empty())
		{
			this->printHeader();
		}
		else
		{
			this->printHistory();
		}
	}

	this->writeToConsoleHistory("root:\\>");

	String command;
	command = this->getConsoleInputToHistory();

	this->decideCommand(command);
}


void BaseScreen::display()
{
}


void BaseScreen::decideCommand(const String& command)
{
	std::vector<String> commandParts = Common::commandExtractor(command);

	// Inherited commands
	if (commandParts[0] == "clear" || command == "cls")
	{
		this->clear();
	}
	else if (commandParts[0] == "process-smi")
	{
		this->printProcessInfo();
	}
	else if (commandParts[0] == "exit")
	{
		this->exit();
		//ConsoleManager::getInstance()->unregisterScreen(this->name); // TODO: Remove  or it will kill the currentProcess
	}
	else if (commandParts[0] == "help")
	{
		this->writeToConsoleHistory("\tprocess-smi : Simple information of the process\n");
		this->writeToConsoleHistory("\texit : Returns the user to the main menu.\n");
	}
	else {
		this->commandNotFound(command);
	}
}


/**
 * @brief Prints the header information.
 * 
 * This function calls printProcessInfo to print the currentProcess information.
 */
void BaseScreen::printHeader()
{
	this->printProcessInfo();
}


/**
 * @brief Prints the currentProcess information.
 *
 * Prints the name, ID, current instruction line, total lines of code, and creation time of the currentProcess.
 */
void BaseScreen::printProcessInfo()
{
	this->writeToConsoleHistory("Process: " + this->attachedProcess->getName() + "\n");
	this->writeToConsoleHistory("ID: " + std::to_string(this->attachedProcess->getPID()) + "\n");
	this->writeToConsoleHistory("\n");

	// If the process is finished, print "Finished!"
	if (this->attachedProcess->getState() == Process::ProcessState::FINISHED)
	{
		this->writeToConsoleHistory("Finished!\n");
	}
	// Else, print the current instruction line and total lines of code
	else
	{
		this->writeToConsoleHistory("Current instruction line: " + std::to_string(this->attachedProcess->getCommandCounter()) + "\n");
		this->writeToConsoleHistory("Lines of code: " + std::to_string(this->attachedProcess->getLinesOfCode()) + "\n");
	}

	this->writeToConsoleHistory("\n");
}
