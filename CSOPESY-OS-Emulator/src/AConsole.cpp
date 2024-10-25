#include "AConsole.h"

#include "ConsoleManager.h"


AConsole::AConsole(const String& name)
{
	this->name = name;
	this->history = std::vector<String>();
}


void AConsole::onEnabled()
{
	// If history is empty, print the header
	if (this->history.empty()) {
		this->printHeader();
	}
	else {
		this->printHistory();
	}
}

void AConsole::onDisabled()
{
}


void AConsole::writeToConsoleHistory(const String& input, const bool onlySaveToHistory)
{
	// If last item of history doesn't have a newline, continue on the same line
	if (!this->history.empty() && this->history.back().back() != '\n')
		this->history.back() += input;
	else
		this->history.push_back(input);

	if (onlySaveToHistory == false)
		std::cout << input;
}


String AConsole::getConsoleInputToHistory()
{
	// Get input
	String input;
	std::getline(std::cin, input);

	// If last item of history doesn't have a newline, continue on the same line
	if (!this->history.empty() && this->history.back().back() != '\n')
		this->history.back() += input + '\n';
	else
		this->history.push_back(input + '\n');

	return input;
}


void AConsole::printHistory() const
{
	for (const auto& line : this->history)
	{
		std::cout << line;
	}
}


// TEMP: Print the name
void AConsole::printHeader()
{
	this->writeToConsoleHistory("Console: " + this->name + "\n");
}


/**
 * @brief Clears the console screen and prints the header.
 */
void AConsole::clear()
{
	// Clear the console
	system("cls");
	this->history.clear();
	this->printHeader();
}


/**
 * @brief Returns to the previous console.
 */
void AConsole::exit()
{
	this->writeToConsoleHistory("Exiting console: " + this->name + "\n");
	ConsoleManager::getInstance()->returnToPreviousConsole();
}


/**
 * @brief Displays a list of available commands.
 */
void AConsole::help()
{
	this->writeToConsoleHistory("List of commands: \n");
	this->writeToConsoleHistory("\thelp : Displays this help message\n");
	this->writeToConsoleHistory("\tclear : Clears the console screen\n");
	//this->writeToConsoleHistory("\texit : Exits the console\n");
}


/**
 * @brief Displays a message for an unknown command.
 * 
 * @param command The command that was not found.
 */
void AConsole::commandNotFound(const String& command) 
{
	this->writeToConsoleHistory("Unknown command: " + command + "\n");
	//writeToConsoleHistory("Type 'help' for a list of commands\n");
}