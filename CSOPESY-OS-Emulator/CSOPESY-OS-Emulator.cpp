#include "./src/ConsoleManager.h"
#include "./src/GlobalScheduler.h"

/// @brief  Main function
int main() {

	// Initialize Singletons
	//InputManager::initialize();
	ConsoleManager::initialize();
	GlobalScheduler::initialize();

	// Main loop
	// Any currentProcess that happens in between every frame
	// Every frame draw console
	// Process waits for now because no marquee
	bool running = true;
	// GlobalScheduler::start();
	while (running)
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

		/*getCommand();*/

		running = ConsoleManager::getInstance()->isRunning();
	}


	// Destroy Singletons
	//InputManager::destroy();
	ConsoleManager::destroy();
	GlobalScheduler::destroy();

	return 0;
}