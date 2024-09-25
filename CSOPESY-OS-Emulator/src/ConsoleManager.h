#pragma once

#include <Windows.h>
#include <memory>
#include <unordered_map>

#include "./Consoles/BaseScreen.h"

const String MAIN_CONSOLE = "MAIN_CONSOLE";
const String MARQUEE_CONSOLE = "MARQUEE_CONSOLE";
const String SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE";
const String MEMORY_CONSOLE = "MEMORY_CONSOLE";

class ConsoleManager
{
public:
    static ConsoleManager* getInstance();
    static void initialize();
    static void destroy();

    typedef std::unordered_map<String, std::shared_ptr<AConsole>> ConsoleTable;
    int screenID = 0;

    void drawConsole() const;
    void process() const;

    void switchConsole(String consoleName);
    void switchToScreen(String screenName);
    void registerScreen(std::shared_ptr<BaseScreen> screenRef);
    void unregisterScreen(String screenName);

    void returnToPreviousConsole();
    void exitApplication();
    bool isRunning() const;

    HANDLE getConsoleHandle() const;

    void setCursorPosition(int posX, int posY) const;

private:
    // Private constructor so that no objects can be created.
    ConsoleManager();
    ~ConsoleManager() = default;
    ConsoleManager(ConsoleManager const&) {}; // copy constructor is private
    ConsoleManager& operator=(ConsoleManager const&) {}; // assignment operator is private*
    static ConsoleManager* sharedInstance;

    // Console Management
    ConsoleTable consoleTable;
    std::shared_ptr<AConsole> currentConsole;
    std::shared_ptr<AConsole> previousConsole;

    HANDLE consoleHandle;
    bool running = true;

    // Initialization
    void initializeConsoles();
    void initializeScreens();

    bool hasConsole(const String consoleName) const;
};