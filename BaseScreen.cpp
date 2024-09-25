#include "BaseScreen.h"

#include "ConsoleManager.h"

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName)
{
	this->attachedProcess = process;
}

void BaseScreen::onEnabled()
{
}

void BaseScreen::process()
{
	if (this->refreshed == false)
	{
		this->refreshed = true;
		this->printProcessInfo();
	}

	std::cout << "root:\\>";

	std::string command;
	std::getline(std::cin, command);

	if (command == "clear" || command == "cls") 
	{
		system("cls");
	}
	else if (command == "process-smi")
	{
		this->printProcessInfo();
	}
	else if (command == "exit")
	{
		ConsoleManager::getInstance()->returnToPreviousConsole();
		ConsoleManager::getInstance()->unregisterScreen(this->name);
	}
}