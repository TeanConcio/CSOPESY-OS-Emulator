// Common.h
#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <iomanip>
#include "AConsole.h"
#include "TypedefRepo.h"

class Common
{
public:
	static std::vector<String> commandExtractor(const String& command);
	static String makeTextCell(const int columnWidth, const String& text, const char alignment, const bool shouldPrint = false);
	static String formatTimeT(const time_t& time);
	static bool isPowerOfTwo(const unsigned int num);
};

#endif // COMMON_H