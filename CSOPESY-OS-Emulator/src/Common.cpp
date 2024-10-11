#include "Common.h"

#include <sstream>  // For std::istringstream

// Helper functions here

//  From the string, each part is separated into a vector (special list), removes all whitespaces
std::vector<String> Common::commandExtractor(const String& command)
{
	std::vector<String> commands;
	std::istringstream stream(command);
	String word;

	while (stream >> word) {
		commands.push_back(word);
	}
	
	return commands;
}


/// <summary>
/// Generate or print a table cell with specified width, text, and alignment
/// </summary>
/// <param name="width">The width of the column</param>
/// <param name="text">The text to print in the cell</param>
/// <param name="alignment">The alignment of the text 
///     ('l' for left, 'r' for right, 'c' for center)</param>
/// <param name="shouldPrint">If true, print the text; otherwise, return it as a string</param>
/// <returns>A string representing the formatted text cell if shouldPrint is false; otherwise, an empty string</returns>
String Common::makeTextCell(
	const int width,
	const String& text,
	const char alignment,
	const bool shouldPrint)
{
	std::ostringstream oss;
	switch (alignment)
	{
	case 'l':
		// Generate text left-aligned
		oss << std::setw(width) << std::left << text;
		break;
	case 'r':
		// Generate text right-aligned
		oss << std::setw(width) << std::right << text;
		break;
	case 'c':
	{
		// Calculate padding for center alignment
		int padding = (width - text.size()) / 2;
		// Extra padding will be 1 if the difference is odd, 0 otherwise
		int extraPadding = (width - text.size()) % 2;
		// Generate text centered
		oss << std::setw(padding) << "" << text
			<< std::setw(padding + extraPadding) << "";
		break;
	}
	default:
		// Default to left alignment
		oss << std::setw(width) << std::left << text;
		break;
	}

	if (shouldPrint) {
		std::cout << oss.str();
		return "";
	}
	else {
		return oss.str();
	}
}


/// <summary>
/// Format a time_t object into a string with the format "(MM/DD/YYYY HH:MM:SSAM/PM)"
/// </summary>
/// <param name="time">The time_t object to format</param>
/// <returns>A string representing the formatted time</returns>
String Common::formatTimeT(const time_t& time)
{
	std::tm timeInfo;
	localtime_s(&timeInfo, &time);
	char timeStr[24];
	std::strftime(timeStr, sizeof(timeStr), "(%m/%d/%Y %I:%M:%S%p)", &timeInfo);
	return timeStr;
}