//
// Created by v3n0m on 31/08/24.
//

#include "../headers/ProcessManager.h"
using namespace std;

class ProcessManager : public IProcessManager {
    pid_t createProcess(const std::string& command) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            char* args[] = {const_cast<char*>("/bin/sh"), const_cast<char*>("-c"), const_cast<char*>(command.c_str()), nullptr};
            execvp(args[0], args);
            exit(EXIT_FAILURE);  // execvp only returns on failure
        }
        return pid;  // Parent process
    }

    int waitForProcess(pid_t pid) {
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
};
