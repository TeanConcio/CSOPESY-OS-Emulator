#pragma once

#include <memory>

#include "AConsole.h"
#include "Process.h"

class Process;



class BaseScreen : public AConsole
{
public:
	explicit BaseScreen(std::shared_ptr<Process> process);
	void onEnabled() override;
	void process() override;
	void display() override;

private:
	std::shared_ptr<Process> attachedProcess;
	bool refreshed = false;

	void decideCommand(const String& command) override;
	void printHeader() override;
	void printProcessInfo();
};