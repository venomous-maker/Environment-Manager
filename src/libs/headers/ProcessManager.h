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
#include <csignal>
#include <vector>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <dirent.h>

namespace processManager {
    struct ARGUMENTS {
        bool hasFile = false;
        bool display = false;
        bool kill = false;
        int killPID = 0;
        bool suspend = false;
        int suspendPID = 0;
        bool resume = false;
        int resumePID = 0;
        bool listSystem = false;
        bool listActive = false;
        std::string filename;
        std::string command;
    };

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
        [[nodiscard]] virtual int getProcessStatus() const = 0;
        [[nodiscard]] virtual pid_t getProcessId() const = 0;

        [[nodiscard]] virtual bool suspendProcess() const = 0;
        [[nodiscard]] virtual bool suspendProcess(pid_t) const = 0;
        [[nodiscard]] virtual bool resumeProcess(pid_t) const = 0;
        virtual bool killProcess(pid_t) = 0;
        virtual void listActiveProcesses() const = 0;
        virtual void listSystemProcesses() const = 0;

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
        // Display resource usage
        virtual void displayUsage(const ResourceUsage& usage) = 0;

        virtual ~IResourceMonitor() = default;
    };

    class ILogger {
    protected:
        mutable std::string filename;
    public:
        virtual void log(const std::string& message) = 0;
        virtual void logResourceUsage(const IResourceMonitor::ResourceUsage&) = 0;
        virtual void logExecutionTime(double) = 0;
        virtual void setFilename(std::string filename) = 0;
        virtual ~ILogger() = default;
    };

    class IEmulator {
    public:
        virtual void runCommand(const ARGUMENTS& arguments) = 0;
        virtual ~IEmulator() = default;
        virtual bool suspendProcess(pid_t pid) = 0;
        [[nodiscard]] virtual bool resumeProcess(pid_t pid) const = 0;

        virtual void killProcess(pid_t pid) const = 0;

        virtual void listSystemProcesses() const = 0;

        virtual void listActiveProcesses() const = 0;
        virtual  void displayUsage(pid_t pid) = 0;
        virtual void setFileName(std::string filename) = 0;
    };

} // namespace processManager

#endif // PROCESSMANAGER_H