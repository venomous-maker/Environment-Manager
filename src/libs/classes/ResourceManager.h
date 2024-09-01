//
// Created by v3n0m on 01/09/24.
//

#include "../headers/ProcessManager.h"

// ResourceMonitor: Tracks resource usage such as CPU, memory, and I/O
class ResourceMonitor : public processManager::IResourceMonitor {
public:
    ResourceUsage getUsage(pid_t pid) override {
        ResourceUsage usage = {0.0, 0, 0, 0};
        struct rusage r_usage{};
        if (getrusage(RUSAGE_CHILDREN, &r_usage) == 0) {
            usage.cpuTime = r_usage.ru_utime.tv_sec + r_usage.ru_utime.tv_usec / 1e6;
            usage.memoryUsage = r_usage.ru_maxrss;
        }

        std::ifstream ioFile("/proc/" + std::to_string(pid) + "/io");
        std::string line;
        while (std::getline(ioFile, line)) {
            if (line.find("rchar:") == 0) {
                usage.ioRead = std::stol(line.substr(7));
            } else if (line.find("wchar:") == 0) {
                usage.ioWrite = std::stol(line.substr(7));
            }
        }
        return usage;
    }
};