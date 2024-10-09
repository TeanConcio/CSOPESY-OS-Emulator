#include "AConsole.h"
#include "./../TypedefRepo.h"
#include <thread>
#include <atomic>
#include <mutex>

class MarqueeConsole : public AConsole
{
public:
	MarqueeConsole(String name);
	~MarqueeConsole();

	void onEnabled() override;
	void process() override;
	void display() override;

private:
	void printHeader() const override;
	void clearConsoleArea(int endY);
	void incrementPosition(int& row, int& col, int rowDirection, int colDirection);
	void checkBoundaries(int& row, int& col, int& rowDirection, int& colDirection, int consoleWidth, int consoleHeight, const String& text);
	void printMarqueeTextAtPosition(const String& text, int row, int col);
	void printMarqueeText();
	void printCommandInputField(int consoleHeight);

	String text;
	int refreshRateMs;
	int pollRateMs;
	int consoleWidth;
	int consoleHeight;
	std::atomic<bool> running;
	std::thread marqueeThread;
	std::thread commandThread;
	std::mutex consoleMutex;
};

