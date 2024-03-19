cmake_minimum_required(VERSION 3.26)
project(first_project_DA)

set(CMAKE_CXX_STANDARD 11)

add_executable(first_project_DA main.cpp
        headers/Graph.h
        headers/MutablePriorityQueue.h
        headers/Reservoir.h
        headers/Element.h
        headers/City.h
        headers/Station.h
)
