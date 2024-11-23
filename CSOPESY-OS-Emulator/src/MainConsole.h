#pragma once

#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include "Config.h"

#include <memory>


class MainConsole : public AConsole
{
public:
	MainConsole(const String& name);

	// Override everything because main
	void process() override;
	void display() override;

private:
	void decideCommand(const String& command) override;
	void printHeader() override;
	void exit() const;
	void help() override;

	// Additional Commands
	void initialize();
	void schedulerTest();
	void schedulerStop();
	void listProcesses();
	void reportUtil();
	void printProcessInfo();
};