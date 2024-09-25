// Common.h
#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include "./Consoles/AConsole.h"
#include "TypedefRepo.h"

class Common
{
public:
    static std::vector<String> commandExtractor(String command);
};

#endif // COMMON_H