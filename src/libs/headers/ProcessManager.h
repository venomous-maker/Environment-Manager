//
// Created by v3n0m on 31/08/24.
//

#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <vector>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <dirent.h>

namespace processManager {

    class IProcessManager {
    protected:
        pid_t pid;
        int status;
        std::vector<pid_t> activeProcesses;

        // Remove a process from activeProcesses list
        void removeActiveProcess(pid_t pid) {
            activeProcesses.erase(
                std::remove(activeProcesses.begin(), activeProcesses.end(), pid),
                activeProcesses.end()
            );
        }

    public:
        IProcessManager() : pid(0), status(0) {}

        virtual pid_t createProcess(const std::string& command) = 0;
        virtual void waitForProcess(int options) = 0;
        virtual int getProcessStatus() const = 0;
        virtual pid_t getProcessId() const = 0;
        virtual bool suspendProcess() const = 0;
        virtual bool suspendProcess(pid_t) const = 0;
        virtual bool resumeProcess(pid_t) const = 0;
        virtual bool killProcess(pid_t) = 0;
        virtual void listActiveProcesses() const;
        virtual void listSystemProcesses() const;

        virtual ~IProcessManager() = default;
    };

    class IResourceMonitor {
    public:
        struct ResourceUsage {
            double cpuTime;      // CPU time in seconds
            long memoryUsage;    // Memory usage in KB
            long ioRead;         // Bytes read
            long ioWrite;        // Bytes written
        };

        IResourceMonitor() = default;

        virtual ResourceUsage getUsage(pid_t pid) = 0;

        virtual ~IResourceMonitor() = default;

        virtual void displayUsage(const ResourceUsage& usage) const;
    };

    class ILogger {
    public:
        virtual void log(const std::string& message) = 0;
        virtual void logResourceUsage(const IResourceMonitor::ResourceUsage&) = 0;
        virtual void logExecutionTime(double) = 0;
        virtual ~ILogger() = default;
    };

    class IEmulator {
    public:
        virtual void runCommand(const std::string& command) = 0;
        virtual ~IEmulator() = default;
    };
} // namespace processManager

#endif // PROCESSMANAGER_H
