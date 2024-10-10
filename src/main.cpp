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

// Function to kill a process by ID
void killProcess(pid_t pid) {
    if (kill(pid, SIGKILL) == 0) {
        std::cout << "Process " << pid << " terminated." << std::endl;
    } else {
        perror("Error killing process");
    }
}

// Function to suspend a process by ID
void suspendProcess(pid_t pid) {
    if (kill(pid, SIGSTOP) == 0) {
        std::cout << "Process " << pid << " suspended." << std::endl;
    } else {
        perror("Error suspending process");
    }
}

// Function to resume a suspended process by ID
void resumeProcess(pid_t pid) {
    if (kill(pid, SIGCONT) == 0) {
        std::cout << "Process " << pid << " resumed." << std::endl;
    } else {
        perror("Error resuming process");
    }
}

// Function to list system processes
void listSystemProcesses() {
    std::cout << "Listing all system processes..." << std::endl;
    system("ps aux");  // Example using the `ps` command
}

// Function to list active processes
void listActiveProcesses() {
    std::cout << "Listing active processes..." << std::endl;
    system("ps -eo pid,stat,cmd | grep -w 'R'");  // Example to show running processes
}

// Handle file command (example to open/display file contents)
void handleFileCommand(const std::string& filename) {
    std::cout << "Processing file: " << filename << std::endl;
    std::string command = "cat " + filename;
    system(command.c_str());
}



// Function to parse and handle commands with multiple arguments
ARGUMENTS handleMultipleCommands(int argc, char* argv[]) {
    ARGUMENTS arguments;
    for (int i = 1; i < argc; ++i) {
        std::string command = argv[i];

        if (command == "file") {
            if (i + 1 < argc) {  // Check if a file name is provided
                arguments.filename = argv[++i];
                handleFileCommand(arguments.filename);
            } else {
                std::cerr << "Error: No filename provided for file command." << std::endl;
            }
        } else if (command == "kill") {
            if (i + 1 < argc) {  // Check if a process ID is provided
                pid_t pid = std::stoi(argv[++i]);
                killProcess(pid);
                arguments.kill = true;
                arguments.killPID = pid;
            } else {
                std::cerr << "Error: No process ID provided for kill command." << std::endl;
            }
        } else if (command == "suspend") {
            arguments.suspend = true;
            if (i + 1 < argc) {
                pid_t pid = std::stoi(argv[++i]);
                suspendProcess(pid);
                arguments.suspendPID = pid;
            } else {
                std::cerr << "Error: No process ID provided for suspend command." << std::endl;
            }
        } else if (command == "resume") {
            if (i + 1 < argc) {
                pid_t pid = std::stoi(argv[++i]);
                resumeProcess(pid);
                arguments.resume = true;
                arguments.resumePID  = pid;
            } else {
                std::cerr << "Error: No process ID provided for resume command." << std::endl;
            }
        } else if (command == "listsystem") {
            arguments.listSystem = true;
        } else if (command == "listactive") {
            arguments.listActive = true;
        } else if (command == "display") {
            arguments.display = true;
        } else {
            arguments.command += " "+command;
        }
    }
    return arguments;
}

int main(int argc, char* argv[]) {
    // Create an instance of the Emulator
    processManager::IEmulator* emulator = new Emulator();

    // Concatenate all command-line arguments into a single command string
    std::string command = concatenateArguments(argc, argv);
    const auto arguments_ = handleMultipleCommands(argc,argv);

    // Run the command
    emulator->runCommand(arguments_);

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