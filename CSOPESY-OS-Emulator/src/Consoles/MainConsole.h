#include "AConsole.h"

class MainConsole : public AConsole
{
public:
	MainConsole(String name);

	void onEnabled() override;
	void process() override;
	void display() override;

private:
    void printHeader() const override;
    void exit() const override;
    void help() const override;

    // Additional Commands
    void initialize() const;
    void schedulerTest() const;
    void schedulerStop() const;
    void reportUtil() const;
};