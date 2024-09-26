#pragma once
#include <ctime>
#include "TypedefRepo.h"

class Process
{
public:
    Process(String name, int totalCodeLines);

    String name;
    int currInstructionLine;
    int totalCodeLines;
    std::time_t createdTime;
};
