#include "./src/ConsoleManager.h"
#include "./src/ProcessManager.h"
#include "./src/GlobalScheduler.h"
#include "./src/MemoryManagementUnit.h"



/// @brief  Main function
int main() {

	// Initialize Singletons
	//InputManager::initialize();
	ConsoleManager::initialize();
	ProcessManager::initialize();
	GlobalScheduler::initialize();
	MemoryManagementUnit::initialize();


	// Main loop
	// Any currentProcess that happens in between every frame
	// Every frame draw console
	// Process waits for now because no marquee
	bool running = true;
	while (running)
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

		/*getCommand();*/

		running = ConsoleManager::getInstance()->isRunning();
	}


	// Destroy Singletons
	//InputManager::destroy();
	GlobalScheduler::destroy();
	MemoryManagementUnit::destroy();
	ProcessManager::destroy();
	ConsoleManager::destroy();

	return 0;
}
