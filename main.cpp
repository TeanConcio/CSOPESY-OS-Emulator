#include <iostream>

using namespace std;

/*
	Variables
*/


/*
	Functions
*/


/// @brief Prints the banner of the program
void printHeader() {
	//  .d8888b.   .d8888b.   .d88888b.  8888888b.  8888888888 .d8888b. Y88b   d88P
	// d88P  Y88b d88P  Y88b d88P" "Y88b 888   Y88b 888       d88P  Y88b Y88b d88P 
	// 888    888 Y88b.      888     888 888    888 888       Y88b.       Y88o88P  
	// 888         "Y888b.   888     888 888   d88P 8888888    "Y888b.     Y888P   
	// 888            "Y88b. 888     888 8888888P"  888           "Y88b.    888    
	// 888    888       "888 888     888 888        888             "888    888    
	// Y88b  d88P Y88b  d88P Y88b. .d88P 888        888       Y88b  d88P    888    
	//  "Y8888P"   "Y8888P"   "Y88888P"  888        8888888888 "Y8888P"     888    

	// cout << " .d8888b.   .d8888b.   .d88888b.  8888888b.  8888888888 .d8888b. Y88b   d88P\n";
	// cout << "d88P  Y88b d88P  Y88b d88P\" \"Y88b 888   Y88b 888       d88P  Y88b Y88b d88P \n";
	// cout << "888    888 Y88b.      888     888 888    888 888       Y88b.       Y88o88P  \n";
	// cout << "888         \"Y888b.   888     888 888   d88P 8888888    \"Y888b.     Y888P   \n";
	// cout << "888            \"Y88b. 888     888 8888888P\"  888           \"Y88b.    888    \n";
	// cout << "888    888       \"888 888     888 888        888             \"888    888    \n";
	// cout << "Y88b  d88P Y88b  d88P Y88b. .d88P 888        888       Y88b  d88P    888    \n";
	// cout << " \"Y8888P\"   \"Y8888P\"   \"Y88888P\"  888        8888888888 \"Y8888P\"     888    \n";

	//  .d8888b.   .d8888b.            .d88888b.   .d88888b.  8888888b.   .d8888b. Y88b   d88P
	// d88P  Y88b d88P  Y88b          d88P" "Y88b d88P" "Y88b 888   Y88b d88P  Y88b Y88b d88P 
	// 888    888 Y88b.               888     888 888     888 888    888 Y88b.       Y88o88P  
	// 888         "Y888b.            888     888 888     888 888   d88P  "Y888b.     Y888P   
	// 888            "Y88b.          888     888 888     888 8888888P"      "Y88b.    888    
	// 888    888       "888          888     888 888     888 888              "888    888    
	// Y88b  d88P Y88b  d88P          Y88b. .d88P Y88b. .d88P 888        Y88b  d88P    888    
	//  "Y8888P"   "Y8888P"            "Y88888P"   "Y88888P"  888         "Y8888P"     888    

	cout << " .d8888b.   .d8888b.            .d88888b.   .d88888b.  8888888b.   .d8888b. Y88b   d88P\n";
	cout << "d88P  Y88b d88P  Y88b          d88P\" \"Y88b d88P\" \"Y88b 888   Y88b d88P  Y88b Y88b d88P \n";
	cout << "888    888 Y88b.               888     888 888     888 888    888 Y88b.       Y88o88P  \n";
	cout << "888         \"Y888b.            888     888 888     888 888   d88P  \"Y888b.     Y888P   \n";
	cout << "888            \"Y88b.          888     888 888     888 8888888P\"      \"Y88b.    888    \n";
	cout << "888    888       \"888          888     888 888     888 888              \"888    888    \n";
	cout << "Y88b  d88P Y88b  d88P          Y88b. .d88P Y88b. .d88P 888        Y88b  d88P    888    \n";
	cout << "\"Y8888P\"    \"Y8888P\"            \"Y88888P\"   \"Y88888P\"  888         \"Y8888P\"     888    \n";

	cout << "\033[1;32m" << "Hello, Welcome to CSOPESY commandline!\n";
	cout << "\033[1;33m" << "Type 'exit' to quit, 'clear' to clear the screen\n";
}

void initialize() {
	cout << "initialize command recognized. Do something.\n";
}
void screen() {
	cout << "screen command recognized. Do something.\n";
}
void schedulerTest() {
	cout << "scheduler-test command recognized. Do something.\n";
}
void schedulerStop() {
	cout << "scheduler-stop command recognized. Do something.\n";
}
void reportUtil() {
	cout << "report-util command recognized. Do something.\n";
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
	system("cls");
	printHeader();
}

/// @brief Exits the program
void exit() {
	cout << "Exiting program...\n";
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

	printHeader();

	while (true) {
		getCommand();
	}

	return 0;
}