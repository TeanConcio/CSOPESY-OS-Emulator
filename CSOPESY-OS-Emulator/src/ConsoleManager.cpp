#include "ConsoleManager.h"

#include <iostream>

#include "./Consoles/MainConsole.h"
#include "./Consoles/MarqueeConsole.h"
//#include "SchedulingConsole.h"
//#include "MemorySimulationConsole.h"
#include "./Consoles/BaseScreen.h"


ConsoleManager* ConsoleManager::sharedInstance = nullptr;

/**
 * @brief Gets the singleton instance of the ConsoleManager.
 * 
 * @return ConsoleManager* The singleton instance of the ConsoleManager.
 */
ConsoleManager* ConsoleManager::getInstance()
{
	if (sharedInstance == nullptr) {
        sharedInstance = new ConsoleManager();
    }
    return sharedInstance;
}

/**
 * @brief Initializes the ConsoleManager singleton instance.
 */
void ConsoleManager::initialize()
{
	sharedInstance = new ConsoleManager();
}

/**
 * @brief Destroys the ConsoleManager singleton instance.
 */
void ConsoleManager::destroy()
{
	delete sharedInstance;
	sharedInstance = nullptr;
}

/**
 * @brief Draws the current console.
 * 
 * If there is a current console, it calls the display method of the current console.
 * Otherwise, it prints an error message.
 */
void ConsoleManager::drawConsole() const
{
	if (this->currentConsole != nullptr)
	{
		this->currentConsole->display();
	}
	else
	{
		std::cerr << "There is no assigned console. Please check." << std::endl;
	}
}

/**
 * @brief Processes the current console.
 * 
 * If there is a current console, it calls the process method of the current console.
 * Otherwise, it prints an error message.
 */
void ConsoleManager::process() const
{
	if (this->currentConsole != nullptr)
	{
		this->currentConsole->process();
	}
	else
	{
		std::cerr << "There is no assigned console. Please check." << std::endl;
	}
}

/**
 * @brief Switches to a different console.
 * 
 * Clears the screen, sets the current console to the specified console, and calls the onEnabled method of the new console.
 * 
 * @param consoleName The name of the console to switch to.
 */
void ConsoleManager::switchConsole(String consoleName)
{
	if (hasConsole(consoleName))
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

/**
 * @brief Switches to a different screen.
 * 
 * Clears the screen, sets the current console to the specified screen, and calls the onEnabled method of the new screen.
 * 
 * @param screenName The name of the screen to switch to.
 */
void ConsoleManager::switchToScreen(String screenName)
{
	if (hasConsole(screenName))
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

/**
 * @brief Registers a screen.
 * 
 * Adds the screen to the console table if it does not already exist.
 * 
 * @param screenRef A shared pointer to the screen to be registered.
 */
void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef)
{
	if (hasConsole(screenRef->name))
	{
		std::cerr << "Screen name " << screenRef->name << " already exists. Please use a different name." << std::endl;
		return;
	}

	this->consoleTable[screenRef->name] = screenRef;
	this->prcoessID++;
}

/**
 * @brief Unregisters a screen.
 * 
 * Removes the screen from the console table if it exists.
 * 
 * @param screenName The name of the screen to be unregistered.
 */
void ConsoleManager::unregisterScreen(String screenName)
{
	if (hasConsole(screenName))
	{
		this->consoleTable.erase(screenName);
	}
	else
	{
		std::cerr << "Unable to unregister " << screenName << ". Was it registered?" << std::endl;
	}
}

/**
 * @brief Returns to the previous console.
 * 
 * Clears the screen, sets the current console to the previous console, and calls the onEnabled method of the previous console.
 */
void ConsoleManager::returnToPreviousConsole()
{
	if (this->previousConsole != nullptr)
	{
		system("cls");
		this->currentConsole = this->previousConsole;
		this->previousConsole = nullptr;
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "There is no previous console to return to." << std::endl;
	}
}

void ConsoleManager::exitApplication()
{
	this->running = false;
}

bool ConsoleManager::isRunning() const
{
	return this->running;
}

/**
 * @brief Constructs a new ConsoleManager object.
 * 
 * Initializes the consoles and sets the current console to the main console.
 */
ConsoleManager::ConsoleManager()
{
	this->running = true;

	//initialize consoles
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	this->initializeConsoles();
	//this->initializeScreens();

	this->switchConsole(MAIN_CONSOLE);
}

/**
 * @brief Initializes the consoles for the ConsoleManager.
 * 
 * This function initializes the consoles for the ConsoleManager by creating instances of the different console classes and storing them in the consoleTable.
 * 
 * @note This function is currently commented out and does not have any effect on the program execution.
 */
void ConsoleManager::initializeConsoles()
{
	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>(MAIN_CONSOLE);
	const std::shared_ptr<MarqueeConsole> marqueeConsole = std::make_shared<MarqueeConsole>(MARQUEE_CONSOLE);
	//const std::shared_ptr<SchedulingConsole> schedulingConsole = std::make_shared<SchedulingConsole>();
	//const std::shared_ptr<MemorySimulationConsole> memoryConsole = std::make_shared<MemorySimulationConsole>();

	this->consoleTable[MAIN_CONSOLE] = mainConsole;
	this->consoleTable[MARQUEE_CONSOLE] = marqueeConsole;
	//this->consoleTable[SCHEDULING_CONSOLE] = schedulingConsole;
	//this->consoleTable[MEMORY_CONSOLE] = memoryConsole;
}

//void ConsoleManager::initializeScreens()
//{
//	const std::shared_ptr<Process> process1 = std::make_shared<Process>("Process1", this->prcoessID, 69, 420);
//	const std::shared_ptr<BaseScreen> screen1 = std::make_shared<BaseScreen>(process1, process1->name);
//	this->registerScreen(screen1);
//
//	const std::shared_ptr<Process> process2 = std::make_shared<Process>("Process2", this->prcoessID, 70, 421);
//	const std::shared_ptr<BaseScreen> screen2 = std::make_shared<BaseScreen>(process2, process2->name);
//	this->registerScreen(screen2);
//
//	const std::shared_ptr<Process> process3 = std::make_shared<Process>("Process3", this->prcoessID, 71, 422);
//	const std::shared_ptr<BaseScreen> screen3 = std::make_shared<BaseScreen>(process3, process3->name);
//	this->registerScreen(screen3);
//}


/**
 * @brief Checks if a console exists in the console table.
 * 
 * @param consoleName The name of the console to check.
 * @return true If the console exists in the console table.
 * @return false If the console does not exist in the console table.
 */
bool ConsoleManager::hasConsole(const String consoleName) const {
    return consoleTable.find(consoleName) != consoleTable.end();
}