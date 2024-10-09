#include "MarqueeConsole.h"

#include "../ConsoleManager.h"
#include <windows.h>
#include <conio.h>

/**
 * @brief Constructs a new MarqueeConsole object.
 *
 * @param name The name of the console.
 */
MarqueeConsole::MarqueeConsole(String name) : AConsole(name), text("This is a marquee text!"), refreshRateMs(20), pollRateMs(10), consoleWidth(80), consoleHeight(15), running(true) {}

MarqueeConsole::~MarqueeConsole() {
    if (marqueeThread.joinable()) {
        marqueeThread.join();
    }
	if (commandThread.joinable()) {
		commandThread.join();
	}
}

void MarqueeConsole::onEnabled() {

}

void MarqueeConsole::process() {
    try {
        marqueeThread = std::thread(&MarqueeConsole::printMarqueeText, this);
        commandThread = std::thread(&MarqueeConsole::printCommandInputField, this, consoleHeight);
    }
    catch (const std::exception& e) {
        std::cerr << "Error initializing threads: " << e.what() << std::endl;
        std::abort();
    }
    catch (...) {
        std::cerr << "Unknown error initializing threads." << std::endl;
        std::abort();
    }
}

void MarqueeConsole::display() {

}

void MarqueeConsole::printHeader() const {
    std::cout << "*****************************************\n";
    std::cout << "* Displaying a marquee console! *        \n";
    std::cout << "*****************************************\n";
}

void MarqueeConsole::clearConsoleArea(int endY) {
    system("cls");
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    COORD topLeft = { 0, 0 };
    for (int i = 0; i < endY; ++i) {
        FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X, topLeft, &written);
        FillConsoleOutputAttribute(console, FOREGROUND_BLUE, screen.dwSize.X, topLeft, &written);
        topLeft.Y++;
    }
    SetConsoleCursorPosition(console, { 0, 0 });
}

void MarqueeConsole::incrementPosition(int& row, int& col, int rowDirection, int colDirection) {
    row += rowDirection;
    col += colDirection;
}

void MarqueeConsole::checkBoundaries(int& row, int& col, int& rowDirection, int& colDirection, int consoleWidth, int consoleHeight, const std::string& text) {
    if (row >= consoleHeight || row < 0) {
        rowDirection *= -1;
        row += rowDirection;
    }
    if (col + text.length() >= consoleWidth || col < 0) {
        colDirection *= -1;
        col += colDirection;
    }
}

void MarqueeConsole::printMarqueeTextAtPosition(const std::string& text, int row, int col) {
    for (int i = 0; i < row; ++i) {
        std::cout << "\n";
    }
    std::cout << std::string(col, ' ') << text << "\n";
}

void MarqueeConsole::printMarqueeText() {
    int row = 0;
    int col = 0;
    int rowDirection = 1;
    int colDirection = 1;

    while (running) {
        {
            std::lock_guard<std::mutex> lock(consoleMutex);
            clearConsoleArea(consoleHeight + 3);
            printHeader();

            incrementPosition(row, col, rowDirection, colDirection);
            checkBoundaries(row, col, rowDirection, colDirection, consoleWidth, consoleHeight, text);

            printMarqueeTextAtPosition(text, row, col);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(refreshRateMs));
    }
}

void MarqueeConsole::printCommandInputField(int consoleHeight) {
    std::string command;
    std::vector<std::string> log;
    while (running) {
        if (_kbhit()) {  // Check if a key has been pressed
            char ch = _getch();  // Read the key without waiting for Enter
            if (ch == '\r') {  // If Enter is pressed
                if (command == "exit") {
                    running = false;
                    break;
                }
                log.push_back(command);  // Add command to log
                command.clear();  // Clear the command after processing
            }
            else if (ch == '\b') {  // Handle backspace
                if (!command.empty()) {
                    command.pop_back();
                }
            }
            else {
                command += ch;  // Append the character to the command
            }
        }

        std::lock_guard<std::mutex> lock(consoleMutex);
        // Clear the input area
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD inputPos = { 0, static_cast<SHORT>(consoleHeight + 5) };
        SetConsoleCursorPosition(console, inputPos);
        std::cout << std::string(80, ' ');  // Clear the line
        SetConsoleCursorPosition(console, inputPos);  // Reset cursor position
        std::cout << "Enter command for MARQUEE_CONSOLE: " << command << " ";
        std::cout.flush();

        // Print the log
        for (size_t i = 0; i < log.size(); ++i) {
            SetConsoleCursorPosition(console, { 0, static_cast<SHORT>(consoleHeight + 6 + i) });
            std::cout << "Command processed in MARQUEE_CONSOLE: ";
            std::cout << log[i] << std::string(80 - log[i].length(), ' ');
        }

        // Ensure the cursor is placed correctly after the command
        SetConsoleCursorPosition(console, { static_cast<SHORT>(35 + command.length()), static_cast<SHORT>(consoleHeight + 5) });

        std::this_thread::sleep_for(std::chrono::milliseconds(pollRateMs));
    }
}