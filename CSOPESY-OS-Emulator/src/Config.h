#pragma once

#include "TypedefRepo.h"
#include "GlobalScheduler.h"

#include <unordered_map>
#include <fstream>

// Singleton class for configurations from config.txt
class Config
{
public:
	// Singleton initialization
	static Config* getInstance() { return Config::sharedInstance; }
	static void initialize();
	static void destroy();

	// Configurations
	static void read();
	static void validate();
	static void set();

private:
	Config() = default;
	~Config() = default;
	Config& operator=(Config const&) {}; // assignment operator is private*
	// Instance
	static Config* sharedInstance;
	// Configurations
	std::unordered_map<String, String> configs;
};

