#include <string>

// Abstract class of Console (Screen)
class AConsole
{
public:
	typedef std::string String;
	AConsole(String name);
	~AConsole() = default;

	String getName();
	virtual void onEnabled() = 0;
	virtual void display() = 0;
	virtual void process() = 0;

	String name;
	friend class ConsoleManager;
};