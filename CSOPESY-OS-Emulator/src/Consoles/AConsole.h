#pragma once

#include "./../TypedefRepo.h"
#include <sstream>

// Abstract class of Console (Screen)
class AConsole
{
	//Console with name and if it gets out of scope, destroy it
public:

	AConsole(String name);
	~AConsole() = default;

	String name;
	std::ostringstream buffer;
	std::streambuf* originalCoutBuffer;
	String history;

	// Main Methods
	virtual void onEnabled() = 0; // Called when the console is enabled
	virtual void display() = 0; // Called when the console is displayed and runs per frame
	virtual void process() = 0;	// Called when the console is processing input, commands, or algorithms

	//Friend can access anything in console manager
	friend class ConsoleManager;

// Only Aconsole and descendants can access
// Implemented functions
protected:
	// Display and Commands
	virtual void printHeader() const;
	virtual void clear();
	virtual void beginSavingHistory();
	virtual void stopSavingHistory();
	virtual void exit() const;
	virtual void help() const;
	virtual void commandNotFound(const String command);
};