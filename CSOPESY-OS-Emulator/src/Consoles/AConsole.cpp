#include "AConsole.h"

#include "./../ConsoleManager.h"

void AConsole::printHeader() const
{
    std::cout << "Console: " << this->name << std::endl;
}

void AConsole::clear() const
{
    system("cls");
    printHeader();
}

void AConsole::exit() const
{
    ConsoleManager::getInstance()->returnToPreviousConsole();
}

void AConsole::help() const
{
    std::cout << "List of commands: " << std::endl;
    std::cout << "\thelp : Displays this help message" << std::endl;
    std::cout << "\tclear : Clears the console screen" << std::endl;
    std::cout << "\texit : Exits the console" << std::endl;
}

void AConsole::commandNotFound(const String command) const
{
    std::cout << "Unknown command:" << command << std::endl;
    //std::cout << "Type 'help' for a list of commands" << std::endl;
}
