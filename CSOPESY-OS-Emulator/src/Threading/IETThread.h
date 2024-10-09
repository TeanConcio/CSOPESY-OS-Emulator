#include <thread>

class IETThread
{
public:
	IETThread();
	~IETThread();

	void start();
	static void sleep(int ms = 1000);

protected:
	virtual void run() = 0;
};