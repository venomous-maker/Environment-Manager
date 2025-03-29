//
// ProcessManager.cpp
// Created by v3n0m on 31/08/24.
//

#ifndef PROCESSMANAGER_HH
#define PROCESSMANAGER_HH

#include "../headers/ProcessManager.h"
#include <dirent.h>   // Needed for listing processes
#include <iostream>
#include <unordered_set> // To track active processes

class ProcessManager : public processManager::IProcessManager {
private:
    std::unordered_set<pid_t> activeProcesses; // Track running processes

public:
    pid_t createProcess(const std::string& command) override {
        pid = fork();
        if (pid == -1) {
            std::cerr << "Fork failed" << std::endl;
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            char* args[] = {const_cast<char*>("/bin/sh"), const_cast<char*>("-c"), const_cast<char*>(command.c_str()), nullptr};
            if (execvp(args[0], args) == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        }
        // Parent process: Track the process
        activeProcesses.insert(pid);
        return pid;
    }

    void waitForProcess(int options) override {
        if (waitpid(pid, &status, options) == -1) {
            perror("waitpid failed");
            status = -1;
        }
    }

    [[nodiscard]] pid_t getProcessId() const override {
        return pid;
    }

    [[nodiscard]] int getProcessStatus() const override {
        return status;
    }

    // Suspend a process
    [[nodiscard]] bool suspendProcess() const override {
        if (kill(this->pid, SIGSTOP) == 0) {
            return true;
        }
        std::cerr << "Failed to suspend process " << this->pid << std::endl;
        return false;
    }

    [[nodiscard]] bool suspendProcess(pid_t _pid_) const override {
        if (kill(_pid_, SIGSTOP) == 0) {
            return true;
        }
        std::cerr << "Failed to suspend process " << _pid_ << std::endl;
        return false;
    }

    // Resume a suspended process
    [[nodiscard]] bool resumeProcess(const pid_t pid) const override {
        if (kill(pid, SIGCONT) == 0) {
            return true;
        }
        std::cerr << "Failed to resume process " << pid << std::endl;
        return false;
    }

    // Kill a process
    bool killProcess(pid_t pid) override {
        if (kill(pid, SIGKILL) == 0) {
            activeProcesses.erase(pid); // Remove from active process list
            return true;
        }
        std::cerr << "Failed to kill process " << pid << std::endl;
        return false;
    }

    // List all active processes
    void listActiveProcesses() const override {
        std::cout << "Active Processes:" << std::endl;
        if (activeProcesses.empty()) {
            std::cout << "No active processes." << std::endl;
        } else {
            for (pid_t pid : activeProcesses) {
                std::cout << "PID: " << pid << std::endl;
            }
        }
    }

    // List all system processes
    void listSystemProcesses() const override {
        std::cout << "System Processes:" << std::endl;
        DIR* dir = opendir("/proc");
        if (dir) {
            struct dirent* ent;
            while ((ent = readdir(dir)) != nullptr) {
                if (isdigit(ent->d_name[0])) {
                    std::cout << "PID: " << ent->d_name << std::endl;
                }
            }
            closedir(dir);
        } else {
            perror("Failed to open /proc");
        }
    }
};

#endif
