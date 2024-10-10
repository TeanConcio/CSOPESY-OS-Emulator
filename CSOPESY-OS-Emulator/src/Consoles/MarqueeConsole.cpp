#include "MarqueeConsole.h"


MarqueeConsole::MarqueeConsole(const String& name) : AConsole(name) {}


// Enable the marquee console
void MarqueeConsole::onEnabled() {
	this->running = true;

	// Disable the cursor
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(ConsoleManager::getInstance()->getConsoleHandle(), &cursorInfo);
	cursorInfo.bVisible = false; // Set the cursor visibility to false
	SetConsoleCursorInfo(ConsoleManager::getInstance()->getConsoleHandle(), &cursorInfo);

	// Start the keyboard polling thread
	keyboardThread = std::thread(&MarqueeConsole::pollKeyboard, this, true);

	// Start the cursor blinking thread
	cursorThread = std::thread(&MarqueeConsole::toggleCursorVisibility, this);
}


// Disable the marquee console
void MarqueeConsole::onDisabled() {
	this->running = false;

	// Enable the cursor
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(ConsoleManager::getInstance()->getConsoleHandle(), &cursorInfo);
	cursorInfo.bVisible = true; // Set the cursor visibility to true
	SetConsoleCursorInfo(ConsoleManager::getInstance()->getConsoleHandle(), &cursorInfo);

	// Stop the keyboard polling thread
	if (keyboardThread.joinable()) {
		keyboardThread.join();
	}

	// Stop the cursor blinking thread
	if (cursorThread.joinable()) {
		cursorThread.join();
	}
}


void MarqueeConsole::process() {
	// Update the marquee position
	marqueeX += marqueeXSpd;
	marqueeY += marqueeYSpd;

	// Reverse the speed if the marquee reaches the edge
	if (marqueeX <= 0 ||
		marqueeX >= marqueeWidth - marqueeTextSize - 1) {
		marqueeXSpd *= -1;
	}

	if (marqueeY <= 0 ||
		marqueeY >= marqueeHeight - 1) {
		marqueeYSpd *= -1;
	}

	// Check if still running
	if (running == false)
		this->exit();
}


void MarqueeConsole::display() {
	// Get the current cursor position and screen buffer size
	COORD cursorPos = ConsoleManager::getInstance()->getConsoleBufferInfo().dwCursorPosition;
	SHORT screenHeight = ConsoleManager::getInstance()->getConsoleBufferInfo().srWindow.Bottom - ConsoleManager::getInstance()->getConsoleBufferInfo().srWindow.Top + 1;
	bool screenFits = false;

	ConsoleManager::getInstance()->setCursorPosition(0, 0);

	// Check if the outputs fit on the screen
	if (this->marqueeHeight + this->history.size() + 5 < screenHeight
		&& false
		) {
		// Move the cursor to the beginning of the console
		cursorPos.X = 0;
		cursorPos.Y = 0;
		SetConsoleCursorPosition(ConsoleManager::getInstance()->getConsoleHandle(), cursorPos);

		screenFits = true;
	}
	else {
		// Clear the console
		system("cls");

		screenFits = false;
	}

	// Print the new output
	printHeader();
	printMarquee();

	std::cout << std::endl;

	printCurrentCommand();
	printHistory();

	// Wait for a short delay before refreshing the display
	std::this_thread::sleep_for(std::chrono::milliseconds(MarqueeConsole::REFRESH_DELAY));

	// If screen fits, add more delay because the console is not cleared
	if (screenFits)
		std::this_thread::sleep_for(std::chrono::milliseconds(
			(MarqueeConsole::REFRESH_DELAY + 1) * 25 / (MarqueeConsole::REFRESH_DELAY + 1)
		));
}


