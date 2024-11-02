# CSOPESY-Operating System Emulator

This project serves as partial fulfillment of the CSOPESY Course


Tean Concio S11

Nicole Ong S12

Hannah Teves S12

Marco Yu S12


## Description

An operating system emulator created in C++. To configure the parameters for your CPU scheduler and process attributes before starting, set them in config.txt. To use the default configuration, simply leave it as is.
To run the program, use Run/Debug in an IDE such as Visual Studio. The entry class file with the main function is located in CSOPESY-OS-Emulator.cpp.

## On Launch

Once launched, the only available commands are the following

### initialize

Initialize the processor configuration of the application. Will use the configurations set in config.txt.


### exit

Terminates the console.

## Commands

After initialization, you will have access to all the other commands.

### screen -s &lt;process name>

Create a new process if process does not already exist and moves to the process screen


### screen -r &lt;process name>

Accesses the screen if the process is still running


### screen -ls 

Lists all processes


### scheduler-test

Continuously generates a batch of dummy processes for the CPU scheduler. Each process is accessible via the “screen” command.


### scheduler-stop

Stops generating dummy processes.


### report-util

Saves all processes into text file – "csopesylog.txt." 


### clear

Clears the screen and reprints the header.


### help

Displays all commands


### exit

Terminates the console.


## Inside a process screen

Inside a process screen, you will have access to these commands


### process-smi

Prints a simple information of the process


### help

Displays all commands


### exit

Returns to the main menu
