#include "./src/ConsoleManager.h"

/// @brief  Main function
int main() {

	// Initialize Singletons
	//InputManager::initialize();
	ConsoleManager::initialize();


	// Main loop
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
	ConsoleManager::destroy();

	return 0;
}