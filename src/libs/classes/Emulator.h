//
// Created by v3n0m on 01/09/24.
//
#ifndef EMULATOR_H
#define EMULATOR_H

#include "../headers/ProcessManager.h"
#include "./Logger.h"
#include "./ProcessManager.h"
#include "./ResourceManager.h"
using namespace processManager;

// Emulator: The core class that ties everything together and runs the environment
class Emulator : public IEmulator {
public:
    Emulator() : processManager(std::make_shared<ProcessManager>()),
                 resourceMonitor(std::make_shared<ResourceMonitor>()),
                 logger(std::make_shared<Logger>("emulator.log")) {}

    void runCommand(const std::string& command) override {
        auto start = std::chrono::high_resolution_clock::now();

        pid_t pid = processManager->createProcess(command);
        processManager->waitForProcess(0);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        IResourceMonitor::ResourceUsage usage = resourceMonitor->getUsage(pid);

        logger->log("Command: " + command);
        logger->log("Exit Status: " + std::to_string(processManager->getProcessStatus()));
        logger->logExecutionTime(elapsed.count());
        logger->logResourceUsage(usage);
        logger->log("----------------------");

        resourceMonitor->displayUsage(usage);
    }

    // Other emulator methods to suspend, resume, kill processes
    bool suspendProcess(pid_t pid) {
        return this->processManager->suspendProcess(pid);
    }

    bool resumeProcess(pid_t pid) const{
       return processManager->resumeProcess(pid);
    }

    void killProcess(pid_t pid) const{
        processManager->killProcess(pid);
    }

    void listSystemProcesses() const{
        processManager->listSystemProcesses();
    }

    void listActiveProcesses() const{
        processManager->listActiveProcesses();
    }

private:
    std::shared_ptr<IProcessManager> processManager;
    std::shared_ptr<IResourceMonitor> resourceMonitor;
    std::shared_ptr<ILogger> logger;
};

#endif