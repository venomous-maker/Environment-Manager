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


class IProcessManager {
public:
    pid_t createProcess(const std::string& );
    int waitForProcess(pid_t);
};



#endif //PROCESSMANAGER_H
