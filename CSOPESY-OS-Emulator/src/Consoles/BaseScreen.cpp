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
	AConsole::onEnabled();
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
		//this->printProcessInfo();
	}

	std::cout << "root:\\>";

	String command;
	std::getline(std::cin, command);

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

void BaseScreen::display()
{
}

/**
 * @brief Prints the process information.
 * 
 * Prints the name, ID, current instruction line, total lines of code, and creation time of the process.
 */
void BaseScreen::printProcessInfo() const
{
	std::cout << "Process: " << this->attachedProcess->name << std::endl;
	std::cout << "ID: " << this->attachedProcess->id << std::endl;
	std::cout << std::endl;
	std::cout << "Current instruction line: " << this->attachedProcess->currInstructionLine << std::endl;
	std::cout << "Lines of code: " << this->attachedProcess->totalCodeLines << std::endl;
	std::cout << std::endl;

	char buffer[26];
	std::tm timeInfo;
	localtime_s(&timeInfo, &this->attachedProcess->createdTime);
	asctime_s(buffer, sizeof(buffer), &timeInfo);
	std::cout << "Created: " << buffer << std::endl;
}

/**
 * @brief Prints the header information.
 * 
 * This function calls printProcessInfo to print the process information.
 */
void BaseScreen::printHeader() const
{
	this->printProcessInfo();
}