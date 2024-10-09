#include "MainConsole.h"

#include "../ConsoleManager.h"

/**
 * @brief Constructs a new MainConsole object.
 * 
 * @param name The name of the console.
 */
MainConsole::MainConsole(String name) : AConsole(name) {}

/**
 * @brief Called when the console is enabled.
 * 
 * Prints the header information.
 */
void MainConsole::onEnabled() {
    this->printHeader();
	std::cout << "\033[1;0m" << this->history;
}

/**
 * @brief Processes the input commands.
 * 
 * Waits for user input and processes the command.
 */
void MainConsole::process() {
    String command;
	std::cout << "root:\\>";
	std::getline(std::cin, command);
    this->history += "root:\\>:" + command + "\n";

	// Extract command parts
    std::vector<String> commandParts = Common::commandExtractor(command);
	// this->beginSavingHistory();

	// Check if command is empty
	if (commandParts[0] == "initialize") {
		this->initialize();
	}
	else if (commandParts[0] == "screen") {
		// Check if enough arguments
		if (commandParts.size() <= 2) {
			this->commandNotFound(command);
			// this->stopSavingHistory();
			return;
		}
        else if (commandParts[1] == "-s") {
			// Check if process exists
			for (const auto& process : this->processTable) {
				if (process->name == commandParts[2]) {
					std::cout << "Process " << commandParts[2] << " already exists." << std::endl;
					this->history += "Process " + commandParts[2] + " already exists.\n";
					// this->stopSavingHistory();
					return;
				}
			}

			// TODO: Make lines not a placeholder
			auto process = std::make_shared<Process>(commandParts[2], 100, ConsoleManager::getInstance()->prcoessID);
			this->addProcess(process); // Push to vector
			auto screen = std::make_shared<BaseScreen>(process, commandParts[2]); // Initialize screen with name and process

			// Go to screen itself
			ConsoleManager::getInstance()->registerScreen(screen);
            ConsoleManager::getInstance()->switchConsole(commandParts[2]);
        }
        else if (commandParts[1] == "-r") {
			for (const auto& process : this->processTable) {
				if (process->name == commandParts[2]) {
					auto screen = std::make_shared<BaseScreen>(process, commandParts[2]);
					ConsoleManager::getInstance()->registerScreen(screen);					
					ConsoleManager::getInstance()->switchConsole(commandParts[2]);
					// this->stopSavingHistory();
					return;
				}	
			}
			std::cout << "Unable to redraw " << commandParts[2] << ". Was it registered?" << std::endl;
			this->history += "Unable to redraw " + commandParts[2] + ". Was it registered?\n";
			// this->stopSavingHistory();
			return;
        }
        else {
            this->commandNotFound(command);
        }
	}
	else if (commandParts[0] == "scheduler-test") {
		this->schedulerTest();
	}
	else if (commandParts[0] == "scheduler-stop") {
		this->schedulerStop();
	}
	else if (commandParts[0] == "report-util") {
		this->reportUtil();
	}
	else if (commandParts[0] == "clear" || commandParts[0] == "cls") {
		this->clear();
	}
	else if (commandParts[0] == "exit") {
		this->exit();
	}
    else if (commandParts[0] == "help") {
		this->help();
	}
	else if (commandParts[0] == "marquee") {
		// Switch to marquee screen
		ConsoleManager::getInstance()->switchConsole("MARQUEE_CONSOLE");
	}
	else {
		this->commandNotFound(command);
	}

	// this->stopSavingHistory();
}

void MainConsole::display() {

}

/**
 * @brief Prints the header information.
 * 
 * Prints a welcome message and instructions for using the console.
 */
void MainConsole::printHeader() const {
    std::cout << "  ____ ____   ___  ____  _____ ______   __\n";
    std::cout << " / ___/ ___| / _ \\|  _ \\| ____/ ___\\ \\ / /\n";
    std::cout << "| |   \\___ \\| | | | |_) |  _| \\___ \\\\ V / \n";
    std::cout << "| |___ ___) | |_| |  __/| |___ ___) || |  \n";
    std::cout << "\\____|_____/ \\___/|_|   |_____|____/ |_|  \n";
	std::cout << "-------------------------------------------\n";
    // std::cout << "\033[1;32m" << "Hello, Welcome to CSOPESY commandline!\n";
    // std::cout << "\033[1;33m" << "Type 'exit' to quit, 'clear' to clear the screen\n";
	std::cout << "Welcome to the CSOPESY Emulator!\n\n";
	std::cout << "Developers:\n";
	std::cout << "Concio, Tean Jeremy\n";
	std::cout << "Ong, Nicole Daphne\n";
	std::cout << "Teves, Hannah Juliet\n";
	std::cout << "Yu, Marco Jalen\n\n";
	std::cout << "Last updated: 10-08-2024\n";
	std::cout << "-------------------------------------------\n\n";
}

/**
 * @brief Exits the program.
 * 
 * Calls the exitApplication method of the ConsoleManager.
 */
void MainConsole::exit() const {
	//std::cout << "exit command recognized. Doing something.\n";
    ConsoleManager::getInstance()->exitApplication();
}

/**
 * @brief Prints the list of available commands.
 * 
 * Calls the help method from AConsole and adds additional commands.
 */
void MainConsole::help() const {
    
	// Call help from AConsole then add on to it
    AConsole::help();

	std::cout << "\tscreen -s <console_name / screen_name> : Displays the screen\n";
	std::cout << "\tinitialize : Initializes the program\n";
	std::cout << "\tscheduler-test : Tests the scheduler\n";
	std::cout << "\tscheduler-stop : Stops the scheduler\n";
	std::cout << "\treport-util : Generates a report\n";
	std::cout << "\tmarquee: Open a threaded marquee console\n";
}

void MainConsole::initialize() const {
	std::cout << "initialize command recognized. Doing something.\n";
}

void MainConsole::schedulerTest() const {
	std::cout << "scheduler-test command recognized. Doing something.\n";
}

void MainConsole::schedulerStop() const {
	std::cout << "scheduler-stop command recognized. Doing something.\n";
}

void MainConsole::reportUtil() const {
	std::cout << "report-util command recognized. Doing something.\n";
}

void MainConsole::addProcess(std::shared_ptr <Process> process) {
	this->processTable.push_back(process);
}
