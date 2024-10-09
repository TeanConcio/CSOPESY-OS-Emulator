#include "BaseScreen.h"

#include <iostream>
#include <ctime>

#include "./../ConsoleManager.h"

// Default screen for the process
/**
 * @brief Constructs a new BaseScreen object.
 * 
 * @param process A shared pointer to the process.
 * @param processName The name of the process.
 */
BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName)
{
	this->attachedProcess = process;
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
 * If the screen is not refreshed, it prints the process info.
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

/**
 * @brief Prints the process information.
 * 
 * Prints the name, ID, current instruction line, total lines of code, and creation time of the process.
 */
void BaseScreen::printProcessInfo()
{
	/*this->writeToConsoleHistory("Process: " + this->attachedProcess->getName() + "\n");
	this->writeToConsoleHistory("ID: " + std::to_string(this->attachedProcess->getPID()) + "\n");
	this->writeToConsoleHistory("\n");
	this->writeToConsoleHistory("Current instruction line: " + std::to_string(this->attachedProcess->getCommandCounter()) + "\n");
	this->writeToConsoleHistory("Lines of code: " + std::to_string(this->attachedProcess->getLinesOfCode()) + "\n");
	this->writeToConsoleHistory("\n");*/

	/*char buffer[26];
	std::tm timeInfo;
	localtime_s(&timeInfo, &this->attachedProcess->createdTime);
	asctime_s(buffer, sizeof(buffer), &timeInfo);
	this->writeToConsoleHistory("Created: " + String(buffer) + "\n");*/
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
		//ConsoleManager::getInstance()->unregisterScreen(this->name); // TODO: Remove  or it will kill the process
	}
	else {
		this->commandNotFound(command);
	}
}


/**
 * @brief Prints the header information.
 * 
 * This function calls printProcessInfo to print the process information.
 */
void BaseScreen::printHeader()
{
	this->printProcessInfo();
}