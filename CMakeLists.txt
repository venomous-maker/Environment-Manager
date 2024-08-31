cmake_minimum_required(VERSION 3.28)
project(EnvironmentManager)

set(CMAKE_CXX_STANDARD 17)

add_executable(EnvironmentManager src/main.cpp
        src/libs/classes/ProcessManager.cpp
        src/libs/headers/ProcessManager.h
        src/libs/classes/ResourceManager.cpp
        src/libs/classes/Logger.cpp
)
