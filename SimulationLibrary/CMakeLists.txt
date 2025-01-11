cmake_minimum_required(VERSION 3.28)
project(Simulation)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SIMULATION_FILES
        *.cpp
        *.hpp
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_library(SimulationLibrary STATIC ${SIMULATION_FILES})
target_link_libraries(SimulationLibrary ${CURSES_LIBRARIES})
