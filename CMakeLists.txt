cmake_minimum_required(VERSION 3.26)
project(first_project_DA)

set(CMAKE_CXX_STANDARD 11)

add_executable(first_project_DA
        src/ui.cpp
        src/managerLoad.cpp
        src/managerWork.cpp
        src/managerReliability.cpp
        src/managerPipes.cpp
)
