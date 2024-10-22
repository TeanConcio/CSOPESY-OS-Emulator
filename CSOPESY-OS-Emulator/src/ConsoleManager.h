#pragma once

#include <Windows.h>
#include <memory>
#include <unordered_map>

#include "BaseScreen.h"

// Default names
const String MAIN_CONSOLE = "MAIN_CONSOLE";
const String MARQUEE_CONSOLE = "MARQUEE_CONSOLE";
const String SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE";
const String MEMORY_CONSOLE = "MEMORY_CONSOLE";

class ConsoleManager
{
public:
	// Singleton
	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	// Console table key value pair (name and pointer)
	using ConsoleTable = std::unordered_map<String, std::shared_ptr<AConsole>>; // Typedef for console table
	int processID = 1;

	// Calls current active console and uses their display and currentProcess
	void drawConsole() const;
	void process() const;

	void switchConsole(const String& consoleName);
	void switchToScreen(const String& screenName);
	void registerScreen(std::shared_ptr<BaseScreen> screenRef);
	void unregisterScreen(const String& screenName);

	void returnToPreviousConsole();
	void exitApplication();
	bool isRunning() const;

	HANDLE getConsoleHandle() const;\
	CONSOLE_SCREEN_BUFFER_INFO getConsoleBufferInfo() const;
	void setCursorPosition(int posX, int posY) const;

private:
	// Singleton
	// Private constructor so that no objects can be created.
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {}; // copy constructor is private
	ConsoleManager& operator=(ConsoleManager const&) {}; // assignment operator is private*
	static ConsoleManager* sharedInstance;

	// Console Management
	ConsoleTable consoleTable; // Actual definition, private
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;

	bool running = true;
	HANDLE consoleHandle;

	// Initialization
	void initializeConsoles();

	bool hasConsole(const String& consoleName) const;
};