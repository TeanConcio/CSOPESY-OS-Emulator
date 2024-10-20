#pragma once
#include "ICommand.h"
class IOCommand :
    public ICommand
{
public:
	IOCommand(int pid);
	void execute() override;
};

