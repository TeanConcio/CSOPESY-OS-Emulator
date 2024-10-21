#include "IETThread.h"

IETThread::IETThread()
{
}

void IETThread::start()
{
	// Detach the thread so it can run independently
	std::thread(&IETThread::run, this).detach();
}

void IETThread::sleep(unsigned int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}