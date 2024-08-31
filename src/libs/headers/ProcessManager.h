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

namespace processManager {
    class IProcessManager {
    protected:
        pid_t pid;
        int status;
    public:
        IProcessManager():pid(0),status(0){};
        virtual pid_t createProcess(const std::string& );
        virtual void waitForProcess(int);
        virtual const int getProcessStatus();
        virtual const pid_t getProcessId();
        virtual ~IProcessManager()= delete;
    };
    // ResourceMonitor: Tracks resource usage such as CPU, memory, and I/O
    class IResourceMonitor {
    public:
        struct ResourceUsage {
            double cpuTime;  // CPU time in seconds
            long memoryUsage;  // Memory usage in KB
            long ioRead;  // Bytes read
            long ioWrite;  // Bytes written
        };
        IResourceMonitor();
        virtual ResourceUsage getUsage(pid_t);

        virtual ~IResourceMonitor()=delete;
    };

    class ILogger {
    public:
        explicit ILogger(const std::string& logFile) : logStream(logFile, std::ios::out | std::ios::app) {};

        virtual void logResourceUsage(const IResourceMonitor::ResourceUsage& usage);

        virtual void logExecutionTime(double time);
        virtual ~ILogger()=delete;

    private:
        std::ofstream logStream;
    };
}


#endif //PROCESSMANAGER_H
