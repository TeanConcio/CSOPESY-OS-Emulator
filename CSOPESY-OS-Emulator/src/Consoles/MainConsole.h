#include "AConsole.h"
#include "./../Process.h"
#include "./../TypedefRepo.h"
#include <memory>

class MainConsole : public AConsole
{
public:
	MainConsole(String name);

    // Override everything because main
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
    void addProcess(std::shared_ptr <Process> process); // Add process to process table

    // List of all the processes
    std::vector<std::shared_ptr<Process>> processTable;
};