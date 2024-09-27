#include "AConsole.h"

#include "./../ConsoleManager.h"

/**
 * @brief Constructor for AConsole.
 * 
 * @param name The name of the console.
 */
AConsole::AConsole(String name)
{
	this->name = name;
    this->history = "";
}

// TEMP: Print the name
void AConsole::printHeader() const
{
    std::cout << "Console: " << this->name << std::endl;
}

/**
 * @brief Clears the console screen and prints the header.
 */
void AConsole::clear()
{
    system("cls");
    this->history = "";
    printHeader();
}

/**
 * @brief Returns to the previous console.
 */
void AConsole::exit() const
{
    ConsoleManager::getInstance()->returnToPreviousConsole();
}

/**
 * @brief Displays a list of available commands.
 */
void AConsole::help() const
{
    std::cout << "List of commands: " << std::endl;
    std::cout << "\thelp : Displays this help message" << std::endl;
    std::cout << "\tclear : Clears the console screen" << std::endl;
    std::cout << "\texit : Exits the console" << std::endl;
}

/**
 * @brief Displays a message for an unknown command.
 * 
 * @param command The command that was not found.
 */
void AConsole::commandNotFound(const String command) const
{
    std::cout << "Unknown command: " << command << std::endl;
    //std::cout << "Type 'help' for a list of commands" << std::endl;
}
