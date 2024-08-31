//
// Created by v3n0m on 01/09/24.
//

#include "../headers/ProcessManager.h"
using namespace  processManager;
// Logger: Captures and logs output, resource usage, and execution time
class Logger : public ILogger{
public:
    explicit Logger(const std::string& logFile) : logStream(logFile, std::ios::out | std::ios::app) {}

    void logResourceUsage(const IResourceMonitor::ResourceUsage& usage) override {
        logStream << "CPU Time: " << usage.cpuTime << " sec, "
                  << "Memory Usage: " << usage.memoryUsage << " KB, "
                  << "I/O Read: " << usage.ioRead << " bytes, "
                  << "I/O Write: " << usage.ioWrite << " bytes" << std::endl;
    }

    void logExecutionTime(double time) override {
        logStream << "Execution Time: " << time << " seconds" << std::endl;
    }

private:
    std::ofstream logStream;
};