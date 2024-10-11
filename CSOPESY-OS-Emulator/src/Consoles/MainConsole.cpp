#include "MainConsole.h"

/**
 * @brief Constructs a new MainConsole object.
 * 
 * @param name The name of the console.
 */
MainConsole::MainConsole(const String& name) : AConsole(name) {}

/**
 * @brief Processes the input commands.
 * 
 * Waits for user input and processes the command.
 */
void MainConsole::process() {
	this->writeToConsoleHistory("root:\\>:");

	String command;
	command = this->getConsoleInputToHistory();

	this->decideCommand(command);
}

void MainConsole::display() {

}


/**
 * @brief Decides which command to execute.
 *
 * @param command The command to be executed.
 */
void MainConsole::decideCommand(const String& command) {
	// Extract command parts
	std::vector<String> commandParts = Common::commandExtractor(command);

	// Check if command is empty
	if (commandParts[0] == "initialize") {
		this->initialize();
	}
	else if (commandParts[0] == "screen") {
		// Check if enough arguments

		if (commandParts[1] == "-ls" && commandParts.size() == 2)
		{
			this->listProcesses();
		}
		else if (commandParts.size() <= 2) {
			this->commandNotFound(command);
			return;
		}
		//else if (commandParts[1] == "-s") {
		//	// Check if currentProcess exists
		//	for (const auto& currentProcess : this->processTable) {
		//		if (currentProcess->getName() == commandParts[2]) {
		//			this->writeToConsoleHistory("Process " + commandParts[2] + " already exists.\n");
		//			return;
		//		}
		//	}

		//	// TODO: Make lines not a placeholder
		//	auto currentProcess = std::make_shared<Process>(commandParts[2], 100, ConsoleManager::getInstance()->processID);
		//	this->addProcess(currentProcess); // Push to vector
		//	auto screen = std::make_shared<BaseScreen>(currentProcess, commandParts[2]); // Initialize screen with name and currentProcess

		//	// Go to screen itself
		//	ConsoleManager::getInstance()->registerScreen(screen);
		//	ConsoleManager::getInstance()->switchConsole(commandParts[2]);
		//}
		//else if (commandParts[1] == "-r") {
		//	for (const auto& currentProcess : this->processTable) {
		//		if (currentProcess->getName() == commandParts[2]) {
		//			auto screen = std::make_shared<BaseScreen>(currentProcess, commandParts[2]);
		//			ConsoleManager::getInstance()->registerScreen(screen);
		//			ConsoleManager::getInstance()->switchConsole(commandParts[2]);
		//			return;
		//		}
		//	}
		//	this->writeToConsoleHistory("Unable to redraw " + commandParts[2] + ". Was it registered?\n");
		//	return;
		//}
		else 
		{
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
		this->writeToConsoleHistory("Switching to MARQUEE_CONSOLE\n");
		ConsoleManager::getInstance()->switchConsole("MARQUEE_CONSOLE");
	}
	else {
		this->commandNotFound(command);
	}
}


/**
 * @brief Prints the header information.
 * 
 * Prints a welcome message and instructions for using the console.
 */
void MainConsole::printHeader() {
	this->writeToConsoleHistory("  ____ ____   ___  ____  _____ ______   __\n");
	this->writeToConsoleHistory(" / ___/ ___| / _ \\|  _ \\| ____/ ___\\ \\ / /\n");
	this->writeToConsoleHistory("| |   \\___ \\| | | | |_) |  _| \\___ \\\\ V / \n");
	this->writeToConsoleHistory("| |___ ___) | |_| |  __/| |___ ___) || |  \n");
	this->writeToConsoleHistory("\\____|_____/ \\___/|_|   |_____|____/ |_|  \n");
	this->writeToConsoleHistory("-------------------------------------------\n");
	// std::cout << "\033[1;32m" << "Hello, Welcome to CSOPESY commandline!\n";
	// std::cout << "\033[1;33m" << "Type 'exit' to quit, 'clear' to clear the screen\n";
	this->writeToConsoleHistory("Welcome to the CSOPESY Emulator!\n\n");
	this->writeToConsoleHistory("Developers:\n");
	this->writeToConsoleHistory("Concio, Tean Jeremy\n");
	this->writeToConsoleHistory("Ong, Nicole Daphne\n");
	this->writeToConsoleHistory("Teves, Hannah Juliet\n");
	this->writeToConsoleHistory("Yu, Marco Jalen\n\n");
	this->writeToConsoleHistory("Last updated: 10-08-2024\n");
	this->writeToConsoleHistory("-------------------------------------------\n\n");
}

/**
 * @brief Exits the program.
 * 
 * Calls the exitApplication method of the ConsoleManager.
 */
void MainConsole::exit() const {
	ConsoleManager::getInstance()->exitApplication();
}

/**
 * @brief Prints the list of available commands.
 * 
 * Calls the help method from AConsole and adds additional commands.
 */
void MainConsole::help() {
	
	// Call help from AConsole then add on to it
	AConsole::help();

	this->writeToConsoleHistory("\tscreen -s <console_name / screen_name> : Displays the screen\n");
	this->writeToConsoleHistory("\tinitialize : Initializes the program\n");
	this->writeToConsoleHistory("\tscheduler-test : Tests the scheduler\n");
	this->writeToConsoleHistory("\tscheduler-stop : Stops the scheduler\n");
	this->writeToConsoleHistory("\treport-util : Generates a report\n");
	this->writeToConsoleHistory("\tmarquee: Open a threaded marquee console\n");

}

void MainConsole::initialize() {
	this->writeToConsoleHistory("initialize command recognized. Doing something.\n");
}

void MainConsole::schedulerTest() {
	this->writeToConsoleHistory("scheduler-test command recognized. Doing something.\n");
}

void MainConsole::schedulerStop() {
	this->writeToConsoleHistory("scheduler-stop command recognized. Doing something.\n");
}

void MainConsole::reportUtil() {
	this->writeToConsoleHistory("report-util command recognized. Doing something.\n");
}

void MainConsole::listProcesses()	{
	GlobalScheduler* globalScheduler = GlobalScheduler::getInstance();
	this->writeToConsoleHistory("--------------------------------------\n");
	this->writeToConsoleHistory("Running processes :\n");
	// Loop the processes in the scheduler
	globalScheduler->printQueuedProcesses();
	this->writeToConsoleHistory("--------------------------------------\n");
}
