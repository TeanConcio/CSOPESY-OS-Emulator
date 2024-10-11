#pragma once
#include "ICommand.h"
#include "../TypedefRepo.h"

// implements the ICommand interface
class PrintCommand :
    public ICommand
{
public:
	PrintCommand(int pid, String& toPrint);
	void execute() override;

	String getPrintMessage() const;
private:
	String toPrint;
};

