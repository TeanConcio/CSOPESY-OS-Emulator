#include <ctime>

class Process
{
public:
    Process(String name, int id, int currInstructionLine, int totalCodeLines)
    {
        this->name = name;
        this->id = id;
        this->currInstructionLine = currInstructionLine;
        this->totalCodeLines = totalCodeLines;
        this->currentTime = std::time(nullptr);
    }

    String name;
    int id;
    int currInstructionLine;
    int totalCodeLines;
    std::time_t currentTime;
};
