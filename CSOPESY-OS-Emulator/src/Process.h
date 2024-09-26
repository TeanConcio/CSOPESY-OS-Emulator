#pragma once
#include <ctime>
#include "TypedefRepo.h"

class Process
{
public:
    Process(String name, int totalCodeLines, int id);

    int id;
    String name;
    int currInstructionLine;
    int totalCodeLines;
    std::time_t createdTime;
};