// Keyboard Polling
void MarqueeConsole::pollKeyboard(bool threading) {

	do {
		if (_kbhit()) { // Check if a key has been pressed
			// Get the pressed key
			char key = _getch();

			// Check if the key is an arrow key
			if (key == 0 || key == -32) {
				key = _getch();
				switch (key) {
				case 75: // Left arrow key
					if (currentCommandCursorPosition > 0) {
						currentCommandCursorPosition--;
					}
					break;
				case 77: // Right arrow key
					if (currentCommandCursorPosition < currentCommand.size()) {
						currentCommandCursorPosition++;
					}
					break;
				}
			}
			else if (key == 27) { // Escape key
				this->running = false;
				break;
			}
			else if (key == 13) { // Enter key
				currentCommand.push_back('\0');
				this->writeToConsoleHistory("Command processed in MARQUEE_CONSOLE: " + String(currentCommand.data()) + "\n", true);
				this->decideCommand(String(currentCommand.data()));
				currentCommand.clear();
				currentCommandCursorPosition = 0;
			}
			else if (key == 8) { // Backspace key
				if (!currentCommand.empty() && currentCommandCursorPosition > 0) {
					currentCommand.erase(currentCommand.begin() + currentCommandCursorPosition - 1);
					currentCommandCursorPosition--;
				}
			}
			else { // Other keys
				currentCommand.insert(currentCommand.begin() + currentCommandCursorPosition, key);
				currentCommandCursorPosition++;
			}
		}

		// If threading is enabled, sleep for a short delay
		if (threading)
			std::this_thread::sleep_for(
				std::chrono::milliseconds(MarqueeConsole::POLLING_DELAY)
			);
	} while (running && threading); 
}


// Command Decider
void MarqueeConsole::decideCommand(const String& command) {
	// Extract command parts
	std::vector<String> commandParts = Common::commandExtractor(command);

	// Check if command is empty
	if (commandParts.empty()) {
		this->writeToConsoleHistory("No command entered.\n");
	}
	else if (commandParts[0] == "clear" || commandParts[0] == "cls") {
		this->clear();
	}
	else if (commandParts[0] == "help") {
		this->writeToConsoleHistory("Commands available:\n");
		this->writeToConsoleHistory("clear, cls - Clear the console.\n");
		this->writeToConsoleHistory("exit - Exit the console.\n");
		this->writeToConsoleHistory("help - Display this help message.\n");
	}
	else if (commandParts[0] == "exit") {
		this->running = false;
	}
	else {
		this->commandNotFound(command);
	}
}


void MarqueeConsole::printHeader() const {
	std::cout << "*****************************************" << std::endl;
	std::cout << "* Displaying a marquee console! *" << std::endl;
	std::cout << "*****************************************" << std::endl;
}


void MarqueeConsole::printMarquee() const {
	for (int i = 0; i < marqueeHeight; i++) {
		if (i == marqueeY) {
			std::cout <<
				String(marqueeX, ' ') <<
				marqueeText <<
				String((marqueeWidth - (marqueeX + marqueeTextSize)), ' ') <<
				std::endl;
		}
		else {
			std::cout << String(marqueeWidth, ' ') << std::endl;
		}
	}
}


void MarqueeConsole::printCurrentCommand() {
	// Move the cursor to the beginning of the current command line
	COORD cursorPos = ConsoleManager::getInstance()->getConsoleBufferInfo().dwCursorPosition;
	ConsoleManager::getInstance()->setCursorPosition(0, cursorPos.Y);

	// Clear the current command line
	std::cout << String(ConsoleManager::getInstance()->getConsoleBufferInfo().dwSize.X, ' ');

	// Move the cursor back to the beginning of the current command line
	SetConsoleCursorPosition(ConsoleManager::getInstance()->getConsoleHandle(), cursorPos);

	// Print the command prompt and the current command
	std::cout << commandPrompt;

	for (int i = 0; i < currentCommand.size(); ++i) {
		if (i == currentCommandCursorPosition) {
			if (this->cursorVisible)
				std::cout << '_'; // Cursor indicator
			else
				std::cout << ' '; // Space to maintain the cursor position
		}
		std::cout << currentCommand[i];
	}

	// If the cursor is at the end of the command, show the cursor indicator
	if (currentCommandCursorPosition == currentCommand.size()) {
		if (this->cursorVisible)
			std::cout << '_'; // Cursor indicator
		else
			std::cout << ' '; // Space to maintain the cursor position
	}

	std::cout << std::endl;
}


void MarqueeConsole::toggleCursorVisibility() {
	while (running) {
		cursorVisible = !cursorVisible;
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Toggle every 500ms
	}
}


void MarqueeConsole::printHistory() const {
	for (const String& command : history) {
		// If last line, remove the newline before printing the current line
		if (&command == &history.back())
			std::cout << command.substr(0, command.size() - 1);
		else
			std::cout << command;
	}
}


void MarqueeConsole::clear() {
	this->history.clear();
}