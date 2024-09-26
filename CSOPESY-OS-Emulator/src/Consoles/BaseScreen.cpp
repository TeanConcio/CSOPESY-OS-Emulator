#include "BaseScreen.h"

#include <iostream>
#include <ctime>

#include "./../ConsoleManager.h"

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName)
{
	this->attachedProcess = process;
}

void BaseScreen::onEnabled()
{
	this->refreshed = false;
}

void BaseScreen::process()
{
	if (this->refreshed == false)
	{
		this->refreshed = true;
		this->printProcessInfo();
	}

	std::cout << "root:\\>";

	String command;
	std::getline(std::cin, command);

	std::vector<String> commandParts = Common::commandExtractor(command);

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
		ConsoleManager::getInstance()->unregisterScreen(this->name);
	}
	else {
		this->commandNotFound(command);
	}
}

void BaseScreen::display()
{
}

void BaseScreen::printProcessInfo() const
{
	std::cout << "Process: " << this->attachedProcess->name << std::endl;
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

void BaseScreen::printHeader() const
{
	this->printProcessInfo();
}