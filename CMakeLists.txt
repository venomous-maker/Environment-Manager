cmake_minimum_required(VERSION 3.28)
project(EnvironmentManager)

set(CMAKE_CXX_STANDARD 17)

add_executable(EnvironmentManager src/main.cpp
        src/libs/classes/ProcessManager.h
        src/libs/headers/ProcessManager.h
        src/libs/classes/ResourceManager.h
        src/libs/classes/Logger.h
        src/libs/classes/Emulator.h
)
