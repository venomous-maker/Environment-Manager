//
// Created by v3n0m on 31/08/24.
//

#include "../headers/ProcessManager.h"
using namespace std;

class ProcessManager : public processManager::IProcessManager {
public:
    pid_t IProcessManager::createProcess(const std::string& command) override{
        this->pid = fork();
        if (this->pid == 0) {
            // Child process
            char* args[] = {const_cast<char*>("/bin/sh"), const_cast<char*>("-c"), const_cast<char*>(command.c_str()), nullptr};
            execvp(args[0], args);
            exit(EXIT_FAILURE);  // execvp only returns on failure
        }
        return pid;  // Parent process
    }

    void IProcessManager::waitForProcess(int options) override {
        waitpid(this->pid, &this->status, options);
    }

    const pid_t IProcessManager::getProcessId()  override{
        return this->pid;
    }

    const int IProcessManager::getProcessStatus() override{
        return this->status;
    }
};
