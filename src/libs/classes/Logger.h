//
// Created by v3n0m on 01/09/24.
//
#ifndef LOGGER_H
#define LOGGER_H
#include "../headers/ProcessManager.h"
using namespace  processManager;
// Logger: Captures and logs output, resource usage, and execution time
/**
 * @file Logger.h
 *
 * @brief Logs output, resource usage, and execution time of executed commands
 *
 * The logger class is responsible for capturing and logging output, resource
 * usage, and execution time of executed commands. It logs to a file and
 * provides methods to log messages, resource usage, and execution time.
 *
 */
class Logger : public ILogger {
public:
    /**
     * @brief Construct a new Logger object
     *
     * @param logFile The name of the log file to write to.
     *
     * Opens the log file in append mode and logs any errors encountered to
     * standard error.
     */
    explicit Logger(const std::string& logFile) : logStream(logFile, std::ios::out | std::ios::app) {
        if (!logStream) {
            std::cerr << "Error: Could not open log file: " << logFile << std::endl;
        }
    }


    /**
     * @brief Logs a message to the log file
     *
     * @param message The message to log
     */
    void log(const std::string& message) override {
        logStream << message << std::endl;
        logStream.flush();
    }

    /**
     * @brief Logs resource usage information
     *
     * Logs CPU time, memory usage, I/O read, and I/O write
     *
     * @param usage Resource usage information
     */
    void logResourceUsage(const IResourceMonitor::ResourceUsage& usage) override {
        logStream << "CPU Time: " << usage.cpuTime << " sec, "
                  << "Memory Usage: " << usage.memoryUsage << " KB, "
                  << "I/O Read: " << usage.ioRead << " bytes, "
                  << "I/O Write: " << usage.ioWrite << " bytes" << std::endl;
    }
    /**
     * @brief Logs execution time of a command
     *
     * @param time Execution time in seconds
     */
    void logExecutionTime(double time) override {
        logStream << "Execution Time: " << time << " seconds" << std::endl;
    }

    /**
     * @brief Sets the name of the log file to write to
     *
     * If the log file is currently open, it is closed before the new log file is opened.
     * If the new log file cannot be opened, an error message is logged to standard error.
     *
     * @param filename The name of the new log file
     */
    void setFilename(std::string filename) override {
        logStream.close();
        logStream.open(filename, std::ios::out | std::ios::app);
        if (!logStream) {
            std::cerr << "Error: Could not open new log file: " << filename << std::endl;
        }
        this->filename = std::move(filename);
    }

    /**
     * @brief Destructor for Logger class
     *
     * If the log file is currently open, it is closed.
     */
    ~Logger() override {
        if (logStream.is_open()) {
            logStream.close();
        }
    }


private:
    /**
     * @brief Stream to write log messages to a file
     *
     * This stream is used to output log messages, including general messages,
     * resource usage, and execution time, to a specified log file.
     */
    std::ofstream logStream;
};

#endif