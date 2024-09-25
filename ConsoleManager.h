#pragma once
#include <memory>
#include <vector>
#include "AConsole.h"
#include <unordered_map>
#include <Windows.h>
#include "TypedefRepo.h"

const String MAIN_CONSOLE = "MAIN_CONSOLE";
const String MARQUEE_CONSOLE = "MARQUEE_CONSOLE";
const String SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE" ;
const String MEMORY_CONSOLE = "MEMORY_CONSOLE";

class ConsoleManager
{
public:
	typedef std::unordered_map<String, std::shared_ptr<AConsole>> ConsoleTable;
	
	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();
	
	void drawConsole() const;
	void process() const;
	void switchConsole(String consoleName);
	void returnToPreviousConsole();
	void exitApplication() ;
	bool isRunning() const;
	
	HANDLE getConsoleHandle() const;

	void setCursorPosition(int posX, int posY) const;
	
private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {}; // copy constructor is private
	ConsoleManager& operator=(ConsoleManager const&) {}; // assignment operator is private*/
	static ConsoleManager* sharedInstance;
	
	ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole ;
	std::previousConsole;
	
	HANDLE consoleHandle;
	boot running = true;
}



















ConsoleManager::ConsoleManager()
{
	this->running = true;
	
	//initialize consoles
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	const std::shared_ptr<MarqueeConsole> marqueeConsole = std::make_shared<MarqueeConsole>();
	const std::shared_ptr<SchedulingConsole> schedulingConsole = std::make_shared<SchedulingConsole>();
	const std::shared_ptr<MemorySimulationConsole> memoryConsole = std::make_shared<MemorySimulationConsole>();
	
	this->consoleTable[MAIN_CONSOLE] = mainConsole;
	this->consoleTable[MARQUEE_CONSOLE] = marqueeConsole;
	this->consoleTable[SCHEDULING_CONSOLE] = schedulingConsole;
	this->consoleTable[MEMORY_CONSOLE] = memoryConsole;
	
	this->switchConsole(MAIN_CONSOLE);
    
}