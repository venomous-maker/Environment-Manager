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
        return this->pid;
    }

    void waitForProcess(int options) override {
        if (waitpid(this->pid, &this->status, options) == -1) {
            // Handle waitpid failure
            std::cerr << "waitpid failed" << std::endl;
        }
    }

    pid_t getProcessId() const override {
        return this->pid;
    }

    int getProcessStatus() const override {
        return this->status;
    }

    // Suspend a process
    bool suspendProcess() const override {
        if (kill(this->pid, SIGSTOP) == 0) {
            return true;
        }

        std::cerr << "Failed to suspend process " << this->pid << std::endl;
        return false;

    }

    bool suspendProcess(pid_t __pid__) const override {
        if (kill(__pid__, SIGSTOP) == 0) {
            return true;
        }

        std::cerr << "Failed to suspend process " << this->pid << std::endl;
        return false;

    }

    // Resume a suspended process
    bool resumeProcess(pid_t pid) const override{
        if (kill(pid, SIGCONT) == 0) {
            return true;
        } else {
            std::cerr << "Failed to resume process " << pid << std::endl;
            return false;
        }
    }

    // Kill a process
    bool killProcess(pid_t pid) const override{
        if (kill(pid, SIGKILL) == 0) {
            removeActiveProcess(pid);
            return true;
        } else {
            std::cerr << "Failed to kill process " << pid << std::endl;
            return false;
        }
    }

    // List all active processes
    void listActiveProcesses() const  override{
        std::cout << "Active Processes:" << std::endl;
        for (pid_t pid : activeProcesses) {
            std::cout << "PID: " << pid << std::endl;
        }
    }

    // List all system processes
    void listSystemProcesses() const override{
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
            std::cerr << "Failed to open /proc directory." << std::endl;
        }
    }


};
#endif
