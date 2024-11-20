#include "AMemoryAllocator.h"

#include <set>
#include <sstream>  // For std::istringstream
#include <sys/stat.h>  // For mkdir
#include <direct.h>    // For _mkdir on Windows


AMemoryAllocator::AMemoryAllocator(size_t maxMemorySize, AllocationAlgorithm allocationAlgo)
{
	this->maxMemorySize = maxMemorySize;
	this->allocationAlgo = allocationAlgo;
}


size_t AMemoryAllocator::getNumUniqueAllocatedProcesses() const
{
	std::set<std::shared_ptr<Process>> uniqueProcesses;
	for (const auto& entry : allocatedProcesses)
	{
		uniqueProcesses.insert(entry.second);
	}
	return uniqueProcesses.size();
}


int AMemoryAllocator::checkExistsInBackingStore(std::shared_ptr<Process> processAddress, bool deleteStore) const
{
	// Ensure the directory exists
	Common::ensureDirectoryExists(Common::OUTPUT_DIRECTORY);

	// Read the backing_store.txt file
	std::ifstream file(Common::OUTPUT_DIRECTORY + "\\backing_store.txt", std::ios::in);

	if (file.is_open())
	{
		// Read the file line by line in increments of 6 lines
		int lineNumber = 0;
		std::string line;
		while (std::getline(file, line))
		{
			if (lineNumber % 6 == 0)
			{
				// Remove the "Process ID: " prefix
				line = line.substr(12);

				// Get the process ID
				std::istringstream iss(line);
				size_t processID;
				iss >> processID;

				// If the process ID matches the process ID of the process we are looking for
				if (processID == processAddress->getPID())
				{
					// If deleteStore is true, remove the process information from the backing store
					if (deleteStore)
					{
						// Close the file
						file.close();

						// Open the original file for reading and a temporary file for writing
						std::ifstream inFile(Common::OUTPUT_DIRECTORY + "\\backing_store.txt");
						std::ofstream tempFile(Common::OUTPUT_DIRECTORY + "\\backing_store_temp.txt");

						if (inFile.is_open() && tempFile.is_open())
						{
							std::string line;
							bool skipLines = false;
							while (std::getline(inFile, line))
							{
								if (line.find("Process ID: " + std::to_string(processID)) != std::string::npos)
								{
									// Skip the next 6 lines (process information)
									skipLines = true;
								}
								else if (skipLines)
								{
									// Skip lines until we find an empty line
									if (line.empty())
									{
										skipLines = false;
									}
								}
								else
								{
									// Write the line to the temporary file
									tempFile << line << "\n";
								}
							}

							inFile.close();
							tempFile.close();

							// Replace the original file with the temporary file
							std::remove((Common::OUTPUT_DIRECTORY + "\\backing_store.txt").c_str());
							std::rename((Common::OUTPUT_DIRECTORY + "\\backing_store_temp.txt").c_str(), (Common::OUTPUT_DIRECTORY + "\\backing_store.txt").c_str());
						}
					}

					// Return the line number of the process information
					return lineNumber;
				}
			}

			lineNumber++;
		}

		file.close();

		// Process not found
		return -1;
	}
	else
	{
		// File not found
		return -1;
	}
}


void AMemoryAllocator::moveToBackingStore(std::shared_ptr<Process> processAddress) const
{
	// Check if the process exists in the backing store
	int lineNumber = checkExistsInBackingStore(processAddress, false);

	// If the process exists in the backing store, just update the command counter
	if (lineNumber >= 0) {
		std::fstream file(Common::OUTPUT_DIRECTORY + "\\backing_store.txt", std::ios::in | std::ios::out);

		if (file.is_open()) {
			// Move to the specific line
			file.seekg(std::ios::beg);
			for (int i = 0; i < lineNumber; ++i) {
				file.ignore(10000, '\n');		// 10000 is just a large number to skip the entire line
			}

			// Skip the "Process ID" and "Name" lines
			file.ignore(10000, '\n');
			file.ignore(10000, '\n');

			// Read the "Command Counter" line
			std::streampos commandCounterPos = file.tellg();
			file.ignore(10000, '\n');

			// Update the command counter
			std::string newCommandCounter = "Command Counter: " + std::to_string(processAddress->getCommandCounter()) + "\n";
			file.seekp(commandCounterPos);
			file.write(newCommandCounter.c_str(), newCommandCounter.size());

			file.close();
		}

		return;
	}

	// Make string of process information
	std::stringstream ss;
	ss << "Process ID: " << processAddress->getPID() << "\n";
	ss << "Name: " << processAddress->getName() << "\n";
	ss << "Command Counter: " << processAddress->getCommandCounter() << "\n";
	ss << "Memory Required: " << processAddress->getMemoryRequired() << "\n";
	ss << "Page Size: " << this->getFrameSize() << "\n";
	ss << "\n";

	// Ensure the directory exists
	Common::ensureDirectoryExists(Common::OUTPUT_DIRECTORY);

	// Add process information to the end of backing_store.txt file
	std::ofstream file(Common::OUTPUT_DIRECTORY + "\\backing_store.txt", std::ios::app);

	if (file.is_open())
	{
		file << ss.str();
		file.close();
	}
}


std::shared_ptr<Process> AMemoryAllocator::getFromBackingStore(std::shared_ptr<Process> processAddress) const
{
	// Check if the process exists in the backing store
	size_t lineNumber = checkExistsInBackingStore(processAddress, true);

	// Did not find the process in the backing store
	if (lineNumber < 0)
	{
		return nullptr;
	}
	else
	{
		return processAddress;
	}
}
