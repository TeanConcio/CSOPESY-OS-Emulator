#pragma once

#include <thread>

// Interface
class IETThread
{
public:
	// Constructor with a parameter to set the detach/join flag
	IETThread(bool detach = true) : detachThread(detach) {}

	// Destructor to join the thread if it is joinable
	virtual ~IETThread() { join(); }

	// Start the thread
	virtual void start();

	// Join the thread
	void join();

	// Static sleep method
	static void sleep(unsigned int ms = 1) { std::this_thread::sleep_for(std::chrono::milliseconds(ms * 100)); }

protected:
	// Pure virtual run method
	virtual void run() = 0;

private:
	std::thread thread; // Thread object
	bool detachThread;  // Flag to indicate whether to detach or join the thread
};

