#pragma once
#include "ICommand.h"
#include "../TypedefRepo.h"

// implements the ICommand interface
class PrintCommand :
	public ICommand
{
public:
	PrintCommand(const int pid, const String& toPrint);
	void execute() override;
private:
	String toPrint;
};

