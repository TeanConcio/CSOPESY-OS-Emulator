#pragma once

#include "./../TypedefRepo.h"

// Abstract class of Console (Screen)
class AConsole
{
public:
	AConsole(String name);
	~AConsole() = default;

	String name;

	// Main Methods
	virtual void onEnabled() = 0; // Called when the console is enabled
	virtual void display() = 0; // Called when the console is displayed and runs per frame
	virtual void process() = 0;	// Called when the console is processing input, commands, or algorithms

	friend class ConsoleManager;

protected:
	// Display and Commands
	virtual void printHeader() const;
	virtual void clear() const;
	virtual void exit() const;
	virtual void help() const;
	virtual void commandNotFound(const String command) const;
};