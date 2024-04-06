cmake_minimum_required(VERSION 3.26)
project(first_project_DA)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(first_project_DA
        src/ui.cpp
        src/managerLoad.cpp
        src/managerWork.cpp
        src/managerReliability.cpp
        src/managerPipes.cpp
		src/uiMaxFlow.cpp
		src/uiMeetDemands.cpp
)
