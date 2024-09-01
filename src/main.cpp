#include "libs/headers/ProcessManager.h"
#include "libs/classes/Emulator.h"
#include <string>
#include <iostream>

// Function to concatenate all command-line arguments into a single string
std::string concatenateArguments(int argc, char* argv[]) {
    std::string command;
    for (int i = 1; i < argc; ++i) {
        command += argv[i];
        if (i < argc - 1) {
            command += " ";  // Add a space between arguments
        }
    }
    return command;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No command provided." << std::endl;
        return 1;
    }

    // Create an instance of the Emulator
    processManager::IEmulator* emulator = new Emulator();

    // Concatenate all command-line arguments into a single command string
    std::string command = concatenateArguments(argc, argv);

    // Run the command
    emulator->runCommand(command);

    // Main process is done, but we should monitor any child processes
    int status;
    pid_t child_pid;
    while ((child_pid = wait(&status)) > 0) {
        if (WIFEXITED(status)) {
            std::cout << "Process " << child_pid << " exited with status " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cout << "Process " << child_pid << " killed by signal " << WTERMSIG(status) << std::endl;
        }
    }

    // Clean up
    delete emulator;

    return 0;
}
