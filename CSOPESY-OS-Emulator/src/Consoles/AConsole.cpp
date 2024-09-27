#include "AConsole.h"
#include <sstream> // Include this header for std::ostringstream
#include <iostream> // Include this header for std::cout
#include <algorithm>
#include <windows.h> // Include this header for Windows API functions

#include "./../ConsoleManager.h"

/**
 * @brief Constructor for AConsole.
 *
 * @param name The name of the console.
 */
AConsole::AConsole(String name)
{
    this->name = name;
}

void AConsole::onEnabled() {

	if (this->screenBuffer.empty()) {
		this->printHeader();
	}
	else {
		this->restoreScreenContent();
	}
}

void AConsole::onDisabled() {
	this->captureScreenContent();
}

// TEMP: Print the name
void AConsole::printHeader() const
{
    std::cout << "Console: " << this->name << std::endl;
}

/**
 * @brief Clears the console screen and prints the header.
 */
void AConsole::clear() const
{
    system("cls");
    printHeader();
}

/**
 * @brief Returns to the previous console.
 */
void AConsole::exit() const
{
    ConsoleManager::getInstance()->returnToPreviousConsole();
}

/**
 * @brief Displays a list of available commands.
 */
void AConsole::help() const
{
    std::cout << "List of commands: " << std::endl;
    std::cout << "\thelp : Displays this help message" << std::endl;
    std::cout << "\tclear : Clears the console screen" << std::endl;
    std::cout << "\texit : Exits the console" << std::endl;
}

/**
 * @brief Displays a message for an unknown command.
 *
 * @param command The command that was not found.
 */
void AConsole::commandNotFound(const String command) const
{
    std::cout << "Unknown command: " << command << std::endl;
    //std::cout << "Type 'help' for a list of commands" << std::endl;
}

// Utility function to trim trailing spaces from a string
std::string rtrim(const std::string& str) {
    size_t end = str.find_last_not_of(" ");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

void AConsole::captureScreenContent()
{
    // Capture the current screen content and store it in the buffer
    screenBuffer = getCurrentScreenContent();
}

void AConsole::restoreScreenContent() const
{
    // Restore the screen content from the buffer
    std::cout << screenBuffer << std::endl;
}

//void AConsole::printBufferContent() const
//{
//    std::cout << "Buffer Content: " << screenBuffer << std::endl;
//}

// Method to get current screen content with formatting preservation
std::string AConsole::getCurrentScreenContent() const
{
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Invalid console handle." << std::endl;
        return "";
    }

    // Get the size of the console screen buffer
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        std::cerr << "Error: Failed to get console screen buffer info." << std::endl;
        return "";
    }

    // Determine the size of the console screen buffer
    int width = csbi.dwSize.X;
    int height = csbi.dwSize.Y;
    int bufferSize = width * height;

    // Allocate buffer to hold the characters from the console
    CHAR_INFO* buffer = new CHAR_INFO[bufferSize];
    COORD bufferSizeCoord = { (SHORT)width, (SHORT)height };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT readRegion = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };

    // Read the console screen buffer into the buffer
    if (!ReadConsoleOutput(hConsole, buffer, bufferSizeCoord, bufferCoord, &readRegion)) {
        std::cerr << "Error: Failed to read console output." << std::endl;
        delete[] buffer;
        return "";
    }

    // Store the captured content into a string stream while keeping formatting
    std::ostringstream oss;
    for (int row = 0; row < height; ++row) {
        std::string currentLine;
        for (int col = 0; col < width; ++col) {
            char c = buffer[row * width + col].Char.AsciiChar;

            // Handle non-printable characters and empty spaces
            if (c == '\0') {
                currentLine += ' ';  // Replace null characters with spaces to preserve formatting
            }
            else {
                currentLine += c;  // Add the character as it is
            }
        }

        // Trim trailing spaces and append the line if it has content
        std::string trimmedLine = rtrim(currentLine);
        if (!trimmedLine.empty()) {
            oss << trimmedLine << "\n";  // Add the line with a newline
        }
    }

    // Cleanup
    delete[] buffer;

    return oss.str();
}
