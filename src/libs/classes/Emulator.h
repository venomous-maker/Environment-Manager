//
// Created by v3n0m on 01/09/24.
//
#ifndef EMULATOR_H
#define EMULATOR_H

#include <utility>

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
    Emulator(std::shared_ptr<IProcessManager> process_manager, std::shared_ptr<IResourceMonitor> resource_monitor,
        std::shared_ptr<ILogger> logger)
    {
        this->processManager = std::move(process_manager);
        this->resourceMonitor = std::move(resource_monitor);
        this->logger = std::move(logger);
    }


    void runCommand(const ARGUMENTS& arguments) override {

        auto start = std::chrono::high_resolution_clock::now();

        pid_t pid = processManager->createProcess(arguments.command);
        if (pid == -1) {
            logger->log("Error: Failed to create process for command: " + arguments.command);
            return;
        }

        processManager->waitForProcess(0);
        if(arguments.display) {
            this->displayUsage(pid);
        }

        if(arguments.listActive) {
            this->listActiveProcesses();
        }

        if(arguments.listSystem) {
            this->listSystemProcesses();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        logExecutionDetails(arguments.command, pid, elapsed.count());
    }

    void logExecutionDetails(const std::string& command, pid_t pid, double elapsed) const override {
        IResourceMonitor::ResourceUsage usage = resourceMonitor->getUsage(pid);
        logger->log("Command: " + command);
        logger->log("Exit Status: " + std::to_string(processManager->getProcessStatus()));
        logger->logExecutionTime(elapsed);
        logger->logResourceUsage(usage);
        logger->log("----------------------");
    }


    // Other emulator methods to suspend, resume, kill processes
    bool suspendProcess(pid_t pid) override {
        return this->processManager->suspendProcess(pid);
    }

    [[nodiscard]] bool resumeProcess(pid_t pid) const override{
        return processManager->resumeProcess(pid);
    }

    void killProcess(pid_t pid) const override{
        processManager->killProcess(pid);
    }

    void listSystemProcesses() const override{
        processManager->listSystemProcesses();
    }

    void listActiveProcesses() const override{
        processManager->listActiveProcesses();
    }

    void displayUsage(pid_t pid) override {
        resourceMonitor->displayUsage(resourceMonitor->getUsage(pid));
    }

    void setFileName(std::string filename) override {
        this->logger = std::make_shared<Logger>(filename);
    }

private:
    std::shared_ptr<IProcessManager> processManager;
    std::shared_ptr<IResourceMonitor> resourceMonitor;
    std::shared_ptr<ILogger> logger;
};

#endif