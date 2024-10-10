#pragma once

#include <thread>

// Interface
class IETThread
{
public:
	IETThread();

	void start();
	static void sleep(int ms = 1000);

protected:
	virtual void run() = 0;
};