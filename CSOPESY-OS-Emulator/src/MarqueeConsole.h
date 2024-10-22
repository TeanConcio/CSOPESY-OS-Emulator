#pragma once

#include "ConsoleManager.h"

#include <Windows.h>
#include <thread>
#include <conio.h>


class MarqueeConsole : public AConsole
{
public:
	MarqueeConsole(const String& name);

	String name;
	bool running = false;

	void onEnabled() override;
	void onDisabled() override;

	void process();
	void display();



private:
	static const int REFRESH_DELAY = 50;
	static const int POLLING_DELAY = 50;

	int marqueeWidth = 100;
	int marqueeHeight = 20;
	int marqueeXSpd = 1;
	int marqueeYSpd = 1;

	String marqueeText = "Hello world in marquee!";
	int marqueeTextSize = marqueeText.length();
	int marqueeX = 0;
	int marqueeY = 0;

	String commandPrompt = "Enter a command for MARQUEE_CONSOLE: ";
	int commandPromptSize = static_cast<int>(marqueeText.length());
	int currentCommandCursorPosition = 0;
	std::vector<char> currentCommand;

	std::thread keyboardThread;
	void pollKeyboard(bool threading);

	bool cursorVisible = true;
	std::thread cursorThread;

	void decideCommand(const String& command) override;
	void printHeader() const;
	void printMarquee() const;
	void printCurrentCommand();
	void toggleCursorVisibility();
	void printHistory() const override;

	void clear() override;
};
