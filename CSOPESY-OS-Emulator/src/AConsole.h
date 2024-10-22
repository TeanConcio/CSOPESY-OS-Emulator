#pragma once

#include "TypedefRepo.h"
#include <sstream>

// Abstract class of Console (Screen)
class AConsole
{
	//Console with name and if it gets out of scope, destroy it
public:

	AConsole(const String& name);
	~AConsole() = default;

	String name;
	std::vector<String> history;

	// Main Methods
	virtual void onEnabled(); // Called when the console is enabled
	virtual void onDisabled(); // Called when the console is disabled
	virtual void display() = 0; // Called when the console is displayed and runs per frame
	virtual void process() = 0;	// Called when the console is processing input, commands, or algorithms

	//Friend can access anything in console manager
	friend class ConsoleManager;

// Only AConsole and descendants can access
// Implemented functions
protected:
	virtual void writeToConsoleHistory(const String& input, const bool onlySaveToHistory = false);
	virtual String getConsoleInputToHistory();
	virtual void printHistory() const;

	// Display and Commands
	virtual void decideCommand(const String& command) = 0;
	virtual void printHeader();
	virtual void clear();
	virtual void exit();
	virtual void help();
	virtual void commandNotFound(const String& command);
};