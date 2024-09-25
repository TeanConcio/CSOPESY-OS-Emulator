#include "ConsoleManager.h"

#include <iostream>

//#include "MainConsole.h"
//#include "MarqueeConsole.h"
//#include "SchedulingConsole.h"
//#include "MemorySimulationConsole.h"


ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy()
{
	delete sharedInstance;
}

void ConsoleManager::switchConsole(std::string consoleName)
{
	if (this->consoleTable.contains(consoleName))
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "Console name " << consoleName << " not found. Was it initialized?" << std::endl;
	}
}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef)
{
	if (this->consoleTable.contains(screenRef->getName()))
	{
		std::cerr << "Screen name " << screenRef->getName() << " already exists. Please use a different name." << std::endl;
		return;
	}

	this->consoleTable[screenRef->getName()] = screenRef;
}

void ConsoleManager::switchToScreen(std::string screenName)
{
	if (this->consoleTable.contains(screenName))
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[screenName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "Screen name " << screenName << " not found. Was it initialized?" << std::endl;
	}
}

ConsoleManager::ConsoleManager()
{
	this->running = true;

	//initialize consoles
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	//const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	//const std::shared_ptr<MarqueeConsole> marqueeConsole = std::make_shared<MarqueeConsole>();
	//const std::shared_ptr<SchedulingConsole> schedulingConsole = std::make_shared<SchedulingConsole>();
	//const std::shared_ptr<MemorySimulationConsole> memoryConsole = std::make_shared<MemorySimulationConsole>();

	//this->consoletable[main_console] = mainconsole;
	//this->consoletable[marquee_console] = marqueeconsole;
	//this->consoletable[scheduling_console] = schedulingconsole;
	//this->consoletable[memory_console] = memoryconsole;

	this->switchConsole(MAIN_CONSOLE);

}