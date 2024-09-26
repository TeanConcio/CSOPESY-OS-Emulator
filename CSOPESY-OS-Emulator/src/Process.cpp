#include "Process.h"

Process::Process(String name, int totalCodeLines)
{
    this->name = name;
    this->currInstructionLine = 0;
    this->totalCodeLines = totalCodeLines;
    this->createdTime = std::time(nullptr);
}