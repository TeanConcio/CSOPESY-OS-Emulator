#include "MainConsole.h"

#include "../ConsoleManager.h"

MainConsole::MainConsole(String name) : AConsole(name) {}

void MainConsole::onEnabled() {
    this->printHeader();
}

void MainConsole::process() {
    String command;
	std::cout << "\033[1;0m" << "Enter a command: ";
	std::getline(std::cin, command);

    std::vector<String> commandParts = Common::commandExtractor(command);

	if (commandParts[0] == "initialize") {
		this->initialize();
	}
	else if (commandParts[0] == "screen") {
		if (commandParts.size() <= 2) {
			this->commandNotFound(command);
			return;
		}
        else if (commandParts[1] == "-s") {
			for (const auto& process : this->processTable) {
				if (process->name == commandParts[2]) {
					std::cout << "Process " << commandParts[2] << " already exists." << std::endl;
					return;
				}
			}

			auto process = std::make_shared<Process>(commandParts[2], 100, ConsoleManager::getInstance()->prcoessID);
			this->addProcess(process);
			auto screen = std::make_shared<BaseScreen>(process, commandParts[2]);

			ConsoleManager::getInstance()->registerScreen(screen);
            ConsoleManager::getInstance()->switchConsole(commandParts[2]);
        }
        else if (commandParts[1] == "-r") {
			for (const auto& process : this->processTable) {
				if (process->name == commandParts[2]) {
					auto screen = std::make_shared<BaseScreen>(process, commandParts[2]);
					ConsoleManager::getInstance()->registerScreen(screen);
					ConsoleManager::getInstance()->switchConsole(commandParts[2]);
					return;
				}	
			}
			std::cerr << "Unable to redraw " << commandParts[2] << ". Was it registered?" << std::endl;
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
	else {
		this->commandNotFound(command);
	}
}

void MainConsole::display() {

}

void MainConsole::printHeader() const {
    std::cout << "  ____ ____   ___  ____  _____ ______   __\n";
    std::cout << " / ___/ ___| / _ \\|  _ \\| ____/ ___\\ \\ / /\n";
    std::cout << "| |   \\___ \\| | | | |_) |  _| \\___ \\\\ V / \n";
    std::cout << "| |___ ___) | |_| |  __/| |___ ___) || |  \n";
    std::cout << "\\____|_____/ \\___/|_|   |_____|____/ |_|  \n";
    std::cout << "\033[1;32m" << "Hello, Welcome to CSOPESY commandline!\n";
    std::cout << "\033[1;33m" << "Type 'exit' to quit, 'clear' to clear the screen\n";
}

/// @brief Exits the program
void MainConsole::exit() const {
	//std::cout << "exit command recognized. Doing something.\n";
    ConsoleManager::getInstance()->exitApplication();
}

/// @brief Prints the list of commands
void MainConsole::help() const {
    
    AConsole::help();

	std::cout << "\tscreen -s <console_name / screen_name> : Displays the screen\n";
	std::cout << "\tinitialize : Initializes the program\n";
	std::cout << "\tscheduler-test : Tests the scheduler\n";
	std::cout << "\tscheduler-stop : Stops the scheduler\n";
	std::cout << "\treport-util : Generates a report\n";
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
