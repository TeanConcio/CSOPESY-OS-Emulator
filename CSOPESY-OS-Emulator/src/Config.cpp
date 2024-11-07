#include "Config.h"

Config* Config::sharedInstance = nullptr;

void Config::initialize()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new Config();
	}

	sharedInstance->read();
	sharedInstance->validate();
	sharedInstance->set();
}

void Config::destroy()
{
	delete sharedInstance;
	sharedInstance = nullptr;
}

/**
* Sets the configurations
* Reads the configurations from config.txt
* Validates the configurations
* Sets the scheduler and memory manager
*/
void Config::set()
{
	ProcessManager::setConfigs(sharedInstance->configs);
	GlobalScheduler::setConfigs(sharedInstance->configs);
	MemoryManagementUnit::setConfigs(sharedInstance->configs);
}

/**
* Reads the configurations from config.txt
* Looks for key-value string pairs separated by " " (2 strings/line)
* @return unordered_map<String, String> - the configurations
*/
void Config::read()
{
	std::ifstream file("config.txt");

	if (file.is_open())
	{
		String line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			String parameter, value;
			iss >> parameter >> value;
			sharedInstance->configs[parameter] = value;
		}
		file.close();
	}
}

/**
* Validates the configurations
* If a configuration is missing or invalid, it is set to a default value
* num-cpu: [1, 128] (default: 4)
* scheduler: "fcfs" or "rr" (default: rr)
* quantum-cycles: [1, 2e32] (default: 5)
* batch-process-freq: [1, 2e32] (default: 1)
* min-ins: [1, 2e32] (default: 1000)
* max-ins: [1, 2e32] (default: 2000)
* delay-per-exec: [0, 2e32] (default: 0)
* max-overall-mem: [2, 2e32] (default: 16384)
* mem-per-frame: [2, 2e32] (default: 16)
* mem-per-proc: [2, 2e32] (default: 4096)
*/
void Config::validate()
{
	// Check if num-cpu is not in range [1, 128] or does not exist
	// Default: 4
	try {
		if (sharedInstance->configs.find("num-cpu") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["num-cpu"]) < 1 ||
			std::stoul(sharedInstance->configs["num-cpu"]) > 128)
		{
			sharedInstance->configs["num-cpu"] = "4";
		}
	}
	catch (const std::exception&) {
		std::cout << "Error: num-cpu is not a number. Setting to default value." << std::endl;
		sharedInstance->configs["num-cpu"] = "4";
	}

	// Check if the scheduler is not "fcfs" or "rr"
	// Default: rr
	try {
		if (sharedInstance->configs.find("scheduler") == sharedInstance->configs.end() ||
			(sharedInstance->configs["scheduler"] != "\"fcfs\"" &&
				sharedInstance->configs["scheduler"] != "\"rr\""))
		{
			sharedInstance->configs["scheduler"] = "rr";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["scheduler"] = "rr";
	}

	// Check if quantum-cycles is not in range [1, 2e32] or does not exist
	// Default: 5
	try {
		if (sharedInstance->configs.find("quantum-cycles") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["quantum-cycles"]) < 1)
		{
			sharedInstance->configs["quantum-cycles"] = "5";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["quantum-cycles"] = "5";
	}

	// Check if batch-process-freq is not in range [1, 2e32] or does not exist
	// Default: 1
	try {
		if (sharedInstance->configs.find("batch-process-freq") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["batch-process-freq"]) < 1)
		{
			sharedInstance->configs["batch-process-freq"] = "1";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["batch-process-freq"] = "1";
	}

	// Check if min-ins is not in range [1, 2e32] or does not exist
	// Default: 1000
	try {
		if (sharedInstance->configs.find("min-ins") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["min-ins"]) < 1)
		{
			sharedInstance->configs["min-ins"] = "1000";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["min-ins"] = "1000";
	}

	// Check if max-ins is not in range [1, 2e32] or does not exist
	// Default: 2000
	try {
		if (sharedInstance->configs.find("max-ins") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["max-ins"]) < 1)
		{
			sharedInstance->configs["max-ins"] = "2000";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["max-ins"] = "2000";
	}

	// Check if delay-per-exec is not in range [0, 2e32] or does not exist
	// Default: 0
	try {
		if (sharedInstance->configs.find("delay-per-exec") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["delay-per-exec"]) < 0)
		{
			sharedInstance->configs["delay-per-exec"] = "0";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["delay-per-exec"] = "0";
	}

	// Check if max-overall-mem is not in range [2, 2e32] or does not exist
	// It must also be a power of 2
	// Default: 16384
	try {
		if (sharedInstance->configs.find("max-overall-mem") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["max-overall-mem"]) < 2 ||
			!Common::isPowerOfTwo(std::stoul(sharedInstance->configs["max-overall-mem"])))
		{
			sharedInstance->configs["max-overall-mem"] = "16384";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["max-overall-mem"] = "16384";
	}

	// Check if mem-per-frame is not in range [2, 2e32] or does not exist
	// It must also be a power of 2
	// Default: 16
	try {
		if (sharedInstance->configs.find("mem-per-frame") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["mem-per-frame"]) < 2 ||
			!Common::isPowerOfTwo(std::stoul(sharedInstance->configs["mem-per-frame"])))
		{
			sharedInstance->configs["mem-per-frame"] = "16";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["mem-per-frame"] = "16";
	}

	// Note: In the final implementation, it should be min-mem-per-proc and max-mem-per-proc

	// Check if mem-per-proc is not in range [2, 2e32] or does not exist
	// It must also be a power of 2
	// Default: 4096
	try {
		if (sharedInstance->configs.find("mem-per-proc") == sharedInstance->configs.end() ||
			std::stoul(sharedInstance->configs["mem-per-proc"]) < 2 ||
			!Common::isPowerOfTwo(std::stoul(sharedInstance->configs["mem-per-proc"])))
		{
			sharedInstance->configs["mem-per-proc"] = "4096";
		}
	}
	catch (const std::exception&) {
		sharedInstance->configs["mem-per-proc"] = "4096";
	}
}
