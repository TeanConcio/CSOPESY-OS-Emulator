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
	this->writeToConsoleHistory("root:\\> ");

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
	if (commandParts.empty()) 
	{
		return;
	}
	else if (commandParts[0] == "exit") 
	{
		this->exit();
	}
	else if (GlobalScheduler::getInstance()->hasInitialized())
	{
		if (commandParts[0] == "screen") {
			if (commandParts.size() < 2)
			{
				return;
			}
			else if (commandParts[1] == "-ls" && commandParts.size() == 2)
			{
				// Check if too many arguments
				if (commandParts.size() != 2) {
					this->writeToConsoleHistory("Invalid arguments for screen -ls command.\n");
					return;
				}

				this->listProcesses();
			}
			else if (commandParts[1] == "-s") {
				// Check if enough arguments
				if (commandParts.size() != 3) {
					this->writeToConsoleHistory("Invalid arguments for screen -s command.\n");
					return;
				}
				
				// Create a new process and screen
				if (ProcessManager::createUniqueProcess(commandParts[2]) == nullptr) {
					this->writeToConsoleHistory("Process " + commandParts[2] + " already exists. Use \"screen -r\" command.\n");
					return;
				}

				// Switch to the new screen
				this->writeToConsoleHistory("Switching to " + commandParts[2] + ".\n");
				if (ConsoleManager::getInstance()->switchScreen(commandParts[2]) == nullptr) {
					this->writeToConsoleHistory("Unable to switch to " + commandParts[2] + ".\n");
					return;
				}
			}
			else if (commandParts[1] == "-r") {
				// Check if enough arguments
				if (commandParts.size() != 3) {
					this->writeToConsoleHistory("Invalid arguments for screen -r command.\n");
					return;
				}

				// Switch to the new screen
				this->writeToConsoleHistory("Switching to " + commandParts[2] + ".\n");
				if (ConsoleManager::getInstance()->switchScreen(commandParts[2]) == nullptr) {
					this->writeToConsoleHistory("Process " + commandParts[2] + " not found.\n");
					return;
				}
			}
			else {
				this->writeToConsoleHistory("Invalid arguments for screen command.\n");
			}
		}
		else if (commandParts[0] == "help")
		{
			this->help();
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
		else if (commandParts[0] == "marquee") {
			// Switch to marquee screen
			this->writeToConsoleHistory("Switching to MARQUEE_CONSOLE\n");
			ConsoleManager::getInstance()->switchScreen("MARQUEE_CONSOLE");
		}
		else if (commandParts[0] == "vmstat") {
			this->printvmstat();
		}
		else if (commandParts[0] == "process-smi") {
			this->printProcessInfo();
		}
		else
		{
			this->commandNotFound(command);
		}
	}
	else if (commandParts[0] == "initialize")
	{
		this->initialize();
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
	this->writeToConsoleHistory(" \\____|____/ \\___/|_|   |_____|____/ |_|  \n");
	this->writeToConsoleHistory("-------------------------------------------\n");
	// std::cout << "\033[1;32m" << "Hello, Welcome to CSOPESY commandline!\n";
	// std::cout << "\033[1;33m" << "Type 'exit' to quit, 'clear' to clear the screen\n";
	this->writeToConsoleHistory("Welcome to the CSOPESY Emulator!\n\n");
	this->writeToConsoleHistory("Developers:\n");
	this->writeToConsoleHistory("Concio, Tean Jeremy\n");
	this->writeToConsoleHistory("Ong, Nicole Daphne\n");
	this->writeToConsoleHistory("Teves, Hannah Juliet\n");
	this->writeToConsoleHistory("Yu, Marco Jalen\n\n");
	this->writeToConsoleHistory("Last updated: 11-29-2024\n");
	this->writeToConsoleHistory("-------------------------------------------\n\n");
}


/**
 * @brief Exits the program.
 * 
 * Calls the exitApplication method of the ConsoleManager.
 */
void MainConsole::exit() const {
	ProcessManager::stopGeneratingProcessesLoop();
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

	//this->writeToConsoleHistory("\tinitialize : Initialize the processor configuration\n");
	this->writeToConsoleHistory("\tscreen -s : Creates a new process and moves to the process screen\n");
	this->writeToConsoleHistory("\tscreen -r : Accesses a preexisting screen\n");
	this->writeToConsoleHistory("\tscreen -ls : List all running processes\n");
	this->writeToConsoleHistory("\tscheduler-test : Tests the scheduler\n");
	this->writeToConsoleHistory("\tscheduler-stop : Stops the scheduler\n");
	this->writeToConsoleHistory("\treport-util : Generates a report\n");
	this->writeToConsoleHistory("\tmarquee : Open a threaded marquee console\n");
	this->writeToConsoleHistory("\tvmstat : Displays processes, memory details, and page info.\n");
	this->writeToConsoleHistory("\tprocess-smi : Displays memory usage summary.\n");
	this->writeToConsoleHistory("\texit : Terminates the console.\n");

}

/**
 * @brief Initializes the program.
 *
 * Calls the initialize method of the GlobalScheduler and starts the scheduler.
 *
**/
void MainConsole::initialize() {
	this->writeToConsoleHistory("initialize command recognized. Doing something.\n");
	Config::initialize();
	GlobalScheduler::startGlobalScheduler();
}

/**
* @brief Creates test processes for the scheduler.
* 
*/
void MainConsole::schedulerTest() {
	this->writeToConsoleHistory("scheduler-test command recognized. Doing something.\n");
	ProcessManager::startGeneratingProcessesLoop();
}

void MainConsole::schedulerStop() {
	this->writeToConsoleHistory("scheduler-stop command recognized. Doing something.\n");
	ProcessManager::stopGeneratingProcessesLoop();
}


/**
 * @brief Generates a report.
 *
 * Calls the makeListProcessesString method of ProcessManager and writes the output to the console.
 */
void MainConsole::listProcesses() {
	this->writeToConsoleHistory(ProcessManager::makeListProcessesString());
}


/**
 * @brief Generates a report in a log file.
 *
 * Calls the makeListProcessesString method of ProcessManager and writes the output to a file.
 */
void MainConsole::reportUtil() {
	// ListProcesses but printed to a txt file
	String logFileName = "csopesy-log.txt";

	if (Common::writeToFile(logFileName, ProcessManager::makeListProcessesString()))
		this->writeToConsoleHistory("Report generated at csopesy-log.txt!\n");

	else
		this->writeToConsoleHistory("Unable to create log file.\n");

}

/**
 * @brief Prints the output of the vmstat command.
 *
 * Prints the memory usage of the system to the console.
 */
void MainConsole::printvmstat() {
	int maxWidth = 10; // Adjust this value based on your needs

	size_t totalMemorySize = MemoryManagementUnit::getInstance()->getMaxMemorySize();
	size_t usedMemorySize = MemoryManagementUnit::getInstance()->getUsedMemorySize();
	size_t freeMemorySize = totalMemorySize - usedMemorySize;
	size_t idleCpuTicks = GlobalScheduler::getIdleCpuTicks();
	size_t activeCpuTicks = GlobalScheduler::getActiveCpuTicks();
	size_t totalCpuTicks = GlobalScheduler::getTotalCpuTicks();
	size_t pagesPagedIn = MemoryManagementUnit::getPagesPagedIn();
	size_t pagesPagedOut = MemoryManagementUnit::getPagesPagedOut();

	this->writeToConsoleHistory(Common::makeTextCell(maxWidth, std::to_string(totalMemorySize), 'r', false) + " K total memory\n");
	this->writeToConsoleHistory(Common::makeTextCell(maxWidth, std::to_string(usedMemorySize), 'r', false) + " K used memory\n");
	this->writeToConsoleHistory(Common::makeTextCell(maxWidth, std::to_string(freeMemorySize), 'r', false) + " K free memory\n");
	this->writeToConsoleHistory(Common::makeTextCell(maxWidth, std::to_string(idleCpuTicks), 'r', false) + " idle cpu ticks\n");
	this->writeToConsoleHistory(Common::makeTextCell(maxWidth, std::to_string(activeCpuTicks), 'r', false) + " active cpu ticks\n");
	this->writeToConsoleHistory(Common::makeTextCell(maxWidth, std::to_string(totalCpuTicks), 'r', false) + " total cpu ticks\n");
	this->writeToConsoleHistory(Common::makeTextCell(maxWidth, std::to_string(pagesPagedIn), 'r', false) + " pages paged in\n");
	this->writeToConsoleHistory(Common::makeTextCell(maxWidth, std::to_string(pagesPagedOut), 'r', false) + " pages paged out\n");
}

/**
 * @brief Prints the memory usage of each process.
 *
 * Prints the memory usage of each process to the console.
 */
void MainConsole::printProcessInfo() {

	std::vector<String> processNameVector;
	std::vector<size_t> processMemoryUsageVector;
	size_t totalMemoryUsed = MemoryManagementUnit::getUsedMemorySize();
	size_t runningCoreCount = 0;

	// Loop through running cores and get the name and memory usage of the process
	for (std::shared_ptr<CPUCoreThread> coreThread : GlobalScheduler::getRunningCores())
	{
		std::shared_ptr<Process> process = coreThread->getCurrentProcess();
		if (process != nullptr && process->getState() != Process::ProcessState::FINISHED)
		{
			processNameVector.push_back(process->getName());

			// If memory allocator is flat, get the memory required by the process
			// Else, get the memory required by the process in frames
			size_t memoryUsed;
			if (MemoryManagementUnit::getMemoryAllocatorType() == AMemoryAllocator::AllocationAlgorithm::Flat)
				memoryUsed = process->getMemoryRequired();
			else
				memoryUsed = process->getFrameIndices().size() * MemoryManagementUnit::getFrameSize();

			processMemoryUsageVector.push_back(memoryUsed);
			runningCoreCount++;
		}
	}

	this->writeToConsoleHistory("-------------------------------------------\n");
	this->writeToConsoleHistory("| PROCESS-SMI V01.00 Driver Version: 01.00 |\n"); // TBC: Where to get the version?
	this->writeToConsoleHistory("-------------------------------------------\n");
	this->writeToConsoleHistory("CPU-Util: " + std::to_string(runningCoreCount * 100 / GlobalScheduler::getCoreCount()) + "%\n");
	this->writeToConsoleHistory("Memory Usage: " + std::to_string(totalMemoryUsed) + "KB / " + std::to_string(MemoryManagementUnit::getMaxMemorySize()) + "KB\n");
	this->writeToConsoleHistory("Memory Util: " + std::to_string(totalMemoryUsed * 100 / MemoryManagementUnit::getMaxMemorySize()) + "%\n\n");

	this->writeToConsoleHistory("===========================================\n");
	this->writeToConsoleHistory("Running Processes and memory usage:\n");
	this->writeToConsoleHistory("-------------------------------------------\n");
	
	for (size_t i = 0; i < processNameVector.size(); i++)
	{
		this->writeToConsoleHistory(processNameVector[i] + " " + std::to_string(processMemoryUsageVector[i]) + "KB\n");
	}

	this->writeToConsoleHistory("-------------------------------------------\n");
}