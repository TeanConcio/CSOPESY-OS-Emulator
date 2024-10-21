#pragma once

#include <thread>

// Interface
class IETThread
{
public:
	IETThread();

	virtual void start();
	static void sleep(unsigned int ms = 1);

protected:
	virtual void run() = 0;
};