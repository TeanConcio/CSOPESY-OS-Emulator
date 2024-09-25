#include <iostream>
#include "ConsoleManager.h"

using namespace std;

/*
	Variables
*/


/*
	Functions
*/


/// @brief Prints the banner of the program
void printHeader() {
	cout << "  ____ ____   ___  ____  _____ ______   __\n";
	cout << " / ___/ ___| / _ \\|  _ \\| ____/ ___\\ \\ / /\n";
	cout << "| |   \\___ \\| | | | |_) |  _| \\___ \\\\ V / \n";
	cout << "| |___ ___) | |_| |  __/| |___ ___) || |  \n";
	cout << "\\____|_____/ \\___/|_|   |_____|____/ |_|  \n";
	cout << "\033[1;32m" << "Hello, Welcome to CSOPESY commandline!\n";
	cout << "\033[1;33m" << "Type 'exit' to quit, 'clear' to clear the screen\n";
}

void initialize() {
	cout << "initialize command recognized. Doing something.\n";
}
void screen() {
	cout << "screen command recognized. Doing something.\n";
}
void schedulerTest() {
	cout << "scheduler-test command recognized. Doing something.\n";
}
void schedulerStop() {
	cout << "scheduler-stop command recognized. Doing something.\n";
}
void reportUtil() {
	cout << "report-util command recognized. Doing something.\n";
}

// /// @brief Prints the list of commands
// void help() {
// 	cout << "List of commands:\n";
// 	cout << "initialize - Initializes the program\n";
// 	cout << "screen - Displays the screen\n";
// 	cout << "scheduler-test - Tests the scheduler\n";
// 	cout << "scheduler-stop - Stops the scheduler\n";
// 	cout << "report-util - Generates a report\n";
// 	cout << "clear - Clears the screen\n";
// 	cout << "exit - Exits the program\n";
// }

/// @brief Clears the screen and reprints header text
void clear() {
	cout << "clear command recognized. Doing something.\n";
	system("cls");
	printHeader();
}

/// @brief Exits the program
void exit() {
	cout << "exit command recognized. Doing something.\n";
	exit(0);
}

/// @brief Prints a message when the command is not found
void commandNotFound() {
	cout << "Command not found.\n";
}

/// @brief Gets the command from the user
void getCommand() {
	string command;
	cout << "\033[1;0m" << "Enter a command: ";
	cin >> command;

	if (command == "initialize") {
		initialize();
	}
	else if (command == "screen") {
		screen();
	}
	else if (command == "scheduler-test") {
		schedulerTest();
	}
	else if (command == "scheduler-stop") {
		schedulerStop();
	}
	else if (command == "report-util") {
		reportUtil();
	}
	// else if (command == "help") {
	// 	help();
	// }
	else if (command == "clear") {
		clear();
	}
	else if (command == "exit") {
		exit();
	}
	else {
		commandNotFound();
	}
}

/// @brief  Main function
int main() {

	ConsoleManager::initialize();


	printHeader();

	bool running = true;
	while (running) 
	{
	
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();
	
		/*getCommand();*/

		running = ConsoleManager::getInstance()->isRunning();
	}

	return 0;
}