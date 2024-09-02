//
// ProcessManager.cpp
// Created by v3n0m on 31/08/24.
//
#ifndef PROCESSMANAGER_HH
#define PROCESSMANAGERH_H
#include "../headers/ProcessManager.h"

class ProcessManager : public processManager::IProcessManager {
public:
    pid_t createProcess(const std::string& command) override {
        pid = fork();
        if (pid == -1) {
            // Handle fork failure
            std::cerr << "Fork failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            // Child process
            char* args[] = {const_cast<char*>("/bin/sh"), const_cast<char*>("-c"), const_cast<char*>(command.c_str()), nullptr};
            if (execvp(args[0], args) == -1) {
                // Handle execvp failure
                std::cerr << "Execution failed" << std::endl;
                exit(EXIT_FAILURE);  // execvp only returns on failure
            }
        }
        // Parent process
        return pid;
    }

    void waitForProcess(int options) override {
        if (waitpid(pid, &status, options) == -1) {
            // Handle waitpid failure
            std::cerr << "waitpid failed" << std::endl;
        }
    }

    pid_t getProcessId() const override {
        return pid;
    }

    int getProcessStatus() const override {
        return status;
    }
};
#endif
