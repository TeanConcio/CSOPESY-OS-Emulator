#include "Common.h"

#include <sstream>  // For std::istringstream

std::vector<String> Common::commandExtractor(String command) {
    std::vector<String> commands;
    std::istringstream stream(command);
    String word;

    while (stream >> word) {
        commands.push_back(word);
    }
    
    return commands;
}